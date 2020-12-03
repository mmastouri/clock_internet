/**
  ******************************************************************************
  * @file    wifi.c
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
#include "wifi.h"


/* Private typedef -----------------------------------------------------------*/
#define TIME_SOURCE_HTTP_HOST   "www.google.com"
#define TIME_SOURCE_HTTP_PORT   80
#define TIME_SOURCE_HTTP_PROTO  ESP_WIFI_TCP

#define WEATHER_SOURCE_HTTP_HOST    "api.openweathermap.org"
#define WEATHER_SOURCE_HTTP_PORT    80
#define WEATHER_SOURCE_HTTP_PROTO   ESP_WIFI_TCP

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

 weather_t weather = 
 {
  .temperature = 25,
  .humidity    = 50,
  .description ="Cloudy",   
  .desc_idx    = 0, 
  .feels_like  = 25,
  .temp_min    = 10,
  .temp_max    = 50,
  .pressure    = 1023,
  .visibility  = 16093,
  .wind_speed  =  1.5,
  .wind_deg    = 350,
  .clouds_all  = 1,
  .sunrise     = 1560343627,
  .sunset      = 1560396563,
  .updatetime  = 0,  
};

static void Add1hour(uint8_t *hour,  uint8_t *day, uint8_t *month, uint8_t *year)
{
  uint32_t iLeap;
  const uint8_t daysofmonth[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    
  if (((*year % 4) == 0) && (((*year % 100) != 0) || ((*year % 400) == 0)))
  {
    iLeap = 1;
  }
  
  
  if((*month) > 10)  *month-= 6;
  
  *hour += 1;
  if(*hour == 24)
  {
     *hour = 0;
     (*day)++;
     if(*day > daysofmonth[iLeap][*month])
     {       
       *day = 0;
        (*month )++;
        if(*month > 12)
        {
          *month = 0;
          (*year)++;
        }
     }
  }
  if((*month) > 10)  *month+= 6;
  
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
        else
        {
          xRet = ESP_WIFI_STATUS_ERROR;
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
    
    char dow[8], month[4];
    int day, year, hour, min, sec;
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    
    int count = sscanf(dateStr, "Date: %s %d %s %d %02d:%02d:%02d ", dow, &day, month, &year, &hour, &min, &sec); 
    
    sTime.Hours   = hour;
    sTime.Minutes = min;
    sTime.Seconds = sec;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    
    if (strcmp(dow, "Mon,") == 0) { sDate.WeekDay = RTC_WEEKDAY_MONDAY; } else
      if (strcmp(dow, "Tue,") == 0) { sDate.WeekDay = RTC_WEEKDAY_TUESDAY; } else
        if (strcmp(dow, "Wed,") == 0) { sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY; } else
          if (strcmp(dow, "Thu,") == 0) { sDate.WeekDay = RTC_WEEKDAY_THURSDAY; } else
            if (strcmp(dow, "Fri,") == 0) { sDate.WeekDay = RTC_WEEKDAY_FRIDAY; } else
              if (strcmp(dow, "Sat,") == 0) { sDate.WeekDay = RTC_WEEKDAY_SATURDAY; } else
                if (strcmp(dow, "Sun,") == 0) { sDate.WeekDay = RTC_WEEKDAY_SUNDAY; } 
    
    if (strcmp(month, "Jan") == 0) { sDate.Month = RTC_MONTH_JANUARY; } else
      if (strcmp(month, "Feb") == 0) { sDate.Month = RTC_MONTH_FEBRUARY; } else
        if (strcmp(month, "Mar") == 0) { sDate.Month = RTC_MONTH_MARCH; } else
          if (strcmp(month, "Apr") == 0) { sDate.Month = RTC_MONTH_APRIL; } else
            if (strcmp(month, "May") == 0) { sDate.Month = RTC_MONTH_MAY; } else
              if (strcmp(month, "Jun") == 0) { sDate.Month = RTC_MONTH_JUNE; } else
                if (strcmp(month, "Jul") == 0) { sDate.Month = RTC_MONTH_JULY; } else
                  if (strcmp(month, "Aug") == 0) { sDate.Month = RTC_MONTH_AUGUST; } else
                    if (strcmp(month, "Sep") == 0) { sDate.Month = RTC_MONTH_SEPTEMBER; } else
                      if (strcmp(month, "Oct") == 0) { sDate.Month = RTC_MONTH_OCTOBER; } else
                        if (strcmp(month, "Nov") == 0) { sDate.Month = RTC_MONTH_NOVEMBER; } else
                          if (strcmp(month, "Dec") == 0) { sDate.Month = RTC_MONTH_DECEMBER; } 
        
    sDate.Date = day;
    sDate.Year = year - 2000;
    
    Add1hour(&sTime.Hours, &sDate.Date,  &sDate.Month, &sDate.Year);
    
    k_SetTime(&sTime) ;
    k_SetDate(&sDate) ;
    UI_ForceUpdateTime();
  }
  return xRet;
}


/*********************************************************************
*
*       WIFI_SyncWeatherData
*/
ESP_WIFI_Status_t WIFI_SyncWeatherData (ESP_WIFI_Object_t * pxObj){
  ESP_WIFI_Status_t xRet;
  char *weatherStr = NULL;
  uint32_t timestamp,sr, ss,cc;
  
  if((xRet = WIFI_SyncData (pxObj, WEATHER_SOURCE_HTTP_HOST, weather_request, sizeof(weather_request),
                            WEATHER_SOURCE_HTTP_PORT, WEATHER_SOURCE_HTTP_PROTO, "\"weather\":", &weatherStr)) == ESP_WIFI_STATUS_OK)
  {  
    
    weatherStr = strstr(weatherStr, "\"main\":\"");
    
    weatherStr+= 8;
    int count = 0;
    do
    {
      weather.description[count] = weatherStr[count];
      count++;
    }
    while((weatherStr[count] != '"') && (count < 16));
    
    weather.description[count] = 0;
    
    if(strcmp( weather.description, "Thunderstorm\"") == 0) weather.desc_idx = 0;
    else if(strcmp( weather.description, "Drizzle") == 0) weather.desc_idx = 1;
    else if(strcmp( weather.description, "Rain") == 0)weather.desc_idx = 2;
    else if(strcmp(weather.description, "Snow") == 0) weather.desc_idx = 3;
    else if(strcmp( weather.description, "Clear") == 0) weather.desc_idx = 4;
    else if(strcmp(weather.description, "Clouds") == 0) weather.desc_idx = 5;
    else
    {
      strcpy(weather.description, "Error");  
      weather.desc_idx = 0;
      return ESP_WIFI_STATUS_ERROR;
    }
    
    weatherStr = strstr(weatherStr, "\"temp\":");
    sscanf(weatherStr, "\"temp\":%f", &weather.temperature); 
    weather.temperature -= 273.15;
    
    weatherStr = strstr(rxBuffer,  "\"feels_like\":");
    sscanf(weatherStr, "\"feels_like\":%f", &weather.feels_like);
    weather.feels_like -= 273.15;
    
    weatherStr = strstr(rxBuffer,  "\"temp_min\":");
    sscanf(weatherStr, "\"temp_min\":%f", &weather.temp_min);     
    weather.temp_min -= 273.15;
    
    weatherStr = strstr(rxBuffer,  "\"temp_max\":");
    sscanf(weatherStr, "\"temp_max\":%f", &weather.temp_max);   
    weather.temp_max -= 273.15;
    
    weatherStr = strstr(rxBuffer,  "\"pressure\":");
    sscanf(weatherStr, "\"pressure\":%d", &weather.pressure);
    
    weatherStr = strstr(rxBuffer,  "\"humidity\":");
    sscanf(weatherStr, "\"humidity\":%f", &weather.humidity);
    
    weatherStr = strstr(rxBuffer,  "\"visibility\":");
    sscanf(weatherStr, "\"visibility\":%d", &weather.visibility);  
    weather.visibility = (weather.visibility / 1000);
    
    weatherStr = strstr(rxBuffer,  "\"speed\":");
    sscanf(weatherStr, "\"speed\":%f", &weather.wind_speed);     
    weather.wind_speed = (weather.wind_speed * 3600) / 1000;
    
    weatherStr = strstr(rxBuffer,  "\"deg\":");
    sscanf(weatherStr, "\"deg\":%f", &weather.wind_deg);
    weather.wind_deg -= 273.15;    
    
    weatherStr = strstr(rxBuffer,  "\"all\":");
    sscanf(weatherStr, "\"all\":%d", &weather.clouds_all);     
    
    weatherStr = strstr(rxBuffer,  "\"dt\":");
    sscanf(weatherStr, "\"dt\":%d", &timestamp); 
    SysTimeLocalTime(timestamp , &weather.updatetime);
    cc = timestamp;
    
    weatherStr = strstr(rxBuffer,  "\"sunrise\":");
    sscanf(weatherStr, "\"sunrise\":%d", &timestamp);     
    SysTimeLocalTime(timestamp , &weather.sunrise);
    sr = timestamp;    
    
    weatherStr = strstr(rxBuffer,  "\"sunset\":");
    sscanf(weatherStr, "\"sunset\":%d", &timestamp);     
    SysTimeLocalTime(timestamp , &weather.sunset);
    ss = timestamp;      
    
    if((cc < sr) || (cc > ss)) //night
      weather.desc_idx += 6;
    
    UI_ForceUpdateWhether();
  }
  return xRet;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
