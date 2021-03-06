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

#include "ui.h"
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
#define ID_HEADER_0                 (GUI_ID_USER + 0x10)
#define ID_HEADER_1                 (GUI_ID_USER + 0x11)
#define ID_WIFI                     (GUI_ID_USER + 0x12)
#define ID_INTERNET                 (GUI_ID_USER + 0x13)
#define ID_IN_OUTDOUR               (GUI_ID_USER + 0x14)
#define ID_MENU                     (GUI_ID_USER + 0x15)
#define ID_PROFILE_INTERNET         (GUI_ID_USER + 0x16)
#define ID_RADIO_0                  (GUI_ID_USER + 0x22)
#define ID_RADIO_1                  (GUI_ID_USER + 0x23)
#define ID_DAYWEEK                  (GUI_ID_USER + 0x17)
#define ID_DAY                      (GUI_ID_USER + 0x18)
#define ID_MONTH                    (GUI_ID_USER + 0x19)
#define ID_WEEK                     (GUI_ID_USER + 0x1A)


#define ID_MENU_WINDOW              (GUI_ID_USER + 0x30)
#define ID_MENU_TITLE               (GUI_ID_USER + 0x31)
#define ID_MENU_HEADER              (GUI_ID_USER + 0x32)

#define ID_WEATHER_ICON             (GUI_ID_USER + 0x50)
#define ID_TEMPERATURE_ICON         (GUI_ID_USER + 0x51)
#define ID_PRESSURE_ICON            (GUI_ID_USER + 0x52)
#define ID_HUMIDITY_ICON            (GUI_ID_USER + 0x53)
#define ID_WIND_ICON                (GUI_ID_USER + 0x54)

#define ID_CONDITION_TXT            (GUI_ID_USER + 0x70)
#define ID_TEMPERATURE_TXT          (GUI_ID_USER + 0x71)
#define ID_PRESSURE_TXT             (GUI_ID_USER + 0x72)
#define ID_HUMIDITY_TXT             (GUI_ID_USER + 0x73)
#define ID_WIND_TXT                 (GUI_ID_USER + 0x74)

#define ID_PROFILE_ICON             (GUI_ID_USER + 0x80)
#define ID_REFRESH_ICON             (GUI_ID_USER + 0x81)
#define ID_PROFILE_TXT              (GUI_ID_USER + 0x82)
#define ID_REFRESH_TXT              (GUI_ID_USER + 0x83)
#define ID_CLOCK_ICON               (GUI_ID_USER + 0x84)
#define ID_LOCATION_ICON            (GUI_ID_USER + 0x85)
#define ID_LOCATION_TXT             (GUI_ID_USER + 0x86)
#define ID_CLOCK_TXT                (GUI_ID_USER + 0x87)



#define TIME_SETTING_REFRESH_PERIOD 100
#define WIFI_REFRESH_PERIOD         500
#define TIME_REFRESH_PERIOD         500
#define CALENDAR_REFRESH_PERIOD     1000
#define ENV_REFRESH_PERIOD          5000
#define SETTINGS_COLOR              GUI_ORANGE

#define TIME_HOUR_TIMER_ID         0X01
#define TIME_MIN_TIMER_ID          0X02
#define WIFI_CONNECTING_TIMER_ID   0X03
#define GUI_TIME_REFRESH_ID        0x04
#define GUI_ENV_REFRESH_ID         0x05
#define GUI_CALENDAR_REFRESH_ID    0x06

#define WIFI_CONNECTING           (WM_USER + 0x00)
#define WIFI_CONNECTED            (WM_USER + 0x01)
#define WIFI_DISCONNECTED         (WM_USER + 0x02)
#define INTERNET_AVAILABLE        (WM_USER + 0x03)

#define TIME_SET                  (WM_USER + 0x10)
#define TIME_ENTER_SETTING_MODE   (WM_USER + 0x11)
#define TEMPERATURE_SET           (WM_USER + 0x20)
#define HUMIDITY_SET              (WM_USER + 0x30) 

#define TEMPERATURE_UPDATE        (WM_USER + 0x40)
#define TIME_UPDATE               (WM_USER + 0x50)
#define CALENDAR_UPDATE           (WM_USER + 0x60)
#define WEATHER_UPDATE            (WM_USER + 0x70)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
typedef struct PARA PARA;
typedef struct WINDOW_DATA WINDOW_DATA;

struct PARA {
  int           xPos;
  int           xSizeScreen;
  int           xSize;
  int           OldPos;
  WINDOW_DATA * pData;
  int           Index;
};

