#include "bsp_config.h"


RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_DateTypeDef   RTC_DateStr;
RTC_AlarmTypeDef  RTC_AlarmStr;


/**
  * @brief  Wait 1 sec for LSE stabilisation .
  * @param  None.
  * @retval None.
  * Note : TIM4 is configured for a system clock = 2MHz
  */
void LSE_StabTime(void)
{

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Configure TIM4 to generate an update event each 1 s */
  TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);
  /* Clear update flag */
  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  /* Wait 1 sec */
  while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET );

  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Disable TIM4 */
  TIM4_Cmd(DISABLE);

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}



 void RTC_Config(void)
{
//--------------------set RTC CLK----------------------------//    
  /* Enable LSE */
  CLK_LSEConfig(CLK_LSE_ON);
  /* Wait for LSE clock to be ready */
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  /* wait for 1 second for the LSE Stabilisation */
  LSE_StabTime();  
   /* Select LSE (32.768 KHz) as RTC clock source */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

//---------------------set wakeup time------------------------//

  // Configures the RTC wakeup timer_step = RTCCLK/16 = LSE/16 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  // RTC wake-up event every 2s (timer_step x (4095 + 1) )
  RTC_SetWakeUpCounter(40950);
  // Enable wake up unit Interrupt 
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);

}

void Calendar_Init(void)
{
    
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStr.RTC_AsynchPrediv = 0x7F;
  RTC_InitStr.RTC_SynchPrediv = 0x00FF;
  RTC_Init(&RTC_InitStr);

  RTC_DateStructInit(&RTC_DateStr);
  RTC_DateStr.RTC_WeekDay = RTC_Weekday_Wednesday;
  RTC_DateStr.RTC_Date = 29;
  RTC_DateStr.RTC_Month = RTC_Month_October;
  RTC_DateStr.RTC_Year = 14;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);

  RTC_TimeStructInit(&RTC_TimeStr);
  RTC_TimeStr.RTC_Hours   = 13;
  RTC_TimeStr.RTC_Minutes = 05;
  RTC_TimeStr.RTC_Seconds = 0;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);


  RTC_AlarmStructInit(&RTC_AlarmStr);
  RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = RTC_TimeStr.RTC_Hours;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = RTC_TimeStr.RTC_Minutes+1;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 0;
  RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
 // RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_Hours|RTC_AlarmMask_DateWeekDay;
  RTC_AlarmCmd(DISABLE);
  RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStr);

  
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  RTC_AlarmCmd(ENABLE);

}


void LCD_DisPlay_RTC_Time(void)
{
    char  tempbuf[16];
    
    RTC_GetTime(RTC_Format_BCD,&RTC_TimeStr);

    tempbuf[0]=0x30+ (RTC_TimeStr.RTC_Hours>>4) ;
    tempbuf[1]=0x30+ (RTC_TimeStr.RTC_Hours & 0x0F) ;
    tempbuf[2]=':';
    tempbuf[3]=0x30+ (RTC_TimeStr.RTC_Minutes>>4);
    tempbuf[4]=0x30+ (RTC_TimeStr.RTC_Minutes&0x0F);
    tempbuf[5]=0;

    LCD_Printf(tempbuf,0);

}

void LCD_DisPlay_RTC_Year(void)
{
    char  tempbuf[16];
    RTC_GetDate(RTC_Format_BCD,&RTC_DateStr);
    
    tempbuf[0]=0x32 ;
    tempbuf[1]=0x30 ;
    tempbuf[2]=0x30+ (RTC_DateStr.RTC_Year>>4);
    tempbuf[3]=0x30+ (RTC_DateStr.RTC_Year&0x0F);
    tempbuf[4]=0;

    LCD_Printf(tempbuf,0);

}

void LCD_DisPlay_RTC_Date(void)
{
    char  tempbuf[16];
    RTC_GetDate(RTC_Format_BCD,&RTC_DateStr);
    
    tempbuf[0]=0x30+ (RTC_DateStr.RTC_Month>>4);
    tempbuf[1]=0x30+ (RTC_DateStr.RTC_Month&0x0F);
    tempbuf[2]='.';
    tempbuf[3]=0x30+ (RTC_DateStr.RTC_Date>>4);
    tempbuf[4]=0x30+ (RTC_DateStr.RTC_Date&0x0F);
    tempbuf[5]=0;

    LCD_Printf(tempbuf,0);

}


void RTC_Minute_Alarm_Set(void)
{
    RTC_GetAlarm(RTC_Format_BIN,  &RTC_AlarmStr);
    RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = RTC_AlarmStr.RTC_AlarmTime.RTC_Hours;
    RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes+1;
    if(RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes==60)
    {
        RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes=0;
        RTC_AlarmStr.RTC_AlarmTime.RTC_Hours=RTC_AlarmStr.RTC_AlarmTime.RTC_Hours+1;
    }
    if(RTC_AlarmStr.RTC_AlarmTime.RTC_Hours==24)
    {
        RTC_AlarmStr.RTC_AlarmTime.RTC_Hours=0;
    }
    RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 0;
    RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
        
    RTC_AlarmCmd(DISABLE);
    RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStr);
    //RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    RTC_AlarmCmd(ENABLE);
}



