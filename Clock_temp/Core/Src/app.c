/**
  ******************************************************************************
  * @file    app
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

static uint32_t enable_time_count = 0;
static uint32_t enable_time_setting = 0;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
