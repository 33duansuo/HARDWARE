#ifndef __ZX_MOTOR_H__
#define __ZX_MOTOR_H__
#include "common.h"
#define Zx_Motor_FRAME_HEADER1  0XFA
#define Zx_Motor_FRAME_HEADER2  0XAF
#define Zx_Motor_MOVE_ANGLE     0X01
#define Zx_Motor_LED            0X04
#define Zx_Motor_READ_ANGLE     0X02
#define Zx_Motor_ID_WRITE       0XCD
#define Zx_Motor_SET_OFFSET     0XD2
#define Zx_Motor_READ_OFFSET    0XD4
#define Zx_Motor_VERSION        0X01
#define Zx_Motor_FRAME_END      0XED
#define Zx_Motor_RUNS           0XFD
#define Zx_Motor_RUNN           0XFE



uint8 Zx_MotorCheckSum(uint8 buf[]);
void Zx_MotorSetID(uint8 oldID,uint8 newID);
void Zx_MotorMove(uint8 id,uint32 position,uint32 time);
void Zx_MotorReadPosition(uint8 id);
void Zx_MotorSetLED(uint8 id,uint8 on);
void Zx_MotorSetOffset(uint8 id,uint32 adjvalue);
void Zx_MotorReadOffset(uint8 id);
void Zx_MotorMsgHandle(void);
void Zx_MotorRunS(uint8 id,uint32 rpm);
void Zx_MotorRunN(uint8 id,uint32 rpm);


#endif
