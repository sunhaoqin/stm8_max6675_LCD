

/******************** (C) COPYRIGHT  青岛春霆电子研发部************************
 *文件名   ：systick.c
 *描述     ：系统时钟设置 
 *硬件平台 ：手枪柜CTSQ01-M-V100 CTSQ01-M-V2.1（STM8S105K4）
 *软件版本 ：V2.1.0
 *作者     ：WangJiandong
 *修改时间 ：2014-9-30
 *实现功能 ：系统时钟初始化，中断函数，获取当前计数器值，及硬件准确延时
******************************************************************************/

#include "bsp_config.h"

static u32 SysClk1Ms;  //系统计数值，用于计时间


/*
*******************************************************************************
* 函数名称:  CLK_Config()
* 功能描述:  芯片时钟源设置
* 入口参数:  时钟源及分频
* 出口参数：无
* 特别注意：时钟源有4种：CLK_SYSCLKSource_HSI，CLK_SYSCLKSource_LSI 
  CLK_SYSCLKSource_HSE， CLK_SYSCLKSource_LSE
*******************************************************************************
*/
void CLK_Config(CLK_SYSCLKSource_TypeDef CLK_SYSCLKSource ,CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv)
{
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource);
  /* system clock prescaler */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource);

}


/*
*******************************************************************************
* 函数名称:  SysTickInit()
* 功能描述:  Timer设置
* 入口参数: 无
* 出口参数：无
* 特别注意：设置1ms中断，要与晶振频率配合
12M HSE  
*******************************************************************************
*/
void SysTickInit(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
    TIM4_DeInit();
    //定时1ms    16M ---TIM4_TimeBaseInit(TIM4_Prescaler_64, 250)
    //定时1ms    2M  ---TIM4_TimeBaseInit(TIM4_Prescaler_8, 250)
    TIM4_TimeBaseInit(TIM4_Prescaler_8, 250);
    TIM4_ARRPreloadConfig(ENABLE);
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
 //   TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4_Cmd(ENABLE);
}


/*
*******************************************************************************
* 函数名称:  SysTickIncrease()
* 功能描述:  SysClk1Ms加1
* 入口参数: 无
* 出口参数：无
* 特别注意：在TIM4中断中引用，每毫秒加1
*******************************************************************************
*/
void SysTickIncrease(void)
{
  SysClk1Ms++;
}


/*
*******************************************************************************
* 函数名称:  GetSysTick()
* 功能描述:  返回SysClk1Ms的值
* 入口参数: 无
* 出口参数：SysClk1Ms
* 特别注意：获取当前SysClk1Ms的值
*******************************************************************************
*/
u32 GetSysTick(void)
{
  return SysClk1Ms;
}


/*
*******************************************************************************
* 函数名称:  TimerDelay_1ms()
* 功能描述:  精确延时，1ms计
* 入口参数: 延时值
* 出口参数：无
* 特别注意：相当于delay，非中断方式精确延时
*******************************************************************************
*/
void TimerDelay_1ms( uint32_t nTime)
{
  uint32_t counter;
  
  counter=SysClk1Ms;
  
  while (nTime>(SysClk1Ms-counter));
  
}


/*
*******************************************************************************
* 函数名称:  DelayMs()
* 功能描述:  非精确延时1ms
* 入口参数: 延时值
* 出口参数：无
* 特别注意：非精确延时1ms
*******************************************************************************
*/
void DelayMs(u16 nCount)
{
	unsigned int i;
	while(nCount--)
	{
		for(i=0;i<33;i++) //143
		{
                    
		}
	}
}

/*
*******************************************************************************
* 函数名称:  Delay()
* 功能描述:  非精确延时
* 入口参数: 延时值
* 出口参数：无
* 特别注意：非精确延时
*******************************************************************************
*/
  void Delay(u32 nCount)
{
  while (nCount != 0)
  {
    nCount--;
  }
}