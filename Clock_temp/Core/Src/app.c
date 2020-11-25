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
#define TIME_SOURCE_HTTP_HOST   "www.st.com"
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
uint8_t  IPAddr[4];

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

extern AppGlobals_s appGlobals;
void wifi_task (void){
  
  if(ESP_WIFI_GetHostIP( &appGlobals.EspObj,TIME_SOURCE_HTTP_HOST , IPAddr ) == ESP_WIFI_STATUS_OK)
  {
    BSP_LED_On(LED_GREEN);
  }
  
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
