#ifndef __MY_MUNE_H
#define __MY_MUNE_H


#include "main.h"
/*----Ŀ¼�ڵ�----*/
extern Menu_t *Time_Menu;
extern Menu_t *Dialogo;
/*----------------*/

/*----���ƾ��----*/
extern Menu_t *Tool;
extern Menu_t *Game;
extern Menu_t *Tcs;
extern Menu_t *Temper;
extern Menu_t *Mpu6050;
extern Menu_t *Back;
/*----------------*/

extern Menu_t *Test_1;
extern Menu_t *Test_2;
extern Menu_t *Test_3;
extern Menu_t *Test_4;


extern Menu_t *Game;
extern Menu_t *Tcs;
extern Menu_t *Game2;
extern Menu_t *Game3;
extern Menu_t *Game4;
extern Menu_t *Game5;
extern Menu_t *Game6;
extern Menu_t *Game7;
extern Menu_t *Game8;
extern Menu_t *Game9;

/*----���ܽڵ�----*/
extern Menu_t *Pre_SelectMune;
extern Menu_t *CurrentMune;
extern Menu_t *SelectMune;
/*----------------*/

extern uint32_t Time_Count;
extern uint32_t Time_Count_Bak;

void Mune_Init(void);
void MUNE_RETURN(void);				//�����ϼ��˵�
void MUNE_INTO(void);				//�����¼��˵�
void WATCH_MUNE(void);

void MAIN_MUNE_DISPLAY(void);
void GAME_MUNE_DISPLAY(void);
void SETTING_MUNE_DISPLAY(void);


void VoidFunction(void);			//�ս���
void ComeBack(void);				//���غ���

#endif /* __MY_MUNE_H */
