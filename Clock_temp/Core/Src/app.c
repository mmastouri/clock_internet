/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_DelayUntil/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    18-November-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#include "app.h"


/* Private typedef -----------------------------------------------------------*/
#define TIME_SOURCE_HTTP_HOST   "www.google.com"
#define TIME_SOURCE_HTTP_PORT   80
#define TIME_SOURCE_HTTP_PROTO  ESP_WIFI_TCP

#define ENV_SOURCE_HTTP_HOST    "api.openweathermap.org"
#define ENV_SOURCE_HTTP_PORT    80
#define ENV_SOURCE_HTTP_PROTO   ESP_WIFI_TCP

/** Maximum number of DNS lookup or connection trials */
#define TIME_NET_MAX_RETRY     4
#define WIFI_DEFAULT_TIMEOUT   20000
#define WIFI_TRIAL_TIMEOUT     1000

/** Size of the HTTP read buffer.
 *  Should be large enough to contain a complete HTTP response header. */
#define NET_BUF_SIZE  1000

/* Private variables ---------------------------------------------------------*/
static const char http_request[] = "HEAD / HTTP/1.1\r\nHost: "TIME_SOURCE_HTTP_HOST"\r\n\r\n";
static const char weather_request[] = "GET /data/2.5/weather?q=Tunis"\
                                      "&appid=5a3687f4840a2a222036e33a28eb8298"\
                                      " HTTP/1.1\r\n"\
                                      "Host: api.openweathermap.org\r\n"\
                                      "Connection: keep-alive\r\n"\
                                      "Accept: *" "/" "*\r\n\r\n";

static uint32_t enable_time_count = 0;
static uint32_t enable_time_setting = 0;
static char rxBuffer[NET_BUF_SIZE + 1];

#define USE_HOME_PW

static const ESP_AvHotspot_t Hotspot ={
#ifdef USE_HOME_PW
   .SSID = "MASTER_EXT" ,  
   .PWD  = "OSMNL7182oo21"
#else
   .SSID = "Android" ,  
   .PWD  = "12345678"
#endif     
};

float itemperature = 25;
float ihumidity    = 50;
/*********************************************************************
*
*       App_task
*/
 void App_task (void){
     
  if(BSP_PB_GetState (BUTTON_USER))
  {
    enable_time_count++;
    
    if(enable_time_count == 10)
      enable_time_setting = 1- enable_time_setting ;

     ui_set_setting_mode(enable_time_setting);
  }
  else
  {
    enable_time_count = 0;
  }  
}

/*********************************************************************
*
*       WIFI_Start
*/
ESP_WIFI_Status_t WIFI_Start (ESP_WIFI_Object_t * pxObj){

  ESP_WIFI_Status_t xRet;
  uint8_t  IPAddr[4];
  uint32_t count;
  
  pxObj->Timeout = WIFI_DEFAULT_TIMEOUT;
  pxObj->IsMultiConn = pdFALSE;
  pxObj->ActiveCmd = CMD_NONE;
  
  xRet = ESP_WIFI_Init( pxObj);
  
  if(xRet == ESP_WIFI_STATUS_OK)
  {  
    UI_SetWifiConnecting();

    count = 10;
    while(((xRet = ESP_WIFI_Connect( pxObj, Hotspot.SSID,Hotspot.PWD)) != ESP_WIFI_STATUS_OK)&& (count-- > 0) )
    {
      HAL_Delay(WIFI_TRIAL_TIMEOUT);
    }
    
    if(xRet == ESP_WIFI_STATUS_OK)
    {
      UI_SetWifiConnected(); 
      
      count = 10;  
      while(((xRet = ESP_WIFI_GetHostIP( pxObj, TIME_SOURCE_HTTP_HOST , IPAddr )) != ESP_WIFI_STATUS_OK) && (count-- > 0) )
      {
        HAL_Delay(WIFI_TRIAL_TIMEOUT);
      } 
      if(xRet == ESP_WIFI_STATUS_OK)
      {
        UI_SetInternetAvailable();          
      }
    }
    else
    {
      UI_SetWifiDisconnected();
    }     
  }
  return xRet;
}

