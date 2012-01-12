#include "test.h"
#include "timer.h" 
/******************************************************************************/
extern u16 waitmessagebacktime;
extern u16 gprssendmessagetiming; 
extern u8 GPRSSTATE;
/******************************************************************************/	 
/****************************************************************
功能：通用定时器2中断初始化函数
参数：arr-自动重装数值；psc-时钟预分频数
返回：NULL
说明：Tout=(arr*(psc+1))/Tclk 
      Tclk时钟为72M(APB1的2倍)，psc一般为7199
****************************************************************/
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
/****************************************************************
功能：定时器2中断函数
参数：
返回：
说明：0.1s定时
****************************************************************/
void TIM2_IRQHandler(void)				   
{ 	  		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		waitmessagebacktime++;	 	    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
/****************************************************************
功能：通用定时器中断初始化
参数：arr：自动重装值；psc：时钟预分频数
返回：
说明：这里时钟选择为APB1的2倍，而APB1为36M
****************************************************************/
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
/****************************************************************
功能：定时器中断函数
参数：
返回：
说明：
****************************************************************/
void TIM3_IRQHandler(void)				   //0.5s定时
{ 	  		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		gprssendmessagetiming++;
		if((gprssendmessagetiming==120)||(gprssendmessagetiming==240)||(gprssendmessagetiming==360)||(gprssendmessagetiming==480))    //120*0.5=60s	
		{
			GPRSSTATE=GPRS_HEARTBEAT;
		}  
		if(gprssendmessagetiming>=600)
		{
			GPRSSTATE=GPRS_HEARTBEAT;
			gprssendmessagetiming=0;
		}  				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
