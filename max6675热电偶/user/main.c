/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "bsp_config.h"
//#include <stdio.h>

#define HIGHTEMP 1
#define LOWTEMP 2
#define NORMALTEMP 0
#define ONOFF 4
#define TIMESET 3
#define BTN1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) 
#define BTN2 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7) 



void main(void)
{
    int mode=NORMALTEMP;
    int activetime=60;
    uint16_t temp,temphigh=0,templow=0,counter=0;
    CLK_Config(CLK_SYSCLKSource_HSI,CLK_SYSCLKDiv_64);
    BSP_IO_Init();
    LCD_GLASS_Init();
    max6675_init();
    temp=max6675_readTemperature();
    DelayMs(250);
    temp=max6675_readTemperature();
    temphigh=temp;
    templow=temp;
    //RTC_Config();
    while(1){
        if(BTN2==0){    //按键1按下，切换显示模式
          counter=0;
          switch(mode){
          case NORMALTEMP:
            mode=HIGHTEMP;
            LCD_Printf_Temperature(temphigh);
            LCD_Printf("H",0);
            break;
          case HIGHTEMP:
            mode=LOWTEMP;
            LCD_Printf_Temperature(templow);
            LCD_Printf("L",0); 
            break;
          case LOWTEMP:
            mode=TIMESET;
            LCD_Printf_Temperature(activetime);
            LCD_Printf("d",0);
             
            break;
          case TIMESET:
            mode=ONOFF;
            LCD_Printf("  on",0);
            break;
          case ONOFF:
            mode=NORMALTEMP;
            LCD_Printf_Temperature(temp);
            LCD_Printf("A",0);
            break;
          }
          while(BTN1==0);
          DelayMs(50);
        }

        if (BTN1==0){    //按键2按下，清零。
          counter=0;
          if(mode==HIGHTEMP){
              temp=max6675_readTemperature();
              temphigh=temp;
              LCD_Printf_Temperature(temphigh);
              LCD_Printf("H",0);
              DelayMs(250);
          }
          else if(mode==LOWTEMP){
              temp=max6675_readTemperature();
              templow=temp;  
              LCD_Printf_Temperature(templow);
              LCD_Printf("L",0);
              DelayMs(250);
          }
          else if(mode==ONOFF){
              LCD_Printf(" oFF",0);
              while(BTN1==0);
              EnterHalt();
              LCD_Printf("  on",0); 
              ExitHalt();
              counter=0;
              DelayMs(250);
          }
          else if(mode==TIMESET){
              activetime=activetime+10;
              if(activetime==130) activetime=10;
              LCD_Printf_Temperature(activetime);
              LCD_Printf("d",0);
          }
          while(BTN1==0);
          DelayMs(50);
        }
        

        temp=max6675_readTemperature();
        if(temp>temphigh) temphigh=temp;
        if(temp<templow) templow=temp;
        if(mode==NORMALTEMP){
              LCD_Printf_Temperature(temp);
              LCD_Printf("A",0);
        }
        else if(mode==HIGHTEMP){
              LCD_Printf_Temperature(temphigh);
              LCD_Printf("H",0);            
        }
        else if(mode==LOWTEMP){
              LCD_Printf_Temperature(templow);
              LCD_Printf("L",0);           
        }
        counter++;
        if(counter>(activetime*3)){
          LCD_Printf(" oFF",0); 
          EnterHalt();
          LCD_Printf("  on",0); 
          ExitHalt();
          counter=0;
        }
        DelayMs(250);
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
