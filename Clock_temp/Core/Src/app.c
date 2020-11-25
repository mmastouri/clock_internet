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

#include "main.h"
#include "ui.h"
#include "rtc.h"

/* Private typedef -----------------------------------------------------------*/
#define TIME_SOURCE_HTTP_HOST   "www.google.com"
#define TIME_SOURCE_HTTP_PORT   80
#define TIME_SOURCE_HTTP_PROTO  ESP_WIFI_TCP

/** Maximum number of DNS lookup or connection trials */
#define TIME_NET_MAX_RETRY  4

/** Size of the HTTP read buffer.
 *  Should be large enough to contain a complete HTTP response header. */
#define NET_BUF_SIZE  1000



/* Private variables ---------------------------------------------------------*/
static const char http_request[] = "HEAD / HTTP/1.1\r\nHost: "TIME_SOURCE_HTTP_HOST"\r\n\r\n";
uint32_t enable_time_count = 0;
uint32_t enable_time_setting = 0;

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

void wifi_task (ESP_WIFI_Object_t * pxObj){
  ESP_WIFI_Conn_t xConn;
  uint8_t  IPAddr[4];
  uint32_t count;
  ESP_WIFI_Status_t xRet;
  uint16_t usSentBytes;
  uint16_t usRecvBytes;  
  char rxBuffer[NET_BUF_SIZE + 1];
  pxObj->Timeout = 20000;
  pxObj->IsMultiConn = pdFALSE;
  pxObj->ActiveCmd = CMD_NONE;
  
  xRet = ESP_WIFI_Init( pxObj);
  
  if(xRet == ESP_WIFI_STATUS_OK)
  {  
    UI_SetWifiConnecting();
    
    count = 10;
    while(((xRet = ESP_WIFI_Connect( pxObj, "Android","12345678")) != ESP_WIFI_STATUS_OK)&& (count-- > 0) )
    {
      HAL_Delay(1000);
    }
    
    if(xRet == ESP_WIFI_STATUS_OK)
    {    
      UI_SetWifiConnected();
    }
    else
    {
      UI_SetWifiDisconnected();
    }
    
    if(xRet == ESP_WIFI_STATUS_OK)
    {       
      count = 10;  
      while(((xRet = ESP_WIFI_GetHostIP( pxObj, TIME_SOURCE_HTTP_HOST , IPAddr )) != ESP_WIFI_STATUS_OK) && (count-- > 0) )
      {
        HAL_Delay(1000);
      }
      
      if(xRet == ESP_WIFI_STATUS_OK)
      {
        xConn.RemotePort = TIME_SOURCE_HTTP_PORT;
        memcpy(xConn.RemoteIP , IPAddr, sizeof(uint32_t));
        xConn.Type = TIME_SOURCE_HTTP_PROTO;
        xConn.TcpKeepAlive = 0;
        xConn.LinkID       = 0;
        count = 10;
        while(((xRet = ESP_WIFI_StartClient( pxObj, &xConn ))!= ESP_WIFI_STATUS_OK) && (count-- > 0) )
        {
          HAL_Delay(1000);
        }
      }
      
      if(xRet == ESP_WIFI_STATUS_OK)
      {
        if((xRet = ESP_WIFI_Send( pxObj, &xConn , (uint8_t *)http_request, 
                                 strlen(http_request), &usSentBytes, pdMS_TO_TICKS(200))) == ESP_WIFI_STATUS_OK)
        {
          
          if((xRet = ESP_WIFI_Recv( pxObj, &xConn , (uint8_t*)rxBuffer, NET_BUF_SIZE,&usRecvBytes, pdMS_TO_TICKS(200) )) == ESP_WIFI_STATUS_OK)
          {
            BSP_LED_On(LED_GREEN);
            char *dateStr = NULL;
            char prefix[8], dow[8], month[4];
            int day, year, hour, min, sec;
            RTC_TimeTypeDef sTime;
            
            memset(dow, 0, sizeof(dow));
            memset(month, 0, sizeof(month));
            day = year = hour = min = sec = 0;
            dateStr = strstr(rxBuffer, "Date: ");
            int count = sscanf(dateStr, "%s %s %d %s %d %02d:%02d:%02d ", prefix, dow, &day, month, &year, &hour, &min, &sec); 
            sTime.Hours = hour;
            sTime.Minutes = min;
            sTime.Seconds = sec;
            sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
            sTime.StoreOperation = RTC_STOREOPERATION_RESET;
            k_SetTime(&sTime) ;
            UI_ForceUpdateTime();
          }
        }
      }
    }
  }

  if(xRet != ESP_WIFI_STATUS_OK)
  {
    BSP_LED_On(LED_RED);
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
