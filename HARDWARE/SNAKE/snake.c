#include "snake.h"
#include "lcd.h"
#define FLASH_SAVE_ADDR  0X0800C004 
int up_y=120;
u8 SNAKE_POS=DOWN;
u32 score=0;
u32 max_score=0;
SNAKE_t *Head,food;
u8 isEat=0;//标记食物有没有被吃
//LCD刷屏时使用的颜色
int lcd_discolor[12]={  BLACK, BLUE,  BRED,      
						GRED,  GBLUE,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };
void LCD_ClearNode(SNAKE_t snake)
{
	int i;
	float j=1;
	if(snake.color==RED)j=1.5;//如果是蛇头,节点放大1倍
	POINT_COLOR=WHITE;
	for(i=0;i<5*j;i++)
	{
		LCD_Draw_Circle(snake.x,snake.y,i);
	}
}
void LCD_DrawNode(SNAKE_t snake)
{
	int i;
	float j=1;
	if(snake.color==RED)j=1.5;//如果是蛇头,节点放大1.5倍
	POINT_COLOR=snake.color;
	for(i=0;i<5*j;i++)
	{
		LCD_Draw_Circle(snake.x,snake.y,i);
	}
}
void CreatFood(void)
{
	u8 col=RNG_Get_RandomRange(0,11);
	food.x=RNG_Get_RandomRange(13,469);
	food.y=RNG_Get_RandomRange(up_y+5,789);
	food.color=lcd_discolor[col];
	printf("food : %d %d\r\n",food.x,food.y);
	LCD_DrawNode(food);
}
void SNAKE_Move()
{
	SNAKE_t *h=Head,*n=Head->prev;
	h=Head;
	n=Head->prev;
	h=Head;
	POINT_COLOR=RED;
	//LCD_DrawRectangle(2,up_y+2,480-2,800-2);
	LCD_DrawRectangle(3,up_y+3,480-3,800-3);
	//蛇身向前移动
	while(n!=h){
		n->x=n->prev->x;
		n->y=n->prev->y;
		n=n->prev;
	}
	switch(SNAKE_POS)
	{
		case LEFT:
			h->x -=10;
			break;
		case RIGHT:
			h->x += 10;
			break;
		case UP:
			h->y -= 10;
			break;
		case DOWN:
			h->y += 10;
			break;
	}
	h=Head;
	//LCD_Fill(6,up_y+6,480-6,800-6,WHITE);
	LCD_DrawNode(food);//画食物
	while(h!=NULL){
		LCD_DrawNode(*h);
		h=h->next;
	}
}
void Game_Start(void){
	u8 key=0;
	float dis;
	SNAKE_t *h=Head,*n=Head->prev,*newNode;
	while(1)
	{
		key=KEY_Scan(0);
		h=Head;
		while(h!=NULL){
				LCD_ClearNode(*h);
				h=h->next;
			}
		h=Head;
		if(key!=0)
		{
			h=Head;
			n=Head->prev;
			//蛇身向前移动
			while(n!=h){
				n->x=n->prev->x;
				n->y=n->prev->y;
				n=n->prev;
			}
			switch(key)
			{
				case KEY2_PRES:
					//不能向反方向180度转
					if(SNAKE_POS!=RIGHT){
						SNAKE_POS=LEFT;
						h->x -=10;
					}
					break;
				case KEY0_PRES:
					if(SNAKE_POS!=LEFT){
						SNAKE_POS=RIGHT;
						h->x += 10;
					}
					break;
				case WKUP_PRES:
					if(SNAKE_POS!=DOWN){
						SNAKE_POS=UP;
						h->y -= 10;
					}
					break;
				case KEY1_PRES:
					if(SNAKE_POS!=UP){
						SNAKE_POS=DOWN;
						h->y += 10;
					}
					break;
			}
		}
		SNAKE_Move();
		//吃到食物,添加节点
		 dis=sqrt((h->x - food.x)*(h->x - food.x)+(h->y - food.y)*(h->y - food.y));
		if(dis<2*5){
			newNode=pvPortMalloc(sizeof(SNAKE_t));
			n=Head->prev;
			newNode->x=n->x;
			newNode->y=n->y+10;
			newNode->prev=n;
			newNode->next=NULL;
			newNode->color=food.color;
			n->next=newNode;
			n=n->next;
			h->prev=n;
//			BEEP=1;
//			delay_ms(10);
//			BEEP=0;
			//Head=h;
			score++;
			LCD_ClearNode(food);
			CreatFood();//创建食物
		}
//		printf("food : %d %d\r\n",food.x,food.y);
		//显示分数
		LCD_ShowNum(290+6*12,40,score,5,24);
		//判断是否到达边界,到边界游戏结束
		if(h->x<6 || h->x>474 || h->y<up_y+12 || h->y>794){
			return;
		}
		delay_ms(80);
	}
}
void Game_End(void)
{
		LCD_Fill(0,up_y,480-1,800-1,WHITE);
		POINT_COLOR=BRRED;
		LCD_DrawBMP(150,300,over);
	  POINT_COLOR=BLUE;
		if(score>max_score){
			STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)&score,1);
		}
		LCD_ShowString(220,700,200,48,24,"Press the up key to continue");
		
}
void SNAKE_Init(void)
{
	int i=0;
	SNAKE_t *p;
	POINT_COLOR = BLUE;
	score=max_score=0;
	//STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)&max_score,1);
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)&max_score,1);//读出最大成绩
	printf("max_score :%d\r\n",max_score);
	
	LCD_DrawBMP(20,0,logo1);
	LCD_DrawBMP(120,0,logo);
	LCD_DrawRectangle(290-2,40-2,480-10,40+30);
	LCD_ShowString(290,40,190,24,24,"Score:   0");
	LCD_DrawRectangle(290-2,40+32-2,480-10,40+30+32);
	LCD_ShowString(290,40+32,190,24,24,"Max_s:    0");
	LCD_ShowNum(290+6*12,40+32,max_score,5,24);
	LCD_DrawLine(2,up_y,480-2,up_y);
	
	POINT_COLOR = RED;
	LCD_DrawRectangle(2,up_y+2,480-2,800-2);
	LCD_DrawRectangle(3,up_y+3,480-3,800-3);
	POINT_COLOR=GREEN;
//	Head=pvPortMalloc(sizeof(SNAKE_t));
	Head=mymalloc(SRAMEX,sizeof(SNAKE_t));
	Head->x=140;
	Head->y=up_y+40;
	Head->next=NULL;
	Head->prev=Head;
	Head->color=RED;
	p=Head;
	score++;
	for(i=1;i<2;i++)
	{
//		SNAKE_t *node=pvPortMalloc(sizeof(SNAKE_t));
		SNAKE_t *node=mymalloc(SRAMEX,sizeof(SNAKE_t));
		node->x=140;
		node->y=up_y+40;
		node->prev=p;
		node->next=NULL;
		node->color=lcd_discolor[RNG_Get_RandomRange(0,12)];
		p->next=node;
		p=p->next;
		Head->prev=p;
		score++;
	}
	CreatFood();
	p=Head;
	while(p!=NULL)
	{
		LCD_DrawNode(*p);
		p=p->next;
	}
	LCD_ShowNum(290+6*12,40,score,5,24);
	//while(1);
}