struct WINDOW_DATA {
  int             xSize;
  int             ySize;
  int             xPos;
  int             yOff;
  U8              Alpha;
  WM_HTIMER       hTimer;
  WM_HWIN         hWin;
  GUI_ANIM_HANDLE hAnimSymbol;
  GUI_ANIM_HANDLE hAnimBackground;
  PARA            aPara[5];
  GUI_TIMER_TIME  TimeLastTouch;
  int             Job;
  int             LastJob;
  int             IndexCity;
  int             IndexAnimIn;
  int             IndexAnimOut;
  int             HasStopped;
  int             Diff;
};

//WINDOW_DATA * pData;
//static WINDOW_DATA Data;

static RTC_TimeTypeDef Time;  
static RTC_DateTypeDef Date; 
static WM_HWIN hMainFrame, hHomeFrame, hWeatherFrame, hBottomFrame, hSettingsFrame;
static uint32_t ui_enable_time_setting = 0;
const char *dayofweek[] = {"Mon.", "Tue.", "Wed.", "Thu.", "Fri.", "Sat.", "Sun."}; 
const char *profile[] = {"[Home]","[Work]"};    
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigital_Font;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigita_Clock;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigitGraphics60;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigitGraphics40;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDigitGraphics20;

extern GUI_CONST_STORAGE GUI_BITMAP bmicon_wifi;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_indoor;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_internet;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_menu;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_outdoor;
extern GUI_CONST_STORAGE GUI_BITMAP bmbackground;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_home;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_home_act;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_menu;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_menu_act;


extern GUI_CONST_STORAGE GUI_BITMAP bmicon_weather_day_clouds;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_night_clouds;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_day_Thunderstorms;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_day_clear;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_night_clear;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_day_dizzle;

extern GUI_CONST_STORAGE GUI_BITMAP bmweather_temp;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_humidity;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_pressure;
extern GUI_CONST_STORAGE GUI_BITMAP bmweather_wind;

extern GUI_CONST_STORAGE GUI_BITMAP bmprofile_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmrefresh_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmlocation_icon;


