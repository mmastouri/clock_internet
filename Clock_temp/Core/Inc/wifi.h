  /******************************************************************************
  * @file    wifi.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIFI_H__
#define __WIFI_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "systime.h"
#include "ui.h"
#include "rtc.h"   
#include "esp8266_wifi.h"  
   
typedef struct {
  float temperature ;
  float humidity    ;
  char  description[64]  ;
  float feels_like  ;
  float temp_min    ;
  float temp_max    ;
  int   pressure    ;
  int   visibility  ;
  float wind_speed  ;
  float wind_deg    ;
  int   clouds_all  ;
  struct tm   sunrise     ;
  struct tm   sunset      ;
  struct tm   updatetime  ;
}weather_t;

/* Includes ------------------------------------------------------------------*/
ESP_WIFI_Status_t WIFI_Start (ESP_WIFI_Object_t * pxObj);
ESP_WIFI_Status_t WIFI_SyncClock (ESP_WIFI_Object_t * pxObj);
ESP_WIFI_Status_t WIFI_SyncWeatherData (ESP_WIFI_Object_t * pxObj);

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

