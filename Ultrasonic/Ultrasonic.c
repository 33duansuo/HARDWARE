#include "Ultrasonic.h"
#include "delay.h"
#include "sys.h"
bool fUltrasonic = TRUE;

#define TRIG 		PCout(6)
#define ECHO		PCin(7)

void InitUltrasonic(void)		//@12.000MHz
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	//TIM_DeInit(TIM1);											/* 重新启动定时器 */

	TIM_TimeBaseStructure.TIM_Period=(65535 - 1);		 				/* 自动重装载寄存器周期的值(计数值) */
	TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);						/* 清除溢出中断标志 */

//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					/* 开启中断触发*/    

	TIM_Cmd(TIM1,ENABLE);									    /* 打开定时器 */ 

	CheckUltrasonic();
}



void CheckUltrasonic(void)
{
	TRIG = 1;		//触发信号
	Delay_us(10);	//
	TRIG = 0;
	while(!ECHO);	//等待回响信号变高
	TIM1->CNT = 0;
	while(ECHO);
	{
		if(TIM1->CNT > 65000)
		{//已经超过了最大检测范围，认为超声波模块不存在
			fUltrasonic = FALSE;
			return;
		}
	}
}

u16 GetDistance(void)
{//单位mm
	//超声波测距范围0.02~4米
	unsigned int distance;
	if(fUltrasonic)
	{
		TRIG = 1;		//触发信号
		Delay_us(10);	//
		TRIG = 0;
		while(!ECHO);	//等待回响信号变高
		TIM1->CNT = 0;
		while(ECHO);
		{
			if(TIM1->CNT > 25000)
			{
				return 65535; //返回特定值
			}

		}
		distance = TIM1->CNT;//计算有多少个us
		//按照最远4米算 distance = 23529
		
		//声速340m/S = 0.34mm/us
		//因为测得的时间为一来一回的时间，所以测得的实际距离 = 声速 * 时间 / 2
		//即距离(mm) = 0.34mm/us * 时间(us) / 2
		// = 17mm/us / 100
		distance = distance * 17 / 100;

		return distance;
	}
	else
	{
		return 0;
	}
}
u16 getMeanDistance(u8 mean,u16 DIST){        //mean为检测次数,dist最大检测距离
 
  u16 sum,minDistance,maxDistance,distance;
  u8 i,num;
	
	if(mean<3)  //最小检测次数小于3时返回0
		return 0;   
	
  maxDistance=sum=0;
  minDistance=~0;
  num=mean;
  for(i=0;i<mean;i++){
    distance=GetDistance();
    if(distance>DIST){                        //去除超出范围的距离值
      return 0;
      }
      if(distance>maxDistance){               //获取检测的最大距离值
      maxDistance=distance;
    }
    if(distance<minDistance){
      minDistance=distance;                   //获取检测的最小距离值
    }
    sum+=distance;                            //累加
  }
  return ((sum-minDistance-maxDistance)/(mean-2));//返回减去最大值和最小值的平均范围
}
