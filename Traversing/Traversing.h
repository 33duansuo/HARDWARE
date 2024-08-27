#ifndef  _TRAVERSING_H
#define  _TRAVERSING_H
#include "stm32f10x.h"


void Auto_Traversing(void);
void Traversing_GPIO_Init(void);
void trace(void);
void motor_direction(void);
void calc_pid(void);
int readoffset(void);
void UART3_Init(void);
void USART3_IRQHandler(void);
void Uart3_NVIC_Init(void);


#endif
