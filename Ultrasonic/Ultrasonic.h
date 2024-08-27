#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#include "stm32f10x.h"
//常用的类型定义
typedef unsigned char	bool, BOOL;

typedef unsigned char	u8;
typedef signed char		s8, S8, int8, INT8;

typedef unsigned short	u16;



#define TRUE   1
#define FALSE  0




void InitUltrasonic(void);
void CheckUltrasonic(void);
u16 GetDistance(void);
u16 getMeanDistance(u8 mean,u16 DIST);
#endif