extern  weather_t weather ;
extern  uint8_t wifi_profile;
extern const ESP_AvHotspot_t Hotspot[];
extern AppGlobals_s appGlobals;
GUI_CONST_STORAGE GUI_BITMAP  *weather_condition_icon[] = 
{
&bmweather_day_Thunderstorms,  //0  Day-Thunderstorm
&bmweather_day_dizzle,         //1  Day-Drizzle
&bmweather_day_dizzle,         //2  Day-Rain
&bmweather_day_dizzle,         //3  Day-Snow
&bmweather_day_clear,          //4  Day-Clear
&bmicon_weather_day_clouds,    //5  Day-Clouds
&bmweather_day_Thunderstorms,  //6  Night-Thunderstorm
&bmweather_day_dizzle,         //7  Night-Drizzle
&bmweather_day_dizzle,         //8  Night-Rain
&bmweather_day_dizzle,         //9  Night-Snow
&bmweather_night_clear,        //10 Night-Clear 
&bmweather_night_clouds,       //11 Night-Clouds  
};
///*********************************************************************
//*
//*       _InitData
//*/
//static void _InitData(WINDOW_DATA * pData) {
//  int i, NumItems, xSizeScreen, xSize;
//
//  NumItems = GUI_COUNTOF(pData->aPara);
//  xSizeScreen = LCD_GetXSize();
//  xSize = xSizeScreen / NumItems;
//  for (i = 0; i < NumItems; i++) {
//    pData->aPara[i].xSizeScreen = xSizeScreen;
//    pData->aPara[i].xSize       = xSize;
//    pData->aPara[i].xPos        = (xSize * i) + xSize / 2;
//    pData->aPara[i].pData       = pData;
//    pData->aPara[i].Index       = i;
//  }
//}



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
*       rtcCalcYearWeek
*/
int rtcCalcYearWeek(int iYear, int iMonth, int iDay, int iWeekDay)
{
    int iLeap = 0;
    static const int ppiYearDays[2][13] =
    {
         /* Normal year */
         {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
         /* Leap year */
         {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
    };
    /* Check for leap year */
    if (((iYear % 4) == 0) && (((iYear % 100) != 0) || ((iYear % 400) == 0)))
    {
        iLeap = 1;
    }
    /* Calculate the year week */
    return (((ppiYearDays[iLeap][iMonth] + iDay) - (iWeekDay + 7) % 7 + 7) / 7) + 1;
}
/*********************************************************************
*
*       _aMainDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aMainDialogCreate[] = {  
  { WINDOW_CreateIndirect, "Window", ID_WINDOW, 0, 0, 800, 480, 0, 0, 0 }, 
  { IMAGE_CreateIndirect, "Image", ID_WIFI, 730, 7, 50, 50, 0, 0, 0 }, 
  { IMAGE_CreateIndirect, "Image", ID_INTERNET, 680, 7, 50, 50, 0, 0, 0 },     
  { IMAGE_CreateIndirect, "Image", ID_MENU, 30, 7, 250, 250, 0, 0, 0 }, 
  { TEXT_CreateIndirect, "profile", ID_PROFILE_INTERNET, 300, 13, 200, 220, TEXT_CF_HCENTER, 0, 0 },  
  
  { TEXT_CreateIndirect, "00", ID_TIME_HOUR, 10, 100, 360, 220, TEXT_CF_HCENTER, 0, 0 },  
  { TEXT_CreateIndirect, "00", ID_TIME_MIN, 430, 100, 360, 220, TEXT_CF_HCENTER, 0, 0 },   
  { TEXT_CreateIndirect, ":", ID_DOT, 370, 100, 120, 220, 0, 0x66, 0 },    
  { HEADER_CreateIndirect, "Header", ID_HEADER_1, 20, 50, 760, 5, 0, 0x0, 0 }, 
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 20, 290, 760, 5, 0, 0x0, 0 }, 
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 380, 465, 80, 16, 0, 0x1402, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_1, 402, 465, 80, 16, 0, 0x1402, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aHomeDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW, 0, 0, 800, 160,  WM_CF_SHOW, 0x0, sizeof(WINDOW_DATA *)},
  { TEXT_CreateIndirect, "00.0 C", ID_TEMPERATURE, 20, 60, 720, 120, 0, 0, 0 },    
  { TEXT_CreateIndirect, "DayofWeek", ID_DAYWEEK, 460, 40, 680, 120, 0, 0, 0 }, 
  { TEXT_CreateIndirect, "Week", ID_WEEK, 460, 110, 680, 120, 0, 0, 0 }, 
  { TEXT_CreateIndirect, "Day", ID_DAY, 660, 40, 120, 120, TEXT_CF_HCENTER, 0, 0 },   
  { TEXT_CreateIndirect, "Month", ID_MONTH, 660, 110, 120, 120, TEXT_CF_HCENTER, 0, 0 },     
  { IMAGE_CreateIndirect, "Image", ID_IN_OUTDOUR, 30, 7, 50, 50, 0, 0, 0 },    
};


static const GUI_WIDGET_CREATE_INFO _aWeatherDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW, 0, 0, 800, 160, WM_CF_SHOW, 0x0, sizeof(WINDOW_DATA *)},
  { IMAGE_CreateIndirect, "condition", ID_WEATHER_ICON, 65, 20, 150, 150, 0, 0, 0 }, 
  
  { IMAGE_CreateIndirect, "Temperature", ID_TEMPERATURE_ICON, 220, 0, 150, 150, 0, 0, 0 }, 
  { IMAGE_CreateIndirect, "Humidity", ID_HUMIDITY_ICON, 244, 100, 150, 150, 0, 0, 0 }, 
  
  { IMAGE_CreateIndirect, "Pressure", ID_PRESSURE_ICON, 480, 20, 150, 150, 0, 0, 0 },   
  { IMAGE_CreateIndirect, "Wind", ID_WIND_ICON, 480, 100, 150, 150, 0, 0, 0 },   
  
  { TEXT_CreateIndirect, "condition", ID_CONDITION_TXT, -15, 130, 260, 120, TEXT_CF_HCENTER, 0, 0 },
  
  { TEXT_CreateIndirect, "Temperatute", ID_TEMPERATURE_TXT, 310, 30, 260, 120, TEXT_CF_LEFT, 0, 0 }, 
  { TEXT_CreateIndirect, "Humidity", ID_HUMIDITY_TXT, 310, 108, 260, 120, TEXT_CF_LEFT, 0, 0 },  
  
  { TEXT_CreateIndirect, "Pressure", ID_PRESSURE_TXT, 550, 30, 260, 120, TEXT_CF_LEFT, 0, 0 }, 
  { TEXT_CreateIndirect, "Wind", ID_WIND_TXT, 550, 108, 260, 120, TEXT_CF_LEFT, 0, 0 },     
};

static const GUI_WIDGET_CREATE_INFO _aSettingsDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW, 0, 0, 800, 180, WM_CF_SHOW, 0x0, sizeof(WINDOW_DATA *)},
  { IMAGE_CreateIndirect, "Profile", ID_PROFILE_ICON, 65, 10, 150, 150, 0, 0, 0 }, 
  { TEXT_CreateIndirect, "[Profile]", ID_PROFILE_TXT, -5, 150, 260, 120, TEXT_CF_HCENTER, 0, 0 },
  
  
  { IMAGE_CreateIndirect, "Refresh", ID_REFRESH_ICON, 245, 10, 150, 150, 0, 0, 0 }, 
  { TEXT_CreateIndirect, "Refresh", ID_REFRESH_TXT, 170, 150, 260, 120, TEXT_CF_HCENTER, 0, 0 },
  
  { IMAGE_CreateIndirect, "Clock", ID_CLOCK_ICON, 425, 0, 150, 150, 0, 0, 0 },
  { TEXT_CreateIndirect, "Settings", ID_CLOCK_TXT, 370, 150, 260, 120, TEXT_CF_HCENTER, 0, 0 },
  
  { IMAGE_CreateIndirect, "Location", ID_LOCATION_ICON, 615, 5, 150, 150, 0, 0, 0 }, 
  { TEXT_CreateIndirect, "Tunis", ID_LOCATION_TXT, 540, 150, 260, 120, TEXT_CF_HCENTER, 0, 0 },  
  
   
};

/*********************************************************************
*
*       _cbSettingsDialog
*/
static void _cbSettingsDialog(WM_MESSAGE * pMsg) {
  WM_HWIN     hItem;  
  int         Id, Node;
  switch (pMsg->MsgId) {
        
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;

    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0xFF000000));
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_ICON);
    IMAGE_SetBitmap(hItem, &bmprofile_icon);  
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_REFRESH_ICON);
    IMAGE_SetBitmap(hItem, &bmrefresh_icon);    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_ICON);
    IMAGE_SetBitmap(hItem, &bmclock_icon);  
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LOCATION_ICON);
    IMAGE_SetBitmap(hItem, &bmlocation_icon);    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics20);
    TEXT_SetText(hItem, profile[wifi_profile]); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_REFRESH_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics20);
    TEXT_SetText(hItem, "Sync Data"); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics20);
    TEXT_SetText(hItem, "Set Time"); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_LOCATION_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics20);
    TEXT_SetText(hItem, "[Tunis]");    
 
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    Node = pMsg->Data.v;      
    if(Node == WM_NOTIFICATION_RELEASED)
    {
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */

      
      if(Id == ID_PROFILE_ICON)
      {  
        hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_ICON);
      }  
      
      if(Id == ID_CLOCK_ICON)
      { 
        hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_ICON);
      }
      
      if(Id == ID_REFRESH_ICON)
      { 
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REFRESH_ICON);
      }
      if(Id == ID_LOCATION_ICON)
      { 
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LOCATION_ICON);
      }      
     
      if(Id == ID_PROFILE_ICON)
      {  
        wifi_profile = (wifi_profile + 1)%2;        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_TXT);
        TEXT_SetText(hItem, profile[wifi_profile]);  
        k_BkupSaveParameter(RTC_BKP_DR1, wifi_profile);
        WIFI_Task_Notify(APP_CONNECT_WIFI_BIT) ;   
      }  
      
      if(Id == ID_CLOCK_ICON)
      { 
        ui_set_setting_mode();
      }
      
      if(Id == ID_REFRESH_ICON)
      { 
        WIFI_Task_Notify(APP_SYNC_WEATHER_BIT | APP_SYNC_TIME_BIT ) ;  
      }

    }
    
   if(Node == WM_NOTIFICATION_CLICKED)
    {
      if(Id == ID_LOCATION_ICON)
      { 
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LOCATION_ICON);
      }  
      
      if(Id == ID_PROFILE_ICON)
      {  
        hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_ICON);
      }  
      
      if(Id == ID_CLOCK_ICON)
      { 
        hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_ICON);
      }
      
      if(Id == ID_REFRESH_ICON)
      { 
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REFRESH_ICON);
      }
      GUI_SetBkColor(GUI_TRANSPARENT);
      WM_SelectWindow (hItem);
      GUI_Clear();
      WM_Paint(pMsg->hWin);
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbHomeDialog
*/
static void _cbWeatherDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;  
 
  displayFloatToInt_t out_value;      
  char temp[64];
    
  switch (pMsg->MsgId) {
        
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;

    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x80000000));
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CONDITION_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics20);
    TEXT_SetText(hItem, "Few Clouds"); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x008080FF));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics40);
    TEXT_SetText(hItem, "25 C"); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_HUMIDITY_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics40);
    TEXT_SetText(hItem, "50 %"); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_PRESSURE_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(GUI_YELLOW));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics40);
    TEXT_SetText(hItem, "1000 Pa"); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIND_TXT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics40);
    TEXT_SetText(hItem, "10 Km/h");     
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WEATHER_ICON);
    IMAGE_SetBitmap(hItem, weather_condition_icon[0]);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE_ICON);
    IMAGE_SetBitmap(hItem, &bmweather_temp);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_PRESSURE_ICON);
    IMAGE_SetBitmap(hItem, &bmweather_pressure);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_HUMIDITY_ICON);
    IMAGE_SetBitmap(hItem, &bmweather_humidity);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIND_ICON);
    IMAGE_SetBitmap(hItem, &bmweather_wind);    
    break;
      
  case WEATHER_UPDATE:
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WEATHER_ICON);
    IMAGE_SetBitmap(hItem, weather_condition_icon[weather.desc_idx]);  
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CONDITION_TXT);
    TEXT_SetText(hItem, weather.description);    
    
    floatToInt(weather.temperature, &out_value, 1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE_TXT);
    snprintf(temp, sizeof(temp), "%02d C", out_value.out_int);
    TEXT_SetText(hItem, temp); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_HUMIDITY_TXT);
    floatToInt(weather.humidity, &out_value, 1);
    snprintf(temp, sizeof(temp), "%d %%", out_value.out_int);    
    TEXT_SetText(hItem, temp); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_PRESSURE_TXT);
    snprintf(temp, sizeof(temp), "%d Pa", weather.pressure);
    TEXT_SetText(hItem, temp); 

    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIND_TXT);
    floatToInt(weather.wind_speed, &out_value, 1);    
    snprintf(temp, sizeof(temp), "%d km/h", out_value.out_int);
    TEXT_SetText(hItem, temp);      
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbHomeDialog
*/
static void _cbHomeDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem; 
  static WM_HTIMER   EnvRefreshTimer = 0;   
  static WM_HTIMER   CalendarRefreshTimer = 0;     
  
  int Id, Node, woy;
  
  char temp[20];
  static uint8_t place_toggle = 0;
  static float Temperature = 25;
  displayFloatToInt_t out_value;    
  
  switch (pMsg->MsgId) {
        
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;

    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0xFF000000));
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x008080FF));
    TEXT_SetFont(hItem, &GUI_FontDigital_Font);
    TEXT_SetText(hItem, "--.- �C"); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DAYWEEK);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics60);
    TEXT_SetText(hItem, "Sun."); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WEEK);
    TEXT_SetTextColor(hItem, GUI_YELLOW);
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics60);
    TEXT_SetText(hItem, "W48");    

    hItem = WM_GetDialogItem(pMsg->hWin, ID_DAY);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics60);
    TEXT_SetText(hItem, "29");     
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MONTH);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics60);
    TEXT_SetText(hItem, "11");      
     
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IN_OUTDOUR);
    IMAGE_SetBitmap(hItem, &bmicon_indoor);
    
    if(!EnvRefreshTimer) EnvRefreshTimer = WM_CreateTimer(pMsg->hWin, GUI_ENV_REFRESH_ID, ENV_REFRESH_PERIOD, 0);     
    WM_SendMessageNoPara (pMsg->hWin, TEMPERATURE_UPDATE);
    
    if(!CalendarRefreshTimer) CalendarRefreshTimer = WM_CreateTimer(pMsg->hWin, GUI_CALENDAR_REFRESH_ID, CALENDAR_REFRESH_PERIOD, 0);     
    WM_SendMessageNoPara (pMsg->hWin, CALENDAR_UPDATE);
    
    break;
 
  case TEMPERATURE_UPDATE:

    if(place_toggle == 0) //Indoor
    {
      Temperature = bsp_get_temp();      
    }
    else
    {
      Temperature = weather.temperature;      
    }
    
    floatToInt(Temperature, &out_value, 1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEMPERATURE);
    snprintf(temp, sizeof(temp), "%d.%dC", out_value.out_int, out_value.out_dec);
    TEXT_SetText(hItem, temp);      
    
    break;
    
  case CALENDAR_UPDATE:
      
      k_GetTime(&Time) ;
      k_GetDate(&Date) ;   

      hItem = WM_GetDialogItem(pMsg->hWin, ID_DAYWEEK);
      snprintf(temp, 5, "%s", dayofweek[Date.WeekDay > 0 ? (Date.WeekDay) - 1 : 0]);            
      TEXT_SetText(hItem, temp); 
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_DAY);
      snprintf(temp, 5, "%02d", Date.Date);            
      TEXT_SetText(hItem, temp);  
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_MONTH);
      snprintf(temp, 5, "%02d", Date.Month);
      TEXT_SetText(hItem, temp);  
      
      woy = rtcCalcYearWeek(Date.Year + 2000, Date.Month, Date.Date, Date.WeekDay > 0 ? (Date.WeekDay) - 1 : 0);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WEEK);
      snprintf(temp, 5, "W%02d", woy);
      TEXT_SetText(hItem, temp); 
      
    break;     
    
  case WM_TIMER:  
    Id = WM_GetTimerId(pMsg->Data.v);
    
    if (Id == GUI_ENV_REFRESH_ID)
    {
      WM_SendMessageNoPara (pMsg->hWin, TEMPERATURE_UPDATE);
      WM_RestartTimer(pMsg->Data.v, ENV_REFRESH_PERIOD);      
    }
    
    if(Id == GUI_CALENDAR_REFRESH_ID) 
    {        
      WM_SendMessageNoPara (pMsg->hWin, CALENDAR_UPDATE);
      WM_RestartTimer(pMsg->Data.v, CALENDAR_REFRESH_PERIOD);       
    }
    
    break;  
      
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    Node = pMsg->Data.v;  

    
    if(Node == WM_NOTIFICATION_RELEASED)
    {
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */

      if(Id == ID_TEMPERATURE)
      {        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IN_OUTDOUR);
        if (place_toggle)
          IMAGE_SetBitmap(hItem, &bmicon_indoor);
        else
          IMAGE_SetBitmap(hItem, &bmicon_outdoor);
        
        place_toggle = 1- place_toggle;
        WM_SendMessageNoPara (pMsg->hWin, TEMPERATURE_UPDATE);
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
*       _cbWin
*/
static void _cbWin(WM_MESSAGE* pMsg) {
    GUI_RECT         Rect;
    WM_MOTION_INFO* pInfo;
    WM_HWIN hItem;
    switch (pMsg->MsgId) {
      
    case WM_PAINT:
      WINDOW_SetBkColor(pMsg->hWin, GUI_MAKE_COLOR(0xFF000000));      
      GUI_Clear();
      break;
        
    case WM_MOTION:
        pInfo = (WM_MOTION_INFO*)pMsg->Data.p;
        switch (pInfo->Cmd) {
        case WM_MOTION_INIT:
            WM_GetClientRectEx(pMsg->hWin, &Rect);
            pInfo->SnapX = (Rect.x1 + 1)/2;
            break;
        }
        if(pInfo->FinalMove)
        {
          WM_GetWindowRectEx(hHomeFrame, &Rect);
          if( Rect.x0 >= 0)
          {
            hItem = WM_GetDialogItem(hMainFrame, ID_RADIO_0);
            RADIO_SetValue(hItem, 1);
            hItem = WM_GetDialogItem(hMainFrame, ID_RADIO_1);
            RADIO_SetValue(hItem, 0);   
          }
          else
          {
            hItem = WM_GetDialogItem(hMainFrame, ID_RADIO_0);
            RADIO_SetValue(hItem, 0);
            hItem = WM_GetDialogItem(hMainFrame, ID_RADIO_1);
            RADIO_SetValue(hItem, 1);   
          }
        }
        break;

    default:
        WM_DefaultProc(pMsg);
    }
}
/*********************************************************************
*
*       _cbMainDialog
*/
static void _cbMainDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  static WM_HTIMER   hTimer = 0;  
  static WM_HTIMER   mTimer = 0;    
  static WM_HTIMER   wifiTimer = 0;  
  static WM_HTIMER   TimeRefreshTimer = 0;    
  
  int Id, Node;
  
  char temp[20];
  static uint32_t toggle =0;  
  static uint8_t time_speed = 0;
  static uint8_t menu_state = 0;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;

    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(GUI_TRANSPARENT));    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_INTERNET);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xCECECE));
    TEXT_SetFont(hItem, &GUI_FontDigitGraphics20);
    TEXT_SetText(hItem, "connecting...");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
    RADIO_SetValue(hItem, 1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
    RADIO_SetValue(hItem, 0);    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME_HOUR);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));
    TEXT_SetFont(hItem, &GUI_FontDigita_Clock);
    TEXT_SetText(hItem, "00");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TIME_MIN);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));
    TEXT_SetFont(hItem, &GUI_FontDigita_Clock);
    TEXT_SetText(hItem, "00");      
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DOT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));
    TEXT_SetFont(hItem, &GUI_FontDigita_Clock);
    TEXT_SetText(hItem, ":");       
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
    IMAGE_SetBitmap(hItem, &bmicon_wifi);
    WM_HideWin(hItem);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU);
    IMAGE_SetBitmap(hItem, &bmicon_menu);   
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_INTERNET);
    IMAGE_SetBitmap(hItem, &bmicon_internet);   
    WM_HideWin(hItem);

    hBottomFrame = WM_CreateWindowAsChild(0, 300, LCD_GetXSize() * 2, 160, hMainFrame, WM_CF_SHOW | WM_CF_MOTION_X, _cbWin, 0);
    WM_MOTION_SetMoveable(hBottomFrame, WM_CF_MOTION_X, 1);
        
    hHomeFrame = GUI_CreateDialogBox(_aHomeDialogCreate, GUI_COUNTOF(_aHomeDialogCreate), _cbHomeDialog, hBottomFrame, 0, 0);
    hWeatherFrame = GUI_CreateDialogBox(_aWeatherDialogCreate, GUI_COUNTOF(_aWeatherDialogCreate), _cbWeatherDialog, hBottomFrame, LCD_GetXSize(), 0);
        
    hSettingsFrame = GUI_CreateDialogBox(_aSettingsDialogCreate, GUI_COUNTOF(_aSettingsDialogCreate), _cbSettingsDialog, hMainFrame, 0, 300);
    WM_HideWindow(hSettingsFrame);
    
    if(!TimeRefreshTimer) TimeRefreshTimer = WM_CreateTimer(pMsg->hWin, GUI_TIME_REFRESH_ID, TIME_REFRESH_PERIOD, 0);     
    WM_SendMessageNoPara (pMsg->hWin, TIME_UPDATE);
    break;
    
  case WIFI_DISCONNECTED:
    if(wifiTimer) WM_DeleteTimer(wifiTimer);
    wifiTimer = 0;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
    WM_HideWin(hItem); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_INTERNET);
    WM_HideWin(hItem); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_INTERNET);
    TEXT_SetText(hItem, "Wifi Disconnected");
    
    break;
    
  case WIFI_CONNECTED:
    if(wifiTimer) WM_DeleteTimer(wifiTimer); 
    wifiTimer = 0;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
    WM_ShowWin(hItem);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_INTERNET);
    TEXT_SetText(hItem, Hotspot[wifi_profile].SSID);
    
    break;
    
  case WIFI_CONNECTING: 
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILE_INTERNET);
    TEXT_SetText(hItem, "connecting...");
    if(!wifiTimer) wifiTimer = WM_CreateTimer(pMsg->hWin, WIFI_CONNECTING_TIMER_ID, WIFI_REFRESH_PERIOD, 0);       
    break;
    
  case INTERNET_AVAILABLE:
    hItem = WM_GetDialogItem(pMsg->hWin, ID_INTERNET);
    WM_ShowWin(hItem);    
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
    
    if(ui_enable_time_setting)
    {
      if((Id == TIME_HOUR_TIMER_ID) || (Id == TIME_MIN_TIMER_ID))
      {

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
        if(time_speed++ == 0)
           WM_RestartTimer(pMsg->Data.v, 5 * TIME_SETTING_REFRESH_PERIOD);
        else
          WM_RestartTimer(pMsg->Data.v, TIME_SETTING_REFRESH_PERIOD);
      }
    }
    else
    {
      //TODO : control on Touch
    }
    
    if (Id == WIFI_CONNECTING_TIMER_ID)
    {
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI);
      if(WM_IsVisible(hItem))
        WM_HideWin(hItem);  
      else
        WM_ShowWin(hItem);     
      WM_RestartTimer(pMsg->Data.v, WIFI_REFRESH_PERIOD);
    }
    
    if (Id == GUI_TIME_REFRESH_ID)
    {  
      
      WM_SendMessageNoPara (pMsg->hWin, TIME_UPDATE);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_DOT); 
      toggle = 1- toggle;
      
      if(toggle)
        TEXT_SetText(hItem, ":");       
      else
        TEXT_SetText(hItem, " ");     
      
      WM_RestartTimer(pMsg->Data.v, TIME_REFRESH_PERIOD);
    }
    break;  
      
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    Node = pMsg->Data.v;  

    
    if(Node == WM_NOTIFICATION_CLICKED)
    {
      
      Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
      
      
      if(Id == ID_TIME_HOUR)
      {
        if(!hTimer) hTimer = WM_CreateTimer(pMsg->hWin, TIME_HOUR_TIMER_ID, TIME_SETTING_REFRESH_PERIOD, 0);         
      }
      if(Id == ID_TIME_MIN)
      {
        if(!mTimer) mTimer = WM_CreateTimer(pMsg->hWin, TIME_MIN_TIMER_ID, TIME_SETTING_REFRESH_PERIOD, 0);           
      } 

      if(Id == ID_MENU)
      { 
        if (menu_state)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU);
          IMAGE_SetBitmap(hItem, &bmicon_home_act);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
        else
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU);
          IMAGE_SetBitmap(hItem, &bmicon_menu_act);    
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
        }
      }
      
    }
    
    if(Node == WM_NOTIFICATION_RELEASED)
    {
      if(ui_enable_time_setting)
      {
        Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
        if(Id == ID_TIME_HOUR)
        {
          WM_DeleteTimer(hTimer); 
          hTimer = 0;
        }
        if(Id == ID_TIME_MIN)
        {
          WM_DeleteTimer(mTimer); 
          mTimer = 0;
        }
        time_speed = 0;
      }
      
      if(Id == ID_MENU)
      { 
        if (menu_state)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU);
          IMAGE_SetBitmap(hItem, &bmicon_menu);          
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
          WM_ShowWindow(hBottomFrame);
          WM_HideWindow(hSettingsFrame);
          ui_enable_time_setting = 1;
          ui_set_setting_mode();
          
        }
        else
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU);
          IMAGE_SetBitmap(hItem, &bmicon_home);    
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
          WM_HideWindow(hBottomFrame);
          WM_ShowWindow(hSettingsFrame);       
        }
        menu_state = 1- menu_state;
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
void ui_set_setting_mode (void)
{
  WM_HWIN hItem;  
 
  if(!ui_enable_time_setting)
  {
    hItem = WM_GetDialogItem(hMainFrame, ID_TIME_HOUR);
    TEXT_SetTextColor(hItem, SETTINGS_COLOR);
    hItem = WM_GetDialogItem(hMainFrame, ID_TIME_MIN);
    TEXT_SetTextColor(hItem, SETTINGS_COLOR);
    hItem = WM_GetDialogItem(hMainFrame, ID_DOT);
    TEXT_SetTextColor(hItem, SETTINGS_COLOR);    
  }
  else
  {
    hItem = WM_GetDialogItem(hMainFrame, ID_TIME_HOUR);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));
    hItem = WM_GetDialogItem(hMainFrame, ID_TIME_MIN);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));
    hItem = WM_GetDialogItem(hMainFrame, ID_DOT);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0xf7d4a3));     
  }
  ui_enable_time_setting = 1- ui_enable_time_setting;
}

