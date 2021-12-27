#include "snake.h"
int main(void)
{ 
	START:
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	FSMC_SRAM_Init();//外部SRAM初始化
	my_mem_init(SRAMIN);//初始化内部内存池
	my_mem_init(SRAMEX);//初始化外部内存池
	my_mem_init(SRAMCCM);//初始化CCM内存池
	LED_Init();		        	//初始化LED端口
	LCD_Init();							//初始化LCD
	KEY_Init();							//初始化按键
	RNG_Init();							//初始化随机数发生器
	BEEP_Init();						//初始化蜂鸣器
	SNAKE_Init();  
	Game_Start();
	Game_End();
	while(KEY_Scan(0)!= WKUP_PRES);
	goto START;
}



