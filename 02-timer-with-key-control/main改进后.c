#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "InternalTimer.h"
#include "Key.h"

volatile uint16_t Num;			//定义在定时器中断里自增的变量
uint16_t LastNum = 0;
uint8_t KeyNum;
int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Timer_Internal_Init();		//定时中断初始化
	Key_Init();
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "Num:");			//1行1列显示字符串Num:
	
	
	while (1)
	{   
		if (Num != LastNum) 
		{
			OLED_ShowNum(1, 5, Num, 5);			//不断刷新显示Num变量
		    LastNum = Num;
		}	
	}
}

/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		KeyNum = Key_GetNum();		//获取按键键码
		if (KeyNum == 2)
		{
		  Num ++;												//Num变量自增，用于测试定时中断
		  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
		}																		//中断标志位必须清															//否则中断将连续不断地触发，导致主程序卡死
	}
}
