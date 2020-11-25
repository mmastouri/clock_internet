/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.44                          *
*        Compiled Nov 10 2017, 08:53:57                              *
*        (c) 2017 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "cmsis_compiler.h"
#include <stdio.h>
#include "ui.h"
#include "main.h"
#include "rtc.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW                   (GUI_ID_USER + 0x0C)
#define ID_TIME_HOUR                (GUI_ID_USER + 0x0D)
#define ID_TIME_MIN                 (GUI_ID_USER + 0x2D)
#define ID_DOT                      (GUI_ID_USER + 0x1D)
#define ID_TEMPERATURE              (GUI_ID_USER + 0x0E)
#define ID_HUMIDITY                 (GUI_ID_USER + 0x0F)
#define ID_HEADER_0                 (GUI_ID_USER + 0x10)
#define ID_HEADER_1                 (GUI_ID_USER + 0x11)
#define ID_WIFI                     (GUI_ID_USER + 0x12)

#define TIME_REFRESH_PERIOD        100
#define WIFI_REFRESH_PERIOD        500
#define GUI_REFRESH_PERIOD         1000
#define SETTINGS_COLOR             GUI_ORANGE

#define TIME_HOUR_TIMER_ID         0X01
#define TIME_MIN_TIMER_ID          0X02
#define WIFI_CONNECTING_TIMER_ID   0X03
#define GUI_TIMER_ID               0x04

#define WIFI_CONNECTING           (WM_USER + 0x00)
#define WIFI_CONNECTED            (WM_USER + 0x01)
#define WIFI_DISCONNECTED         (WM_USER + 0x02)

#define TIME_SET                  (WM_USER + 0x10)
#define TIME_ENTER_SETTING_MODE   (WM_USER + 0x11)
#define TEMPERATURE_SET           (WM_USER + 0x20)
#define HUMIDITY_SET              (WM_USER + 0x30)

#define ENV_UPDATE                (WM_USER + 0x40)
#define TIME_UPDATE               (WM_USER + 0x50)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

WM_HWIN hWin;
uint32_t enable_setting = 0;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigital_Font;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigita_Clock;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_wifi;
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
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