/*********************************************************************
*
*       UI_SetWifiDisconnected
*/
void UI_SetWifiDisconnected(void)
{  
  WM_SendMessageNoPara (hMainFrame, WIFI_DISCONNECTED);   
}

/*********************************************************************
*
*       UI_SetWifiConnected
*/
void UI_SetWifiConnected(void)
{
  
  WM_SendMessageNoPara (hMainFrame, WIFI_CONNECTED);     
}
/*********************************************************************
*
*       UI_SetWifiConnecting
*/
void UI_SetWifiConnecting(void)
{
  WM_SendMessageNoPara (hMainFrame, WIFI_CONNECTING); 
}

/*********************************************************************
*
*       UI_SetInternetAvailable
*/
void UI_SetInternetAvailable(void)
{
  WM_SendMessageNoPara (hMainFrame, INTERNET_AVAILABLE); 
}

/*********************************************************************
*
*       UI_ForceUpdateTime
*/
void UI_ForceUpdateTime(void)
{
  WM_SendMessageNoPara (hMainFrame, TIME_UPDATE);
  WM_SendMessageNoPara (hHomeFrame, CALENDAR_UPDATE);  
}

/*********************************************************************
*
*       UI_SetWifiConnecting
*/
void UI_ForceUpdateWhether(void)
{
  WM_SendMessageNoPara (hHomeFrame, ID_CONDITION_TXT);  
  WM_SendMessageNoPara (hWeatherFrame, WEATHER_UPDATE);    
}

