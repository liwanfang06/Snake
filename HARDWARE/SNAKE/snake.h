#ifndef SNAKE__H
#define SNAKE__H
#include "sys.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "flash.h"
#include "FreeRTOS.h"
#include "task.h"
#include "key.h" 
#include "math.h"
#include "rng.h"
#include "beep.h"
#include "bmp.h"
#include "malloc.h"
#include "sram.h"
enum POS{
	LEFT=1,
	RIGHT,
	UP,
	DOWN
};
//Éß½á¹¹Ìå
typedef struct SnakeNode{
	int x;
	int y;
	u16 color;
	struct 	SnakeNode *next,*prev;
}SNAKE_t,*SNAKELIST;

void Game_Start(void);
void SNAKE_Init(void);
void Game_End(void);
#endif

