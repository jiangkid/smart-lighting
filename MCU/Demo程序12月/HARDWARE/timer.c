#include "test.h"
#include "timer.h" 
#include "spi.h"
//��ʱ��3�жϷ������
extern u16 time3count;
extern u8 time2count;
extern u8 time3_flag;	 
/*********************************************************************/
void TIM3_IRQHandler(void)				   //0.5s��ʱ
{ 	  		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		time3count++;
		if((time3count==60)||(time3count==120)||(time3count==180)||(time3count==240)||(time3count==300)||(time3count==360)||(time3count==420)||(time3count==480)||(time3count==540))    //60*0.5=30s	
		{
			time3_flag=1;
		}  
		if(time3count>=600)
		{
			time3_flag=2;	
			time3count=0;
		}  				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->DIER|=1<<6;   //�������ж�	   
//	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//��ռ1�������ȼ�3����2									 
}
/*********************************************************************/
void TIM2_IRQHandler(void)				   //0.5s��ʱ
{ 	  		  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		time2count++;	    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}
/*************************************************************/
void Timer2_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;//TIM2ʱ��ʹ��    
 	TIM2->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM2->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM2->DIER|=1<<0;   //��������ж�				
	TIM2->DIER|=1<<6;   //�������ж�	   
//	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
  	MY_NVIC_Init(1,3,TIM2_IRQChannel,2);//��ռ1�������ȼ�3����2									 
}
