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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	//TIM_DeInit(TIM1);											/* ����������ʱ�� */

	TIM_TimeBaseStructure.TIM_Period=(65535 - 1);		 				/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	TIM_TimeBaseStructure.TIM_Prescaler=71;						/* ʱ��Ԥ��Ƶ�� 72M/72 */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* �ⲿʱ�Ӳ�����Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* ���ϼ���ģʽ */
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);						/* �������жϱ�־ */

//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					/* �����жϴ���*/    

	TIM_Cmd(TIM1,ENABLE);									    /* �򿪶�ʱ�� */ 

	CheckUltrasonic();
}



void CheckUltrasonic(void)
{
	TRIG = 1;		//�����ź�
	Delay_us(10);	//
	TRIG = 0;
	while(!ECHO);	//�ȴ������źű��
	TIM1->CNT = 0;
	while(ECHO);
	{
		if(TIM1->CNT > 65000)
		{//�Ѿ�����������ⷶΧ����Ϊ������ģ�鲻����
			fUltrasonic = FALSE;
			return;
		}
	}
}

u16 GetDistance(void)
{//��λmm
	//��������෶Χ0.02~4��
	unsigned int distance;
	if(fUltrasonic)
	{
		TRIG = 1;		//�����ź�
		Delay_us(10);	//
		TRIG = 0;
		while(!ECHO);	//�ȴ������źű��
		TIM1->CNT = 0;
		while(ECHO);
		{
			if(TIM1->CNT > 25000)
			{
				return 65535; //�����ض�ֵ
			}

		}
		distance = TIM1->CNT;//�����ж��ٸ�us
		//������Զ4���� distance = 23529
		
		//����340m/S = 0.34mm/us
		//��Ϊ��õ�ʱ��Ϊһ��һ�ص�ʱ�䣬���Բ�õ�ʵ�ʾ��� = ���� * ʱ�� / 2
		//������(mm) = 0.34mm/us * ʱ��(us) / 2
		// = 17mm/us / 100
		distance = distance * 17 / 100;

		return distance;
	}
	else
	{
		return 0;
	}
}
u16 getMeanDistance(u8 mean,u16 DIST){        //meanΪ������,dist��������
 
  u16 sum,minDistance,maxDistance,distance;
  u8 i,num;
	
	if(mean<3)  //��С������С��3ʱ����0
		return 0;   
	
  maxDistance=sum=0;
  minDistance=~0;
  num=mean;
  for(i=0;i<mean;i++){
    distance=GetDistance();
    if(distance>DIST){                        //ȥ��������Χ�ľ���ֵ
      return 0;
      }
      if(distance>maxDistance){               //��ȡ����������ֵ
      maxDistance=distance;
    }
    if(distance<minDistance){
      minDistance=distance;                   //��ȡ������С����ֵ
    }
    sum+=distance;                            //�ۼ�
  }
  return ((sum-minDistance-maxDistance)/(mean-2));//���ؼ�ȥ���ֵ����Сֵ��ƽ����Χ
}
