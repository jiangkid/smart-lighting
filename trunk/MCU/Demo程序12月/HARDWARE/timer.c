#include "test.h"
#include "timer.h" 
#include "spi.h"
//定时器3中断服务程序
extern u16 time3count;
extern u8 time2count;
extern u8 time3_flag;	 
/*********************************************************************/
void TIM3_IRQHandler(void)				   //0.5s定时
{ 	  		  			    
	if(TIM3->SR&0X0001)//溢出中断
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
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能    
 	TIM3->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->DIER|=1<<6;   //允许触发中断	   
//	TIM3->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//抢占1，子优先级3，组2									 
}
/*********************************************************************/
void TIM2_IRQHandler(void)				   //0.5s定时
{ 	  		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		time2count++;	    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
/*************************************************************/
void Timer2_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;//TIM2时钟使能    
 	TIM2->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM2->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM2->DIER|=1<<0;   //允许更新中断				
	TIM2->DIER|=1<<6;   //允许触发中断	   
//	TIM2->CR1|=0x01;    //使能定时器2
  	MY_NVIC_Init(1,3,TIM2_IRQChannel,2);//抢占1，子优先级3，组2									 
}
