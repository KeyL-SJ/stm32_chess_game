#ifndef __EXTI_H__
#define __EXTI_H__

void extiInit(void);
void KEY_Init(void); //IO初始化

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

#define BEEP PBout(8)	// BEEP,蜂鸣器接口	

#endif