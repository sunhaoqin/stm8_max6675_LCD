
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "bsp_config.h"

void CLK_Config(CLK_SYSCLKSource_TypeDef CLK_SYSCLKSource ,CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv);
void SysTickInit(void);
u32 GetSysTick(void);
void SysTickIncrease(void);
void TimerDelay_1ms( uint32_t nTime);
void DelayMs(u16 nCount);
void Delay(u32 nCount);
#endif

