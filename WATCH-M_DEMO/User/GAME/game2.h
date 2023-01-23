#ifndef __GAME2_H
#define __GAME2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


#define MODE_POINT 		0
#define MODE_BLOCK 		1

#define MODE			MODE_BLOCK

#define Food_Reset		0
#define Food_Set		1

#if (MODE == MODE_POINT)
	#define Y_MAX			64
	#define X_MAX			128
#else
	#define Y_MAX			8
	#define X_MAX			16
#endif

typedef struct Snake
{
	uint16_t x;
	uint16_t y;
}Snake;

typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
}Detect;

void Init_Snake(void);

void Draw_Food(void);
void Set_Food(void);

void Clear_Snake_Tail(void);
void Draw_Snake_Body(uint8_t Parament);
void Draw_Snake(void);



void TCS_Game(void);




#endif /* __GAME2_H */
