#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#include "bsp_config.h"
#include <string.h>
#include <stdio.h>

#define MAX_LCD_COM     4
#define MAX_LCD_SEG     8


typedef enum {
  PIXEL_OFF=0,
  PIXEL_ON=1
} LCD_PIXEL_SET_TypeDef;

void LCD_GLASS_Init(void);
void LCD_SetPixel(uint8_t Com, uint8_t Seg, LCD_PIXEL_SET_TypeDef On_Off);
void LCD_DisplayChar(uint8_t ch,uint8_t position,uint8_t signbit);
void LCD_Printf(char *string,uint8_t position);
void LCD_Printf_Temperature(s16 temperature);
void LCD_Printf_Humidity(s16 humidity);
#endif