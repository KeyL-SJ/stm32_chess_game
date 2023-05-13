 #include "exti.h"
 #include "usart.h"
 #include <sys.h>


 void extiInit(void)
 {
     EXTI_InitTypeDef extiInitStruct;
     NVIC_InitTypeDef nvicInitStruct;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);


     GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

     extiInitStruct.EXTI_Line = EXTI_Line4;
     extiInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
     extiInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
     EXTI_Init(&extiInitStruct);

     nvicInitStruct.NVIC_IRQChannel = EXTI4_IRQn;
     nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
     nvicInitStruct.NVIC_IRQChannelSubPriority = 0x02;
     nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&nvicInitStruct);
 }


void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4,3

}
