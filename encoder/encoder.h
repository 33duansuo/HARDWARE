#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "sys.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
void Encoder_Init_TIM3(void);
int Read_Encoder(u8 TIMX);
void Encoder_Init_TIM_Exit0(void);
void Encoder_Init_TIM_Exit1(void);


#endif
