#ifndef __MAX6675_H
#define __MAX6675_H
 
#include "bsp_config.h"
 
#define MAX6675_CS1_PORT  GPIOB
#define MAX6675_CS1_PIN   GPIO_Pin_4
 
void max6675_init(void);			 //初始化max6675模块 
uint8_t max6675_readWriteByte(void); //max6675模块读写一个字节
uint16_t max6675_readRawValue(void);
float max6675_readTemperature(void);
 
 
#endif