/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter (ST) for emWin V5.44.                      *
*        Compiled Nov 10 2017, 08:52:20                              *
*                                                                    *
*        (c) 1998 - 2017 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: icon_home                                             *
* Dimensions:  40 * 40                                               *
* NumColors:   2                                                     *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmicon_home;
extern GUI_CONST_STORAGE GUI_BITMAP bmicon_home_act;

/*********************************************************************
*
*       Palette
*
*  Description
*    The following are the entries of the palette table.
*    The entries are stored as a 32-bit values of which 24 bits are
*    actually used according to the following bit mask: 0xBBGGRR
*
*    The lower   8 bits represent the Red   component.
*    The middle  8 bits represent the Green component.
*    The highest 8 bits represent the Blue  component.
*/
static GUI_CONST_STORAGE GUI_COLOR _Colorsicon_home[] = {
#if (GUI_USE_ARGB == 0)
  0xCECECE, 0x000000
#else
  0xFFCECECE, 0xFF000000
#endif  


};

static GUI_CONST_STORAGE GUI_COLOR _Colorsicon_home_act[] = {
#if (GUI_USE_ARGB == 0)
  0xf7d4a3, 0x000000
#else
  0xFFf7d4a3, 0xFF000000
#endif
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palicon_home = {
  2,  // Number of entries
  0,  // No transparency
  &_Colorsicon_home[0]
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _Palicon_home_act = {
  2,  // Number of entries
  0,  // No transparency
  &_Colorsicon_home_act[0]
};


#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned char _acicon_home[] = {
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XX____XX, XXXX____, __XXXXXX,
  XXXXXXXX, XXXXXXXX, X______X, XXXX____, __XXXXXX,
  XXXXXXXX, XXXXXXXX, ________, XXXX____, __XXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXX____, __XXXXXX,
  XXXXXXXX, XXXXXX__, ________, __XX____, __XXXXXX,
  XXXXXXXX, XXXX____, ___XX___, ________, __XXXXXX,
  XXXXXXXX, XXX_____, _XXXXXX_, ________, __XXXXXX,
  XXXXXXXX, XX______, XXX__XXX, ________, __XXXXXX,
  XXXXXXXX, X______X, XX____XX, X_______, __XXXXXX,
  XXXXXXXX, ______XX, ________, XX______, __XXXXXX,
  XXXXXX__, _____XX_, ________, _XX_____, __XXXXXX,
  XXXXX___, ___XXX__, ________, __XXX___, ___XXXXX,
  XXXX____, __XXX___, ________, ___XXX__, ____XXXX,
  XXX_____, _XX_____, ________, _____XX_, _____XXX,
  XX______, XX______, ________, ______XX, ______XX,
  _______X, X_______, ________, _______X, X_______,
  _____XXX, ________, ________, ________, XXX_____,
  X___XXX_, ________, ________, ________, _XXX___X,
  XX_XXX__, ________, ________, ________, __XXX_XX,
  XXXXXX__, ________, ________, ________, __XXXXXX,
  XXXXXX__, ________, ________, ________, __XXXXXX,
  XXXXXX__, ________, ________, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXX__, ________, _XXXXXX_, ________, __XXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE GUI_BITMAP bmicon_home = {
  40, // xSize
  40, // ySize
  5, // BytesPerLine
  1, // BitsPerPixel
  _acicon_home,  // Pointer to picture data (indices)
  &_Palicon_home   // Pointer to palette
};

GUI_CONST_STORAGE GUI_BITMAP bmicon_home_act = {
  40, // xSize
  40, // ySize
  5, // BytesPerLine
  1, // BitsPerPixel
  _acicon_home,  // Pointer to picture data (indices)
  &_Palicon_home_act   // Pointer to palette
};

/*************************** End of file ****************************/