/*********************************************************************
*
*       _aDialogCreate
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW, -2, -2, 800, 480, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "00", ID_TIME_HOUR, 30, 100, 770, 220, 0, 0x66, 0 },  
  { TEXT_CreateIndirect, "00", ID_TIME_MIN, 430, 100, 770, 220, 0, 0x66, 0 },   
  { TEXT_CreateIndirect, ":", ID_DOT, 370, 100, 120, 220, 0, 0x66, 0 },    
  { TEXT_CreateIndirect, "00.0 �C", ID_TEMPERATURE, 20, 360, 720, 120, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "00 %", ID_HUMIDITY, 480, 360, 680, 120, 0, 0x65, 0 },  
  { HEADER_CreateIndirect, "Header", ID_HEADER_1, 20, 50, 760, 5, 0, 0x0, 0 }, 
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 20, 300, 760, 5, 0, 0x0, 0 }, 
  { IMAGE_CreateIndirect, "Image", ID_WIFI, 740, 7, 50, 50, 0, 0, 0 },  

};

/*********************************************************************
*
*       _cbDialog
*/

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  static WM_HTIMER   hTimer = 0;  
  static WM_HTIMER   mTimer = 0;    
  static WM_HTIMER   wifiTimer = 0;  
  static WM_HTIMER   GuiRefreshTimer = 0;      
  int Id, Node;
  RTC_TimeTypeDef Time;  
  RTC_DateTypeDef Date; 
  
  char temp[20];
  static uint32_t toggle =0;  
  
  float Temperature;
  float Humidity;
  displayFloatToInt_t out_value;    
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00000000));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x008080FF));
    TEXT_SetFont(hItem, &GUI_FontDigital_Font);
    TEXT_SetText(hItem, "--.- �C");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HUMIDITY);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    TEXT_SetFont(hItem, &GUI_FontDigital_Font);
    TEXT_SetText(hItem, "-- %");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME_HOUR);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFF00));
    TEXT_SetFont(hItem, &GUI_FontDigita_Clock);
    TEXT_SetText(hItem, "00");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME_MIN);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFF00));
    TEXT_SetFont(hItem, &GUI_FontDigita_Clock);
    TEXT_SetText(hItem, "00");      
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DOT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFF00));
    TEXT_SetFont(hItem, &GUI_FontDigita_Clock);
    TEXT_SetText(hItem, ":");       
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
    IMAGE_SetBitmap(hItem, &bmicon_wifi);
    WM_HideWin(hItem);
    
    if(!GuiRefreshTimer) GuiRefreshTimer = WM_CreateTimer(pMsg->hWin, GUI_TIMER_ID, GUI_REFRESH_PERIOD, 0);   
    WM_SendMessageNoPara (pMsg->hWin, TIME_UPDATE);
    WM_SendMessageNoPara (pMsg->hWin, ENV_UPDATE);
    break;
    
  case WIFI_DISCONNECTED:
     if(wifiTimer) WM_DeleteTimer(wifiTimer);     
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
    WM_HideWin(hItem);    
    break;
    
  case WIFI_CONNECTED:
     if(wifiTimer) WM_DeleteTimer(wifiTimer); 
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
    WM_ShowWin(hItem);    
    break;
    
  case WIFI_CONNECTING: 
      if(!wifiTimer) wifiTimer = WM_CreateTimer(pMsg->hWin, WIFI_CONNECTING_TIMER_ID, WIFI_REFRESH_PERIOD, 0);       
    break;
    
  case ENV_UPDATE:
      Temperature = bsp_get_temp();
      floatToInt(Temperature, &out_value, 1);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE);
      snprintf(temp, sizeof(temp), "%d.%d C", out_value.out_int, out_value.out_dec);
      TEXT_SetText(hItem, temp);      
      
      Humidity = bsp_get_humidity(); 
      floatToInt(Humidity, &out_value, 0);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_HUMIDITY);
      snprintf(temp, sizeof(temp), "%d %%", out_value.out_int);
      TEXT_SetText(hItem, temp); 
    break;    
    
  case TIME_UPDATE:
      
      k_GetTime(&Time) ;
      k_GetDate(&Date) ;   
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME_HOUR);    
      snprintf(temp, sizeof(temp), "%02d", Time.Hours);
      
      TEXT_SetText(hItem, temp);      
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME_MIN);
      snprintf(temp, sizeof(temp), "%02d", Time.Minutes);
      
      TEXT_SetText(hItem, temp);  
      
    break;     
    
  case WM_TIMER:  
    Id = WM_GetTimerId(pMsg->Data.v);
    
    if((Id == TIME_HOUR_TIMER_ID) || (Id == TIME_MIN_TIMER_ID))
    {
      WM_RestartTimer(pMsg->Data.v, TIME_REFRESH_PERIOD);
      k_GetTime(&Time) ; 
      k_GetDate(&Date) ;    
      
      
      if(Id == TIME_HOUR_TIMER_ID) //hour
      {        
        Time.Hours++;
        
        if(Time.Hours >= 24)
        {
          Time.Hours = 0;
        }        
        k_SetTime(&Time) ; 
        WM_SendMessageNoPara (pMsg->hWin, TIME_UPDATE);
      }
      else if(Id == TIME_MIN_TIMER_ID) //min
      {
        Time.Minutes++;
        if(Time.Minutes >= 60)
        {
          Time.Minutes = 0;
          Time.Seconds = 0;          
        }
        
        k_SetTime(&Time) ; 
        WM_SendMessageNoPara (pMsg->hWin, TIME_UPDATE);
      } 
    }
    else if (Id == WIFI_CONNECTING_TIMER_ID)
    {
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
      if(WM_IsVisible(hItem))
        WM_HideWin(hItem);  
      else
        WM_ShowWin(hItem);     
      WM_RestartTimer(pMsg->Data.v, WIFI_REFRESH_PERIOD);
    }
    
    else if (Id == GUI_TIMER_ID)
    {            
      WM_SendMessageNoPara (pMsg->hWin, ENV_UPDATE);
      WM_SendMessageNoPara (pMsg->hWin, TIME_UPDATE);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_DOT); 
      toggle = 1- toggle;
      
      if(toggle)
        TEXT_SetText(hItem, ":");       
      else
        TEXT_SetText(hItem, " ");     
      
      WM_RestartTimer(pMsg->Data.v, GUI_REFRESH_PERIOD);
    }
    
    break;  
      
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    Node = pMsg->Data.v;  

    
    if(Node == WM_NOTIFICATION_CLICKED)
    {
      
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      if((Id == ID_TIME_HOUR) && (enable_setting))
      {
        if(!hTimer) hTimer = WM_CreateTimer(pMsg->hWin, TIME_HOUR_TIMER_ID, TIME_REFRESH_PERIOD, 0);         
      }
      if((Id == ID_TIME_MIN) && (enable_setting))
      {
        if(!mTimer) mTimer = WM_CreateTimer(pMsg->hWin, TIME_MIN_TIMER_ID, TIME_REFRESH_PERIOD, 0);           
      }      
    }
    
    if(Node == WM_NOTIFICATION_RELEASED)
    {
      
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      if((Id == ID_TIME_HOUR) && (enable_setting))
      {
        WM_DeleteTimer(hTimer); 
        hTimer = 0;
      }
      if((Id == ID_TIME_MIN) && (enable_setting))
      {
        WM_DeleteTimer(mTimer); 
        mTimer = 0;
      }      
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       ui_set_setting_mode
*/
void ui_set_setting_mode (uint32_t enable)
{
  WM_HWIN hItem;  
  enable_setting = enable;
  
  if(enable)
  {
    hItem = WM_GetDialogItem(hWin, ID_TIME_HOUR);
    TEXT_SetTextColor(hItem, SETTINGS_COLOR);
    hItem = WM_GetDialogItem(hWin, ID_TIME_MIN);
    TEXT_SetTextColor(hItem, SETTINGS_COLOR);
    hItem = WM_GetDialogItem(hWin, ID_DOT);
    TEXT_SetTextColor(hItem, SETTINGS_COLOR);    
  }
  else
  {
    hItem = WM_GetDialogItem(hWin, ID_TIME_HOUR);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFF00));
    hItem = WM_GetDialogItem(hWin, ID_TIME_MIN);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFF00));
    hItem = WM_GetDialogItem(hWin, ID_DOT);
    TEXT_SetTextColor(hItem, 0x00FFFF00);     
  }
}

/*********************************************************************
*
*       UI_SetWifiDisconnected
*/
void UI_SetWifiDisconnected(void)
{  
  WM_SendMessageNoPara (hWin, WIFI_DISCONNECTED);   
}

/*********************************************************************
*
*       UI_SetWifiConnected
*/
void UI_SetWifiConnected(void)
{
  WM_SendMessageNoPara (hWin, WIFI_CONNECTED);     
}
/*********************************************************************
*
*       UI_SetWifiConnecting
*/
void UI_SetWifiConnecting(void)
{
  WM_SendMessageNoPara (hWin, WIFI_CONNECTING); 
}

/*********************************************************************
*
*       UI_SetWifiConnecting
*/
void UI_ForceUpdateTime(void)
{
  WM_SendMessageNoPara (hWin, TIME_UPDATE);
}
/*********************************************************************
*
*       CreateWindow
*/

WM_HWIN CreateWindow(void) {


  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
/*************************** End of file ****************************/
