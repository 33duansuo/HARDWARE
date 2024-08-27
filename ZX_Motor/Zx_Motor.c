#include "Zx_Motor.h"
#include "common.h"
#include "string.h"
#include "usart.h"
#include "delay.h"
#define GET_LOW_BYTE(A)  ((uint8)(A))
#define GET_HIGH_BYTE(A) ((uint8)((A) >> 8))
//extern uint8 flag_uart2_rev;
//extern char uart2_buf[50] ;
//extern char isUartRxCompleted ;
//extern char uart3_buf[50];
uint8 Zx_MotorCheckSum(uint8 buf[])
{
	uint8 i;
	uint32 sum=0;
	for(i=2;i<8;i++)
	{
		sum +=buf[i];
	}
	if(sum>255)sum&=0x00FF;
  return sum;
}

void Zx_MotorSetID(uint8 oldID,uint8 newID)
{
	uint8  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=oldID;
	buf[3]=Zx_Motor_ID_WRITE;
	buf[4]=0x00;
	buf[5]=newID;
	buf[6]=0x00;
	buf[7]=0x00;
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
  uartWriteBuf(buf, 10);//将指令发送出去
	//UB_ServoMsgHandle();

}
/*功能对应ID舵机转动角度以及转动时间 其中Position 范围为500-2500（对应舵机0-180度） 
	时间为ms 比如说1000就表示1000ms  最大范围为 5100
*/
void Zx_MotorMove(uint8 id,uint32 position,uint32 time)
{
	uint8  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=Zx_Motor_MOVE_ANGLE ;
//	buf[4]=(uint8)(map(position,500,2500,0,240));
	//buf[4]=position;
	buf[5]=time/20;
	buf[6]=0x00;
	buf[7]=time/20;
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
	uartWriteBuf(buf, 10);//将指令发送出去
	//Delay_ms(10);
}

void Zx_MotorReadPosition(uint8 id)
{
	uint8  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=Zx_Motor_READ_ANGLE ;
	buf[4]=0x00;
	buf[5]=0x00;
	buf[6]=0x00;
	buf[7]=0x00;
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
  uartWriteBuf(buf, 10); //将指令发送出去
	Delay_ms(1);
	//UB_ServoMsgHandle();

}

void Zx_MotorRunS(uint8 id,uint32 rpm)
{
	uint8  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=0x01  ;
	buf[4]=Zx_Motor_RUNS;
	buf[5]=0x00;
	buf[6]=GET_HIGH_BYTE(rpm);
	buf[7]=GET_LOW_BYTE(rpm);
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
	uartWriteBuf(buf, 10);//将指令发送出去
}

void Zx_MotorRunN(uint8 id,uint32 rpm)
{
	uint8  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=0x01  ;
	buf[4]=Zx_Motor_RUNN;
	buf[5]=0x00;
	buf[6]=GET_HIGH_BYTE(rpm);
	buf[7]=GET_LOW_BYTE(rpm);
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
	uartWriteBuf(buf, 10);//将指令发送出去
}

void Zx_MotorMsgHandle(void)
{
//	if(flag_uart2_rev)
//	{
			//UART_PutStr(USART1,"#ok.\r\n");
	//	flag_uart2_rev=0;
	//	uart1PrintBuf(uart2_buf,10);
//	}

}

