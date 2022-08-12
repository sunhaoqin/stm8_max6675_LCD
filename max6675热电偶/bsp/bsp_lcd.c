
#include "bsp_config.h"

/*
如果SEG不是0-7的顺序，比如改为0-3，6-9，中间空出两个引脚
修改方法如下
1.更改LCD_GLASS_Init()中的端口LCD_PortMaskRegister
2.更改数组LCD_MAP[8][MAX_LCD_SEG][2]

*/

const uint8_t LCD_CHAR[] = { 

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x00-0x0F*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x10-0x1F*/
 /* space !    "    #    $    %    &    ￡    (    )   *    +    ,    -    .    /    */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x00, /*0x20-0x2F*/
 /*  0     1   2    3    4    5    6    7    8    9    :    ;    <    =    >    ?    */
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80,0x00,0x00,0x00,0x00,0x00, /*0x30-0x3F*/
 /*  @     A    B    C    D    E    F    G    H    I    J    K    L    M    N    O   */
    0x00,0x77,0x7f,0x39,0x00,0x79,0x71,0x00,0x76,0x00,0x00,0x00,0x38,0x00,0x00,0x3f, /*0x40-0x4F*/
/*    P    Q   R     S    T   U    V     W    X    Y    Z        */
    0x73,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x50-0x5F*/
/*    '    a    b    c    d    e    f    g   h     i    j    k    l    m    n    o   */
    0x00,0x5c,0x7c,0x58,0x5e,0x00,0x00,0x00,0x74,0x00,0x00,0x00,0x00,0x00,0x54,0x5c, /*0x60-0x6F*/
/*    p    q    r    s    t    u   v     w    x    y    z        */    
    0x73,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x70-0x7F*/       
    
};

const char LCD_MAP[8][MAX_LCD_SEG][2] = {
    {
        // 1
        //{com, seg}
        // A       // B     // C     // D
        {0, 0}, {0, 1}, {2, 1}, {3,  1},
        // E      // F     // G     // P1
        {2, 0}, {1, 0}, {1, 1}, {3,  0},
    },
    
    {
        // 2
        //{com, seg}
        // A      // B     // C     // D
        {0, 2}, {0, 3}, {2, 3}, {3,  3},
        // E     // F     // G     //P2
        {2, 2}, {1, 2}, {1, 3}, {3,  2},
    },
    {
        // 3
        //{com, seg}
        // A      // B    // C     // D
        {0, 4}, {0, 5}, {2, 5}, {3,  5},
        // E     // F     // G     //P3
        {2, 4}, {1, 4}, {1, 5}, {3,  4},
    },
    
    {
        // 4
        //{com, seg}
        // A      // B     // C     // D
        {0, 6}, {0, 7}, {2, 7}, {3,  7},
        // E      // F     // G     // P4
        {2, 6}, {1, 6}, {1, 7}, {3,  6},
    },
    


};



void LCD_GLASS_Init(void)
{
  /*
    The LCD is configured as follow:
     - clock source = LSE (32.768 KHz)
     - Voltage source = Internal
     - Prescaler = 8
     - Divider = 17  
     - Mode = 1/4 Duty, 1/3 Bias
     - LCD frequency = (clock source * Duty) / (Prescaler * Divider)
                     = 60 Hz */

  /* Enable LCD clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_4, LCD_Divider_17, LCD_Duty_1_4,
           LCD_Bias_1_3, LCD_VoltageSource_Internal);

  /* Mask register*/
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
 // LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0x00);

  LCD_ContrastConfig(LCD_Contrast_Level_3);
  LCD_DeadTimeConfig(LCD_DeadTime_1);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_0);
  LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
}

/*
P1(com,seg)=(4,0)
P2(com,seg)=(4,2)
P3(com,seg)=(4,4)
P4(com,seg)=(4,6)
*/

