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
#include "rtc.h"

static void GUI_Task(void const *argument);
static void WIFI_Task(void const *argument);
static void TouchPanel_TimerCallback(TimerHandle_t pxTimer);
static void Wifi_TimerCallback(TimerHandle_t pxTimer);
static void SystemClock_Config(void);
AppGlobals_s appGlobals;

/**
  * @brief  Main program
  * @param  None
  * @retval None
    */
int main(void)
{

  HAL_Init();
  SystemClock_Config();
  k_BspInit(); 
  k_rtc_init();  
  
  appGlobals.touchPanelTimerId = 1;
  appGlobals.WifiTimerId = 2;
  
  xTaskCreate((TaskFunction_t)GUI_Task, "GUI_Task", 4096, NULL, 1, &appGlobals.guiTaskId);
  xTaskCreate((TaskFunction_t)WIFI_Task, "WIFI_Task", 1024, NULL, 1, &appGlobals.WIFITaskId);  
  
  appGlobals.touchPanelTimer = xTimerCreate ("Touch Screen", pdMS_TO_TICKS(20), pdTRUE, &appGlobals.touchPanelTimerId, TouchPanel_TimerCallback );
  appGlobals.WifiTimer = xTimerCreate ("WifiBackgroundSynck", 15 * 60 * pdMS_TO_TICKS(1000), pdTRUE, &appGlobals.WifiTimerId, Wifi_TimerCallback );  
  
  vTaskStartScheduler();
  
  for(;;);
}

/**
  * @brief  GUI Task
  * @param  GUI Task args
  * @retval None
  */
void GUI_Task(void const *arg) {
  
  UI_Init();
  xTimerStart(appGlobals.touchPanelTimer, 0);
  
  for (;;) {
    GUI_Delay(10);
  }
}

/**
  * @brief  WIFI Task
  * @param  WIFI Task args
  * @retval None
  */
void WIFI_Task(void const *arg) {
  
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
  BaseType_t xResult;
  uint32_t   ulNotifiedValue;
  
  if(WIFI_Init(&appGlobals.EspObj) == ESP_WIFI_STATUS_OK)
  {
    if(WIFI_Connect(&appGlobals.EspObj) == ESP_WIFI_STATUS_OK)
    {
      WIFI_SyncClock (&appGlobals.EspObj);
      WIFI_SyncWeatherData (&appGlobals.EspObj);
      xTimerStart(appGlobals.WifiTimer, 0);
    }
  }
  for( ;; )
  {
    /* Wait to be notified of an interrupt. */
    xResult = xTaskNotifyWait( pdFALSE,    /* Don't clear bits on entry. */
                              ULONG_MAX,        /* Clear all bits on exit. */
                              &ulNotifiedValue, /* Stores the notified value. */
                              xMaxBlockTime );
    
    if( xResult == pdPASS )
    {
      /* A notification was received.  See which bits were set. */
      if( ( ulNotifiedValue & APP_CONNECT_WIFI_BIT ) != 0 )
      {
        ESP_WIFI_Disconnect (&appGlobals.EspObj);
        WIFI_Connect(&appGlobals.EspObj);
      }
      
      if( ( ulNotifiedValue & APP_SYNC_WEATHER_BIT ) != 0 )
      {
        WIFI_SyncWeatherData (&appGlobals.EspObj);
      }
      
      if( ( ulNotifiedValue & APP_SYNC_TIME_BIT ) != 0 )
      {
        WIFI_SyncClock (&appGlobals.EspObj);
      }
    }
  }
}

/**
  * @brief  WIFI Task
  * @param  WIFI Task args
  * @retval None
  */
void WIFI_Task_Notify(uint32_t ulNotificationValue ) 
{
  xTaskNotify( appGlobals.WIFITaskId,
              ulNotificationValue,
              eSetBits);
}

/**
  * @brief  vApplicationTickHook
  * @param  None
  * @retval None
  */
void vApplicationTickHook( void )
{
    HAL_IncTick();
}

/**
  * @brief  TouchPanel_TimerCallback
  * @param  TimerID
  * @retval None
  */
static void TouchPanel_TimerCallback(TimerHandle_t pxTimer) {
  k_TouchUpdate();
}

/**
  * @brief  Wifi_TimerCallback
  * @param  Timer ID
  * @retval None
  */
void Wifi_TimerCallback(TimerHandle_t pxTimer) {
  WIFI_SyncWeatherData (&appGlobals.EspObj);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 7;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
