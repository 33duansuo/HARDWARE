#include "Traversing.h"
#include "motor.h"
#include "common.h"
#include "zx_motor.h"
#include "usart.h"	
#include "Ultrasonic.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
u16 distance;	
u32 ZMRPM = 255;
u32  MPWM  =400;
u32  MIPWM = 100;
u32  MAPWM = 600;
float Kp = 100, Ki =0, Kd = 0;                    //pid弯道参数参数   
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;//pid直道参数 
//float decide = 0;                                   //元素判断
float previous_error = 0, previous_I = 0;           //误差值 

extern uint16 CPWM[MOTOR_NUM];
int offset;

#define D1  PCin(8)
#define D2  PCin(9)
#define D3  PAin(14)
//#define D4  PCin(10)
//#define D5  PCin(11)

void UART3_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB, ENABLE);

    // ??USART Tx (PB.10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ??USART Rx (PB.11)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // USART3
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
		
}

//void Uart3_NVIC_Init(void)
//{
//	  //Usart3 NVIC 配置
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//shi neng chuang ko zhong duan
//}
int  openmv[4];
void USART3_IRQHandler(void) {
    static int i=0;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        openmv[i++] = USART_ReceiveData(USART3);
        if (openmv[0]!=0xa3) i=0;
					if((i==2)&&(openmv[1]!=0xb3)) i=0;
			if(i==4)
			{
				if(openmv[3]==0xc3)
				{offset=openmv[2];
				USART_SendData(USART3,offset);
        } 
    }
}
}

int readoffset(void)
{
	return offset;
}
void Traversing_GPIO_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void Auto_Traversing(void)
{
	//distance = GetDistance();
	//if(distance<=180)
	//{
		//Set_Pwm_Motor1(0);
		//Set_Pwm_Motor2(0);
	//}
	//else
	{
		trace();//寻迹模块传回数据判断
		calc_pid();//pid 参数整定
		motor_direction(); //舵机方向控制
	}
	
	

}

/**************************************************************************
函数功能：判断路况是那种情况
入口参数：无
返回  值：无
**************************************************************************/
void trace(void)
{	
	if(D1==1 && D2==0 &&D3==1)
	{
		  error =0;	
			Set_Pwm_Motor1(MPWM);
		  Set_Pwm_Motor2(MPWM);
	}
	if(D1==1 && D2==1&&D3==0)
	{
		  error =0;	
			Set_Pwm_Motor1(MPWM);
		  Set_Pwm_Motor2(MPWM);
	}
   if (D1==1 && D2==1 && D3 ==1) //左小转
		{
			error =1;	
			Set_Pwm_Motor1(MIPWM);
		  Set_Pwm_Motor2(MPWM);
			Delay_ms(600);
		}
	 if (D1==0 && D2==0) //右小转
		{
			error =-1;
			Set_Pwm_Motor1(MPWM);
		  Set_Pwm_Motor2(MIPWM);
      Delay_ms(600);
		}
	}
		
/**************************************************************************
函数功能：该算法是针对舵机方向进行参数整定的
入口参数：无
返回  值：无
**************************************************************************/

/********
左转1100 - 1300  ERROR= 1   1500-400=1100  1500-400*2=700 
右转1900-2300  ERRORR=-1   1500-(-400)=1900  1500-(-400*2)=2300
*********/

/********
左转1100 - 1300  ERROR= 1   1500-300=1200  1500-300*2=900 
右转1900-2300  ERRORR=-1   1500-(-300)=1800  1500-(-300*2)=2100
*********/

void calc_pid()//                    //pid弯道参数参数   Kp = 400, Ki = 0, Kd = 0 比例粗调，积分小的增加输入量，减少输入误差，微分是负数防止过调
{
  P = error;
  I = I + error;
  D = error - previous_error;
 
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
 
  previous_error = error;
}
/**************************************************************************
函数功能：对判断出的路况舵机进行反应，速度进行反应
入口参数：direction 控制舵机方向  MPWM 控制速度 -999-+999  舵机角度计算 1500 居中  每400等于调45度 比如说 1100 左偏45
返回  值：无
**************************************************************************/
void motor_direction(void)
{
			// Set_Pwm_Motor1(MPWM);
		  // Set_Pwm_Motor2(MPWM);
		   Zx_MotorRunS(1, ZMRPM);  
		   Zx_MotorRunN(2, ZMRPM);
			 CPWM[1]=1500-PID_value;
	   // UART_Put_Inf("p:",CPWM[1]);

}
