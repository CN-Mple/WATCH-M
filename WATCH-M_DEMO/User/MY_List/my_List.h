#ifndef __MY_LIST_H
#define __MY_LIST_H

#include "stm32f10x.h"
#include "stddef.h"
#include <stdlib.h>
#include "bsp_usart.h"

#define Name_Max		16

#define Equal			0
#define Next			1


typedef struct Menu
{
	uint8_t ID;
	char Name[Name_Max];
	
	struct Menu* Menu_Previous_Item;
	struct Menu* Menu_Equal_Previous_Item;
	struct Menu* Menu_Equal_Next_Item;
	struct Menu* Menu_Next_Item;
	
	void (*Funcation)();
}Menu_t;

typedef	void * Menu_Handle_t;


Menu_t * Create_RootMenu(char *Name, void (*Funcation)(void));
Menu_t * Create_Menu( Menu_t *,char * Name,void (*Funcation)(void));

//创建下级菜单列表项
Menu_t * Create_NextMenu( Menu_t *Menu,char * Name, void (*Funcation)(void));
//创建同级菜单列表项
Menu_t * Create_Equal_NextMenu( Menu_t *Menu,char * Name, void (*Funcation)(void));

#endif /* __MY_LIST_H */
