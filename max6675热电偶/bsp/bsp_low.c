#include "bsp_config.h"




__IO uint8_t  _DDR_Setting[7];     /* store DDR */
__IO uint8_t  _ODR_Setting[7];	   /* store ODR */
__IO uint8_t  _CR1_Setting[7];  /* store CR1 */
__IO uint8_t  _CR2_Setting[7];  /* store CR2 */



void SavePinSetting(void)
{
    /* Save DDR */
    _DDR_Setting[0] = GPIOA->DDR;
    _DDR_Setting[1] = GPIOB->DDR;
    _DDR_Setting[2] = GPIOC->DDR;
    _DDR_Setting[3] = GPIOD->DDR;
    _DDR_Setting[4] = GPIOE->DDR;
    _DDR_Setting[5] = GPIOF->DDR;
    _DDR_Setting[6] = GPIOG->DDR;

    /* Save ODR */
    _ODR_Setting[0] =  GPIOA->ODR;
    _ODR_Setting[1] =  GPIOB->ODR;
    _ODR_Setting[2] =  GPIOC->ODR;
    _ODR_Setting[3] =  GPIOD->ODR;
    _ODR_Setting[4] =  GPIOE->ODR;
    _ODR_Setting[5] =  GPIOF->ODR;
    _ODR_Setting[6] =  GPIOG->ODR;
    
    /* Save CR1 */
    _CR1_Setting[0] =  GPIOA->CR1;
    _CR1_Setting[1] =  GPIOB->CR1;
    _CR1_Setting[2] =  GPIOC->CR1;
    _CR1_Setting[3] =  GPIOD->CR1;
    _CR1_Setting[4] =  GPIOE->CR1;
    _CR1_Setting[5] =  GPIOF->CR1;
    _CR1_Setting[6] =  GPIOG->CR1;

    /* Save CR2 */
    _CR2_Setting[0] = GPIOA->CR2;
    _CR2_Setting[1] = GPIOB->CR2;
    _CR2_Setting[2] = GPIOC->CR2;
    _CR2_Setting[3] = GPIOD->CR2;
    _CR2_Setting[4] = GPIOE->CR2;
    _CR2_Setting[5] = GPIOF->CR2;
    _CR2_Setting[6] = GPIOG->CR2;

}


void RestorePinSetting(void)
{
    /* Restore DDR */
    GPIOA->DDR = _DDR_Setting[0];
    GPIOB->DDR = _DDR_Setting[1];
    GPIOC->DDR = _DDR_Setting[2];
    GPIOD->DDR = _DDR_Setting[3];
    GPIOE->DDR = _DDR_Setting[4];
    GPIOF->DDR = _DDR_Setting[5];
    GPIOG->DDR = _DDR_Setting[6];

    /* Restore ODR */
    GPIOA->ODR =  _ODR_Setting[0];
    GPIOB->ODR =  _ODR_Setting[1];
    GPIOC->ODR =  _ODR_Setting[2];
    GPIOD->ODR =  _ODR_Setting[3];
    GPIOE->ODR =  _ODR_Setting[4];
    GPIOF->ODR =  _ODR_Setting[5];
    GPIOG->ODR =  _ODR_Setting[6];
    
    /* Restore CR1 */
    GPIOA->CR1 =  _CR1_Setting[0];
    GPIOB->CR1 =  _CR1_Setting[1];
    GPIOC->CR1 =  _CR1_Setting[2];
    GPIOD->CR1 =  _CR1_Setting[3];
    GPIOE->CR1 =  _CR1_Setting[4];
    GPIOF->CR1 =  _CR1_Setting[5];
    GPIOG->CR1 =  _CR1_Setting[6];

    /* Restore CR2 */
    GPIOA->CR2 = _CR2_Setting[0];
    GPIOB->CR2 = _CR2_Setting[1];
    GPIOC->CR2 = _CR2_Setting[2];
    GPIOD->CR2 = _CR2_Setting[3];
    GPIOE->CR2 = _CR2_Setting[4];
    GPIOF->CR2 = _CR2_Setting[5];
    GPIOG->CR2 = _CR2_Setting[6];
}

void GPIO_LowPower_Config(void)
{

   // Port ABCDEFG in output push-pull 0 
   // GPIO_Init(GPIOC, GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);
   GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_PU_IT);
   GPIO_Init(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);

   //GPIO_Init(GPIOC,GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);
   //GPIO_Init(GPIOD, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);
   //GPIO_Init(GPIOE, GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);
   //GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);

   //BSP_IO_Init();
 
  //设置S1,S2为中断唤醒引脚，下降沿有效
   EXTI_InputPin_Init(GPIOB, GPIO_Pin_2);
   enableInterrupts();
}

void EnterHalt(void)
{
 /* Set GPIO*/	
   
   GPIO_LowPower_Config();  
   halt();
}



void ExitHalt(void)
{  
  
   //使用外部时钟
   disableInterrupts();
   CLK_Config(CLK_SYSCLKSource_HSI,CLK_SYSCLKDiv_64);
   BSP_IO_Init();
   max6675_init();
   //  RestorePinSetting();

}
