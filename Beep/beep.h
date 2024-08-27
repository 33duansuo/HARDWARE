#ifndef __BEEP_H__
#define __BEEP_H__
#include "sys.h"

#define ON  0
#define OFF 1
#define BEEP(a) if(a) \
											GPIO_SetBits(GPIOD,GPIO_Pin_2); \
								else \
											GPIO_ResetBits(GPIOD,GPIO_Pin_2)
								
#define	Beep PDout(2)  //Ñ¡ÖÐFLASH										

void Beep_Init(void);
void Beep_Test(void);
void N_Beep(char n);
#endif