void LCD_SetPixel(uint8_t Com, uint8_t Seg, LCD_PIXEL_SET_TypeDef On_Off)
{
    uint8_t x,com_offset,Seg_offset;
    
    if(Com>MAX_LCD_COM-1)
    {
        return;
    }
    
    x=Com*28 + Seg;
    com_offset = x/8;
    Seg_offset = x%8;
    
    LCD->CR4 &= (uint8_t)~LCD_CR4_PAGECOM;
    if(Com>3)
    {
         LCD->CR4 |= LCD_CR4_PAGECOM;
    }
    
    if(On_Off==PIXEL_ON)
    {
       LCD->RAM[com_offset]|= 0x01<<Seg_offset;
    }
    else
    {
      LCD->RAM[com_offset] &= (uint8_t)~(0x01<<Seg_offset);
    }
    
}    
    

void LCD_DisplayChar(uint8_t ch,uint8_t position,uint8_t signbit)
{
    uint8_t data,i;
    char com,seg;


    data = LCD_CHAR[ch];
  
    if(signbit)
    {
       data |=0x80;
    }
    
    for(i=0;i<MAX_LCD_SEG;i++)
    {
        com = *((char*)(LCD_MAP)+position*16+i*2);
        seg = *((char*)(LCD_MAP)+position*16+i*2+1);
        LCD_SetPixel(com, seg, PIXEL_OFF);
        if (data & (1 << i)) 
        {
                /* Turn on segment */
            LCD_SetPixel(com, seg, PIXEL_ON);
        }
    }
}

//支持 “12:40”,"1.2.3.4"," 123","123","123 "," 1.23","1.2:.3.4","1.2.:3.4"
void LCD_Printf(char *string,uint8_t position)
{
    uint8_t length,i,signbit;
    length = strlen(string);
    signbit=0;
    for(i=0;i<length;i++)
    {
        if((*string)!=':' && (*string)!='.')
        { 
          LCD_DisplayChar(*(string++),position++,signbit);
          signbit=0;
          /* 如果字符串长度大于4个字符，则退出（不含符号位）*/
          if(position==MAX_LCD_COM)
          {
             return;
          }
          
        }
        else if((*string)==':')
        {
            LCD_SetPixel(3, 0, PIXEL_ON);
            string++;
        }
        else if((*string)=='.')
        {
            signbit=1;
            string++;
        }
    }
    
}


void LCD_Printf_Temperature(s16 temperature)
{

   s8 buffer[3];
   s8 IntegerPart;
   
   IntegerPart=temperature;
       
   if(temperature>=0)
   {
       if(temperature<1000)
       {
           //0<=x<100    for 85.5C  and  8.5C
           sprintf((char*)buffer,"%3d",IntegerPart);
           LCD_Printf((char*)buffer,1);
       }
       else
       {
           // //x>100   for 125C
           // if(DecimalPart>=5)
           // {
           //      IntegerPart=IntegerPart+1;
           // }
           // sprintf((char*)buffer,"%3d%c",IntegerPart,'C');
           // LCD_Printf((char*)buffer);
       }
   }
   else
   {
        // DecimalPart = -DecimalPart;
        
        // if(temperature<= (-100))
        // {
        //     //x<=-10    for -11C
        //    if(DecimalPart>=5)
        //    {
        //      IntegerPart=IntegerPart-1;
        //    }
        //    sprintf((char*)buffer,"%3d%c",IntegerPart,'C');
        //    LCD_Printf((char*)buffer);
        // }
        // else
        // {
        //     //  -10<x<0  for -8.5C
        //    sprintf((char*)buffer,"%2d%c%d%c",IntegerPart,'.',DecimalPart,'C');
        //    LCD_Printf((char*)buffer);
        // }
   }
   
}


void LCD_Printf_Humidity(s16 humidity)
{
   u8 buffer[6];
   
   sprintf((char*)buffer,"%2d%c%d%c",humidity/10,'.',humidity%10,'H');
   LCD_Printf((char*)buffer,0);

}