void RTC_Date_Time_Set(char *time)
{
    
  RTC_TimeStr.RTC_Seconds=0;  
  RTC_TimeStr.RTC_Minutes= *(time+0)+(*(time+1))*16;
  RTC_TimeStr.RTC_Hours=  *(time+2)+(*(time+3))*16;
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStr);
  
  RTC_DateStr.RTC_Date= *(time+4)+(*(time+5))*16;
  RTC_DateStr.RTC_Month=(RTC_Month_TypeDef) (*(time+6)+(*(time+7))*16);
  RTC_DateStr.RTC_Year=  *(time+8)+ (*(time+9))*16;  
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStr);
 
   
  //更新RTC_Alarm
  RTC_AlarmStructInit(&RTC_AlarmStr);  
  RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = RTC_TimeStr.RTC_Hours;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = RTC_TimeStr.RTC_Minutes+1;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 0;
  RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
 // RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_Hours|RTC_AlarmMask_DateWeekDay;
  RTC_AlarmCmd(DISABLE);
  RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStr);
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  RTC_AlarmCmd(ENABLE);

}

void LCD_Printf_Blink(char *string,u8 position)
{
    u8 i;
    static u16 blinktime;
    
    if(blinktime%25 ==0)
    {
        for(i=0;i<4;i++)
        {

           LCD_DisplayChar((*(string+(position/4)*4+i)+0x30),3-i,0);  
           
           //当为时间时，显示“：”
           if( (position/4)==0 && i==3)
           {
              LCD_DisplayChar((*(string+(position/4)*4+i)+0x30),3-i,1);
           }
           //当为日期时，显示“.”
           if( (position/4)==1  && i==1)
           {
             LCD_DisplayChar((*(string+(position/4)*4+i)+0x30),3-i,1);
           }
           //闪烁
           if(i==(position%4) && blinktime%50==0)
           { 
               LCD_DisplayChar(' ',3-i,0);
               //符号不闪烁
               if( (position/4)==0 && i==3)
               {
                 LCD_DisplayChar(' ',3-i,1);
               }
               if( (position/4)==1 && i==1)
               {
                 LCD_DisplayChar(' ',3-i,1);
               }                                                
           }        
       }
    }
   blinktime++;

}


u8 SetRTC(u8 key)
{
     static char  tempbuf[3][4];
     static u8 blink_position,runonce;
     

    
    switch(key)
    {
      case 0x01:   //短按S1，移位
            
                blink_position++;                           
                if(blink_position==10)
                {
                  blink_position=0;
                }
                      
                break;
              
      case 0x02:   //短按S2，改变数值
          
                *((char*)(tempbuf)+blink_position)
                    =*((char*)(tempbuf)+blink_position)+1;
                
               //限制时间的四个位置
                if(blink_position<4)
                {
                  if(tempbuf[0][0]==10 && blink_position==0)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  } 
                  if(tempbuf[0][1]==6 && blink_position==1)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  }
                                    
                  if(tempbuf[0][2]==10 && blink_position==2)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  } 
                  if(tempbuf[0][3]==3 && blink_position==3)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  }
                  
                }
                //限制月和日的四个位置
                if((blink_position>=4) &&(blink_position<8))
                {
                  if(tempbuf[1][0]==10 && blink_position==4)
                  {
                   *((char*)(tempbuf)+blink_position)=0;
                  } 
                  if(tempbuf[1][1]==4 && blink_position==5)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  }
                                    
                  if(tempbuf[1][2]==10 && blink_position==6)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  } 
                  if(tempbuf[1][3]==2 && blink_position==7)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  }
                  
                }
                //限制年份的最后两个位置
                if(blink_position>=8)
                {
                  if(tempbuf[2][0]==10 && blink_position==8)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  } 
                  if(tempbuf[2][1]==10 && blink_position==9)
                  {
                    *((char*)(tempbuf)+blink_position)=0;
                  }
                                    
                }                
                  
                break;    
        
      case 0x11:   //长按S1，进入，再次长按S1，设置完成退出
                
                if(runonce==0)
                {
                    RTC_GetTime(RTC_Format_BCD,&RTC_TimeStr);
                    RTC_GetDate(RTC_Format_BCD,&RTC_DateStr);
     
                    tempbuf[0][3]=(RTC_TimeStr.RTC_Hours>>4) ;      //高位
                    tempbuf[0][2]=(RTC_TimeStr.RTC_Hours & 0x0F) ;   //低位
                    tempbuf[0][1]=(RTC_TimeStr.RTC_Minutes>>4);     //高位
                    tempbuf[0][0]=(RTC_TimeStr.RTC_Minutes&0x0F);  //低位

                    tempbuf[1][3]=(RTC_DateStr.RTC_Month>>4);
                    tempbuf[1][2]=(RTC_DateStr.RTC_Month&0x0F);
                    tempbuf[1][1]=(RTC_DateStr.RTC_Date>>4);
                    tempbuf[1][0]=(RTC_DateStr.RTC_Date&0x0F);
    
                    tempbuf[2][3]=2 ;
                    tempbuf[2][2]=0 ;
                    tempbuf[2][1]=(RTC_DateStr.RTC_Year>>4);
                    tempbuf[2][0]=(RTC_DateStr.RTC_Year&0x0F);
                    blink_position=0;
                    runonce=1;
                    break;
                }
                else
                {
                    RTC_Date_Time_Set((char*)tempbuf);
                    blink_position=0;
                    runonce=0;
                    return 0;  //0：设置完成    
                    break;
                }
                
      case 0xff:   //10s 无按键
                blink_position=0;
                runonce=0;
                return 0;
                break;
      default: 
                break;
    }            
    
    LCD_Printf_Blink((char*)tempbuf,blink_position);
    
    return 1; //1：设置中
         
}


