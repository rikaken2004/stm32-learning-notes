#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "NEW_LED.h"
#include "Key.h"
#include "ExternalTimer.h"
#include "PWM.h"
#include "OLED.h"
volatile uint8_t i;
volatile uint8_t brightness;
uint16_t LastNum = 0;
int main(void)
{
	NEW_LED_Init();		//LED初始化
	OLED_Init();
    PWM_Init(); 
	Timer_External_Init();
	OLED_ShowString(1, 1, "brightness");
	while (1)
	{
		for (i = 0; i <= 100; i++)
	    {
			PWM_SetCompare1(i);			//依次将定时器的CCR寄存器设置为0~100，PWM占空比逐渐增大，LED逐渐变亮
			Delay_ms(100);				//延时10ms
			if ( brightness!= LastNum) 
		    {
			OLED_ShowNum(2, 1, brightness, 3);			//不断刷新显示Num变量
		    LastNum = brightness;
		    }	
		}
		for (i = 100; i <= 100; i--)
		{
			PWM_SetCompare1(i);	//依次将定时器的CCR寄存器设置为100~0，PWM占空比逐渐减小，LED逐渐变暗
			Delay_ms(100);				//延时10ms
			if ( brightness!= LastNum) 
		    {
			OLED_ShowNum(2, 1, brightness, 3);			//不断刷新显示Num变量
		    LastNum = brightness;
		    }	
		}
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		brightness = i;												//Num变量自增，用于测试定时中断
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