/*********************************************************************
*
*       UI_CreateMainFame
*/

static WM_HWIN UI_CreateMainFame(void) {

  WM_SetSize(WM_HBKWIN, LCD_GetXSize(), LCD_GetYSize()); 
  hMainFrame = GUI_CreateDialogBox(_aMainDialogCreate, GUI_COUNTOF(_aMainDialogCreate), _cbMainDialog, WM_HBKWIN, 0, 0);
  return hMainFrame;
}

/**
  * @brief  Callback routine of desktop window.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_DrawBitmap (&bmbackground, 0, 0);
    break;

  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       UI_CreateMainFame
*/

void UI_Init(void) {
 
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();
 
  /* Initialize the SDRAM */
 BSP_SDRAM_Init();
 GUI_Init(); 
 GUI_SelectLayer(0); 
 LCD_Off();
 GUI_Clear();  
 GUI_Exec(); 
 
 WM_SetCreateFlags(WM_CF_MEMDEV | WM_CF_MEMDEV_ON_REDRAW);
 WM_MULTIBUF_Enable(1);
 WM_MOTION_Enable(1);
 WM_MOTION_SetDefaultPeriod (100);

 WM_SetCallback(WM_GetDesktopWindowEx(0), _cbBk);

 GUI_SelectLayer(1);
 GUI_Clear(); 

 UI_CreateMainFame();

 LCD_On();
}
/*************************** End of file ****************************/