/*********************************************************************
*
*       WIFI_SyncData
*/
static ESP_WIFI_Status_t WIFI_SyncData (ESP_WIFI_Object_t * pxObj,  const char *host, const char *request, uint16_t request_len, 
                                        uint16_t Port, ESP_WIFI_ConnType_t type, const char *filter, char ** pDataStr){
    
  ESP_WIFI_Conn_t xConn;
  uint8_t  IPAddr[4];                                              
  uint32_t count;
  ESP_WIFI_Status_t xRet;
  uint16_t usSentBytes;
  uint16_t usRecvBytes;  
  
  count = 10;
  while(((xRet = ESP_WIFI_GetHostIP( pxObj, (char *)host , IPAddr )) != ESP_WIFI_STATUS_OK) && (count-- > 0) )
  {
    HAL_Delay(WIFI_TRIAL_TIMEOUT);
  } 
  
  if(xRet == ESP_WIFI_STATUS_OK)
  {          
    xConn.RemotePort = Port;
    memcpy(xConn.RemoteIP , IPAddr, sizeof(uint32_t));
    xConn.Type = type;
    xConn.TcpKeepAlive = 0;
    xConn.LinkID       = 0;
    count = 10;
    while(((xRet = ESP_WIFI_StartClient( pxObj, &xConn ))!= ESP_WIFI_STATUS_OK) && (count-- > 0) )
    {
      HAL_Delay(WIFI_TRIAL_TIMEOUT);
    }
    
    if(xRet == ESP_WIFI_STATUS_OK)
    {
      if((xRet = ESP_WIFI_Send( pxObj, &xConn , (uint8_t *)request, 
                               request_len, &usSentBytes, pdMS_TO_TICKS(200))) == ESP_WIFI_STATUS_OK)
      {
        int read = 0;
        do {
          xRet = ESP_WIFI_Recv( pxObj, &xConn , (uint8_t*)(rxBuffer + read), NET_BUF_SIZE - read,&usRecvBytes, pdMS_TO_TICKS(1000) );
          if (usRecvBytes > 0)
          {
            read += usRecvBytes;
            *pDataStr = strstr(rxBuffer, filter);
          }  
        }
        while ( (pDataStr == NULL) && ((usRecvBytes > 0) || (xRet == ESP_WIFI_STATUS_TIMEOUT)) && (read < NET_BUF_SIZE));
        
        if (pDataStr != NULL)
        {
          xRet = ESP_WIFI_STATUS_OK;
        }
      }
      ESP_WIFI_StopClient( pxObj, &xConn ); 
    }
  }
  return xRet;
}
/*********************************************************************
*
*       WIFI_SyncClock
*/
ESP_WIFI_Status_t WIFI_SyncClock (ESP_WIFI_Object_t * pxObj){
  
  ESP_WIFI_Status_t xRet;  
  char *dateStr = NULL;
  
  if((xRet = WIFI_SyncData (pxObj, TIME_SOURCE_HTTP_HOST, http_request, sizeof(http_request),
                            TIME_SOURCE_HTTP_PORT, TIME_SOURCE_HTTP_PROTO, "Date: ", &dateStr)) == ESP_WIFI_STATUS_OK)
  {
    
    char day_week[8], month[4];
    int day, year, hour, min, sec;
    RTC_TimeTypeDef sTime;
    
    int count = sscanf(dateStr, "Date: %s %d %s %d %02d:%02d:%02d ", day_week, &day, month, &year, &hour, &min, &sec); 
    
    sTime.Hours = (hour + 1) % 24;
    sTime.Minutes = min;
    sTime.Seconds = (sec + 1) % 60;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    k_SetTime(&sTime) ;
    UI_ForceUpdateTime();
  }
  return xRet;
}


/*********************************************************************
*
*       WIFI_SyncEnvData
*/
ESP_WIFI_Status_t WIFI_SyncEnvData (ESP_WIFI_Object_t * pxObj){
  ESP_WIFI_Status_t xRet;
  char *weatherStr = NULL;
  
  if((xRet = WIFI_SyncData (pxObj, ENV_SOURCE_HTTP_HOST, weather_request, sizeof(weather_request),
                            ENV_SOURCE_HTTP_PORT, ENV_SOURCE_HTTP_PROTO, "\"temp\":", &weatherStr)) == ESP_WIFI_STATUS_OK)
  {  
    
    sscanf(weatherStr, "\"temp\":%f", &itemperature); 
    itemperature -= 273.15;
    weatherStr = strstr(rxBuffer,  "\"humidity\":");
    sscanf(weatherStr, "\"humidity\":%f", &ihumidity);                             
  }
  return xRet;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
