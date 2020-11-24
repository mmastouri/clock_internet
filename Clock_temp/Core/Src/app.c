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


typedef struct 
{
  int8_t sign; /* 0 means positive, 1 means negative*/
  uint32_t out_int;
  uint32_t out_dec;
} displayFloatToInt_t;
  
uint32_t enable_time_count = 0;
uint32_t  enable_time_setting = 0;
static float Temperature;
static float Humidity;
displayFloatToInt_t out_value;  

static void floatToInt(float in, displayFloatToInt_t *out_value, int32_t dec_prec);


static void floatToInt(float in, displayFloatToInt_t *out_value, int32_t dec_prec)
{
  if(in >= 0.0f)
  {
    out_value->sign = 0;
  }else
  {
    out_value->sign = 1;
    in = -in;
  }

  in = in + (0.5 / pow(10, dec_prec));
  out_value->out_int = (int32_t)in;
  in = in - (float)(out_value->out_int);
  out_value->out_dec = (int32_t)trunc(in * pow(10, dec_prec));
}

 void App_task (void){
  
  displayFloatToInt_t out_value;    
  static int32_t refresh_bound = -1;  
  
  RTC_TimeTypeDef Time;
  RTC_DateTypeDef Date;  
  
  if((refresh_bound < 0) || (refresh_bound++ > 8))
  {  
    
    Temperature = bsp_get_temp();
    floatToInt(Temperature, &out_value, 1);
    UI_SetTemperature(out_value.out_int,out_value.out_dec);
    
    Humidity = bsp_get_humidity(); 
    floatToInt(Humidity, &out_value, 0);
    UI_SetHumidity(out_value.out_int,out_value.out_dec);
    
    k_GetTime(&Time) ;
    k_GetDate(&Date) ;    
    
    UI_SetTime(Time.Hours, Time.Minutes, Time.Seconds);   
    
    refresh_bound = 0;    
  }
  
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
