
#ifndef _BSP_RTC_H
#define _BSP_RTC_H


#include "bsp_config.h"


//extern RTC_InitTypeDef   RTC_InitStr;
//extern RTC_TimeTypeDef   RTC_TimeStr;
//extern RTC_DateTypeDef   RTC_DateStr;
//extern RTC_AlarmTypeDef  RTC_AlarmStr;

void RTC_Config(void);
void Calendar_Init(void);
void LCD_DisPlay_RTC_Time(void);
void LCD_DisPlay_RTC_Year(void);
void LCD_DisPlay_RTC_Date(void);
void RTC_Minute_Alarm_Set(void);
u8 SetRTC(u8 key);
#endif
