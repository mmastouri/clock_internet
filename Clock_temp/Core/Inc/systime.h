/**
******************************************************************************
* @file    stm32_systime.h
* @author  MCD Application Team
* @brief   System time functions implementation
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the 
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_SYS_TIME_H__
#define __STM32_SYS_TIME_H__

#ifdef __cplusplus
extern "C"
{
#endif
  /* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "time.h"
  
  
  /* Exported constants --------------------------------------------------------*/
  
  
  /*!
  * @brief Days, Hours, Minutes and seconds of systime.h
  */
#define TM_DAYS_IN_LEAP_YEAR                        ( ( uint32_t )  366U )
#define TM_DAYS_IN_YEAR                             ( ( uint32_t )  365U )
#define TM_SECONDS_IN_1DAY                          ( ( uint32_t )86400U )
#define TM_SECONDS_IN_1HOUR                         ( ( uint32_t ) 3600U )
#define TM_SECONDS_IN_1MINUTE                       ( ( uint32_t )   60U )
#define TM_MINUTES_IN_1HOUR                         ( ( uint32_t )   60U )
#define TM_HOURS_IN_1DAY                            ( ( uint32_t )   24U )
  
  
  /*!
  * @brief Months of systime.h
  */
#define TM_MONTH_JANUARY                            ( ( uint8_t ) 0U )
#define TM_MONTH_FEBRUARY                           ( ( uint8_t ) 1U )
#define TM_MONTH_MARCH                              ( ( uint8_t ) 2U )
#define TM_MONTH_APRIL                              ( ( uint8_t ) 3U )
#define TM_MONTH_MAY                                ( ( uint8_t ) 4U )
#define TM_MONTH_JUNE                               ( ( uint8_t ) 5U )
#define TM_MONTH_JULY                               ( ( uint8_t ) 6U )
#define TM_MONTH_AUGUST                             ( ( uint8_t ) 7U )
#define TM_MONTH_SEPTEMBER                          ( ( uint8_t ) 8U )
#define TM_MONTH_OCTOBER                            ( ( uint8_t ) 9U )
#define TM_MONTH_NOVEMBER                           ( ( uint8_t )10U )
#define TM_MONTH_DECEMBER                           ( ( uint8_t )11U )
  
  /*!
  * @brief Week days of systime.h
  */
#define TM_WEEKDAY_SUNDAY                           ( ( uint8_t )0U )
#define TM_WEEKDAY_MONDAY                           ( ( uint8_t )1U )
#define TM_WEEKDAY_TUESDAY                          ( ( uint8_t )2U )
#define TM_WEEKDAY_WEDNESDAY                        ( ( uint8_t )3U )
#define TM_WEEKDAY_THURSDAY                         ( ( uint8_t )4U )
#define TM_WEEKDAY_FRIDAY                           ( ( uint8_t )5U )
#define TM_WEEKDAY_SATURDAY                         ( ( uint8_t )6U )
  
  /*!
  * @brief Number of seconds elapsed between Unix and GPS epoch
  */
#define UNIX_GPS_EPOCH_OFFSET                       315964800
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

void SysTimeLocalTime( const uint32_t timestamp, struct tm *localtime );
uint32_t SysTimeMkTime( const struct tm* localtime );

#ifdef __cplusplus
}
#endif

#endif // __STM32_SYS_TIME_H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

