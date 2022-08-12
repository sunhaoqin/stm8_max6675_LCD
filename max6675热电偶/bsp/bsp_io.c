

/******************** (C) COPYRIGHT  Çàµº´ºöªµç×ÓÑÐ·¢²¿ ************************
 * ÎÄ¼þÃû  £ºled.c
 * ÃèÊö    £ºIO¿ÚÅäÖÃº¯Êý¿â   
 *Ó²¼þÆ½Ì¨ £ºÊÖÇ¹¹ñCTSQ01-M-V100£¨STM8S105K4£©
 * ¿â°æ±¾  £ºV2.0.0
 * ×÷Õß    £ºWangjiandong
 *ÐÞ¸ÄÊ±¼ä £º2013-11-21

  STM8S105Ó²¼þÁ¬½Ó
    |--------------------|
    |  LED_L-PB3         |
    |  LED_R-PD2         |
    |  LED_G-PD0         |
    |  BUZZER-PC3        |
    |  PCON-PB2          |
    |  MIA-PC2           |
    |  MIB-PC1           |
    |  MEN-PE5           |
    |--------------------|

*******************************************************************************/

#include "bsp_config.h"



/*Pin configured Input pull-up, external interrupt, EXTI_Trigger_Falling*/
void EXTI_InputPin_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
    EXTI_Trigger_TypeDef EXTI_Trigger = EXTI_Trigger_Falling;
 
    GPIO_Init(GPIOx, GPIO_Pin, GPIO_Mode_In_PU_IT);
      
 // EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)(INPUT_PIN[InPinName]), EXTI_Trigger_Falling);
    switch (GPIO_Pin)
    {
        case GPIO_Pin_0:
        EXTI->CR1 &=  (uint8_t)(~EXTI_CR1_P0IS);
        EXTI->CR1 |= (uint8_t)((uint8_t)(EXTI_Trigger) << EXTI_Pin_0);
        break;
        case GPIO_Pin_1:
        EXTI->CR1 &=  (uint8_t)(~EXTI_CR1_P1IS);
        EXTI->CR1 |= (uint8_t)((uint8_t)(EXTI_Trigger) << EXTI_Pin_1);
        break;
        case GPIO_Pin_2:
        EXTI->CR1 &=  (uint8_t)(~EXTI_CR1_P2IS);
        EXTI->CR1 |= (uint8_t)((uint8_t)(EXTI_Trigger) << EXTI_Pin_2);
        break;
        case GPIO_Pin_3:
        EXTI->CR1 &=  (uint8_t)(~EXTI_CR1_P3IS);
        EXTI->CR1 |= (uint8_t)((uint8_t)(EXTI_Trigger) << EXTI_Pin_3);
        break;
        case GPIO_Pin_4:
        EXTI->CR2 &=  (uint8_t)(~EXTI_CR2_P4IS);
        EXTI->CR2 |= (uint8_t)((uint8_t)(EXTI_Trigger) << ((uint8_t)EXTI_Pin_4 & (uint8_t)0xEF));
        break;
        case GPIO_Pin_5:
        EXTI->CR2 &=  (uint8_t)(~EXTI_CR2_P5IS);
        EXTI->CR2 |= (uint8_t)((uint8_t)(EXTI_Trigger) << ((uint8_t)EXTI_Pin_5 & (uint8_t)0xEF));
        break;
        case GPIO_Pin_6:
        EXTI->CR2 &=  (uint8_t)(~EXTI_CR2_P6IS);
        EXTI->CR2 |= (uint8_t)((uint8_t)(EXTI_Trigger) << ((uint8_t)EXTI_Pin_6 & (uint8_t)0xEF));
        break;
        case GPIO_Pin_7:
        EXTI->CR2 &=  (uint8_t)(~EXTI_CR2_P7IS);
        EXTI->CR2 |= (uint8_t)((uint8_t)(EXTI_Trigger) << ((uint8_t)EXTI_Pin_7 & (uint8_t)0xEF));
        break;
        default:
        break;
    }
}


void BSP_IO_Init(void)
{

    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(GPIOB,GPIO_Pin_3);
    
    //GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);
    //GPIO_SetBits(GPIOD,GPIO_Pin_7);
 
    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);

    GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);  
     
}



/************************END OF FILE****/