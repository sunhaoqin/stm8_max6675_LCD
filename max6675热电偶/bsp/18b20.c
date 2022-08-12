
#include "bsp_config.h"
#include <stdio.h>
/****************************************
函数名称：void mode_output(void)
函数参数：无参
函数返回值：无
函数功能：输出模式
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void mode_output(void)
{
    GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow);
}
 
/****************************************
函数名称：void mode_input(void)
函数参数：无参
函数返回值：无
函数功能：输入模式
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void mode_input(void)
{
    GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
}
 
/****************************************
函数名称：void ds18b20_rst(void)
函数参数：无参
函数返回值：无
函数功能：复位
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void ds18b20_rst(void)
{
  mode_output();                //输出模式
  GPIO_ResetBits(GPIOF,GPIO_Pin_0);              //拉低DQ引脚
  DelayMs(600);         //延时480-960us
  GPIO_SetBits(GPIOF,GPIO_Pin_0);              //拉高DQ引脚  
  mode_input();
  while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0));         //等待器件应答（器件拉低），约15-60us后
  while(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0));        //应答脉冲出现后，等待器件拉高，约60-240us后
}
 
/****************************************
函数名称：void ds18b20_write_zero(void)
函数参数：无参
函数返回值：无
函数功能：写0位
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void ds18b20_write_zero(void)
{
  mode_output();                    //输出模式
  GPIO_ResetBits(GPIOF,GPIO_Pin_0);                   //拉低引脚 
  Delay(80);              //延时60-120us
  GPIO_SetBits(GPIOF,GPIO_Pin_0);                  //拉高引脚
  Delay(2);
} 
 
/****************************************
函数名称：void ds18b20_write_one(void)
函数参数：无参
函数返回值：无
函数功能：写1位
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void ds18b20_write_one(void)
{
  mode_output();                           //输出模式
  GPIO_ResetBits(GPIOF,GPIO_Pin_0);
  Delay(2);
  GPIO_SetBits(GPIOF,GPIO_Pin_0);
  Delay(80);
} 
 
/****************************************
函数名称：void ds18b20_read_bit(void)
函数参数：无参
函数返回值：无
函数功能：读1位数据
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
unsigned char ds18b20_read_bit(void)
{
  unsigned char data;
  mode_output();
  GPIO_ResetBits(GPIOF,GPIO_Pin_0);
  Delay(2);
  GPIO_SetBits(GPIOF,GPIO_Pin_0);;
  mode_input();
  Delay(5);
  if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0))
    data=1;
  else
    data=0;
  Delay(500);  
  return data;
}
 
/****************************************
函数名称：void ds18b20_write_byte(void)
函数参数：无参
函数返回值：无
函数功能：写一个字节
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void ds18b20_write_byte(unsigned char data)
{
  unsigned char i,testb;
  mode_output();
  for(i=0;i<8;i++)
  {
    testb=data&0x01;                 //从低位开始写
    data>>=1;
    if(testb)
      ds18b20_write_one();
    else
      ds18b20_write_zero();
  }
}
 
/****************************************
函数名称：unsigned char ds18b20_read_byte(void)
函数参数：无参
函数返回值：无
函数功能：读一个字节
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
unsigned char ds18b20_read_byte(void)
{
  unsigned char i,j,data=0;
  for(i=0;i<8;i++)
  {
    j=ds18b20_read_bit();
    data=(j<<7)|(data>>1);      //从低位开始读
  }
  return data;
}
 
/****************************************
函数名称：void ds18b20_start(void)
函数参数：无参
函数返回值：无
函数功能：开始转换
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void ds18b20_start(void)
{                                                                  
   ds18b20_rst();              
   ds18b20_write_byte(0xcc);   // 跳过ROM
   ds18b20_write_byte(0x44);   //温度转换
}
 
/****************************************
函数名称：void ds18b20_init(void)
函数参数：无参
函数返回值：无
函数功能：初始化
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
void ds18b20_init(void)
{
  //gpio_init();     //此引脚需根据相应的单片机进行配置
  ds18b20_rst();
} 
 
/****************************************
函数名称：short get_tempetature(void)
函数参数：无参
函数返回值：无
函数功能：获取温度
创建作者：王艇
创建时间：2021.5.21
联系方式：
修改作者：
修改时间：
修改说明：
****************************************/
short get_tempetature(void)
{
   unsigned char TL,TH;
   short tem;
   ds18b20_start();           //开始转换
   DelayMs(700);             //等待转换完成
   ds18b20_init();    
   ds18b20_write_byte(0xcc);   //跳过ROM
   ds18b20_write_byte(0xbe);   //读取暂存寄存器    
   TL=ds18b20_read_byte();     //低八位 
   TH=ds18b20_read_byte();     //高八位，注意前五位为符号位只有全为0的时候温度是正的  
   //printf(TH);
   //printf(TL);
   tem=TH;                      //获得高八位
   tem<<=8;   
   tem+=TL;                     //获得底八位
   return tem;    
}
//————————————————
//版权声明：本文为CSDN博主「小材大用」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/xiaocaidayong/article/details/117162292