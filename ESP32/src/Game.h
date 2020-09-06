#ifndef __GAME_H
#define __GAME_H

#include "GUI_Paint.h"
#include "DEV_Config.h"

#define HEIGHT 40
#define WIDTH 20


typedef struct SnakeNode
{
  int x;
  int y;
  
  struct SnakeNode *prev,*next;

}Snake_t;

typedef struct Food
{
  int x;
  int y;
}Food_t;



extern Snake_t SnakeHand;
extern Food_t Food;
extern unsigned char GameMap[HEIGHT][WIDTH];

void GAME_SnakeTest(void);
void GAME_SnakeFillInGameMap(unsigned char (*gamemap)[WIDTH],Snake_t *snake);
void GAME_SnakePixel(int x,int y,UWORD col);
void GAME_NewFood(unsigned char (*gamemap)[WIDTH],Food_t *food);
void GAME_SnakeAddNode(Snake_t *snake,int x,int y);
void GAME_NewSnake(Snake_t *snake);
void GAME_SnakeRun(unsigned char (*gamemap)[WIDTH],Snake_t *snake);
void GAME_SnakeFreeNode(Snake_t *snake);
void GAME_SnakeInit(Snake_t *snake,unsigned char (*gamemap)[WIDTH]);
int GAME_Over(int n);

unsigned char GAME_SnakeMove(unsigned char (*gamemap)[WIDTH],Snake_t *snake,int dir) ;
#endif
