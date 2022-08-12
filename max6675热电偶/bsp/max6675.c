#include "max6675.h"
 
#define SPI1_CS_HIGH()   GPIO_SetBits(MAX6675_CS1_PORT,MAX6675_CS1_PIN) 
#define SPI1_CS_LOW()   GPIO_ResetBits(MAX6675_CS1_PORT,MAX6675_CS1_PIN); 
/**
  * @brief  max66675模块初始化
  * @param  None
  * @retval None
  */
void max6675_init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  GPIO_Init(GPIOB,GPIO_Pin_5  , GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOB,GPIO_Pin_7  , GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOB,GPIO_Pin_4,   GPIO_Mode_Out_PP_High_Slow);

  /* Deselect the FLASH: Chip Select high */
  SPI1_CS_HIGH();

  /* SPI configuration */
  SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_64, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);

  /* Enable SPI  */
  SPI_Cmd(SPI1, ENABLE);
} 
 
/**
  * @brief  max6675模块读写一个字节的数据
  * @param  txData：要发送的数据
  * @retval 接收到的数据
  */
uint8_t max6675_readWriteByte(void)
{		
    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPI1,0xff);
    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
    return SPI_ReceiveData(SPI1);
}  
 
/**
  * @brief  max6675模块读取测得的原始数据
  * @param  None
  * @retval 温度的原始数据
  */
uint16_t max6675_readRawValue(void)
{
  uint16_t tmp;
  
  SPI1_CS_LOW(); //enable max6675
  tmp = max6675_readWriteByte(); //read MSB
  tmp <<= 8;
  tmp |= max6675_readWriteByte(); //read LSB
  SPI1_CS_HIGH(); //disable max6675

  if (tmp & 4) 
  {
    // thermocouple open
    tmp = 4095; //未检测到热电偶
  }
  else 
  {
    tmp = tmp >> 3;
  }
  return tmp;
}
 
/**
  * @brief  max6675模块读取测得的原始数据
  * @param  None
  * @retval 温度值（单位：℃）
  */
float max6675_readTemperature(void)
{
  return (max6675_readRawValue() * 1024.0 / 4096);
}
//————————————————
//版权声明：本文为CSDN博主「顶点元」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/xiaoyuanwuhui/article/details/81734764

