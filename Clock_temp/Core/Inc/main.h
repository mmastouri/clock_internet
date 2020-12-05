  /******************************************************************************
  * @file    FreeRTOS/FreeRTOS_DelayUntil/Inc/main.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    18-November-2015 
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "timers.h"
#include "cmsis_os.h"
#include "arm_math.h"
#include "arm_common_tables.h"
#include "bsp.h"   
#include "ui.h"       
#include "wifi.h"    
#include "esp8266_wifi.h"       

typedef struct {
	TaskHandle_t      guiTaskId;
	TaskHandle_t      WIFITaskId;        
	TimerHandle_t     touchPanelTimer;  
        int32_t           touchPanelTimerId;
	TimerHandle_t     WifiTimer;          
	int32_t	          WifiTimerId;
        ESP_WIFI_Object_t EspObj;
} AppGlobals_s;

#define ULONG_MAX 0xFFFFFFFFUL

#define APP_CONNECT_WIFI_BIT 0x01
#define APP_SYNC_WEATHER_BIT 0x02
#define APP_SYNC_TIME_BIT    0x04
        

void WIFI_Task_Notify(uint32_t ulNotificationValue ) ;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

