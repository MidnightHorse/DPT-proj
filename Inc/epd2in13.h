/*
 * edp2in13.h
 *
 *  Created on: Feb 24, 2026
 *      Author: Austi
 */

#ifndef INC_EPD2IN13_H_
#define INC_EPD2IN13_H_

#include "DEV_Config.h"
#ifndef UBYTE
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t
#endif


// Display resolution
#define EPD_2in13_V4_WIDTH       122
#define EPD_2in13_V4_HEIGHT      250

void EPD_2in13_V4_Init(void);
void EPD_2in13_V4_Init_Fast(void);
void EPD_2in13_V4_Init_GUI(void);
void EPD_2in13_V4_Clear(void);
void EPD_2in13_V4_Clear_Black(void);
void EPD_2in13_V4_Display(UBYTE *Image);
void EPD_2in13_V4_Display_Fast(UBYTE *Image);
void EPD_2in13_V4_Display_Base(UBYTE *Image);
void EPD_2in13_V4_Display_Partial(UBYTE *Image);
void EPD_2in13_V4_Sleep(void);


#endif
