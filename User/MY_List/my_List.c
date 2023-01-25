#include "MY_List.h"



Menu_t * Create_RootMenu(char *Name, void (*Funcation)(void))
{
	Menu_t * Menu;
	uint8_t x;
	
	Menu = (Menu_t *)malloc(sizeof(Menu_t));
	
	Menu->ID = 1;
	for(x = 0; x < Name_Max; x++)
	{
		Menu->Name[x] = Name[x];
		if(Menu->Name[x] == 0)
		{
			break;
		}
	}
	Menu->Name[Name_Max - 1] = '\0';

	Menu->Menu_Previous_Item = NULL;
	Menu->Menu_Next_Item = NULL;
	
	Menu->Menu_Equal_Previous_Item = NULL;
	Menu->Menu_Equal_Next_Item = NULL;
	
	Menu->Funcation = Funcation;
	
	return Menu;
}

Menu_t * Create_Menu( Menu_t *Menu,char * Name, void (*Funcation)(void))
{
	Menu_t *New_Menu, *Pu_Menu;
	uint8_t x;
	
	New_Menu = (Menu_t *)malloc(sizeof(Menu_t));
	
	if(New_Menu != NULL)
	{
		printf("Ture\r\n");
	}
	else
	{
		printf("Error\r\n");
		free(New_Menu);
		return NULL;
	}
	//如果子节点存在
	if(Menu->Menu_Next_Item != NULL)
	{
		Pu_Menu = Menu->Menu_Next_Item;
		//如果只有子节点 则直接填入其后 ID + 1
		if(Pu_Menu->Menu_Equal_Next_Item == Menu->Menu_Next_Item)
		{
			Pu_Menu->Menu_Equal_Next_Item = New_Menu;
			Pu_Menu->Menu_Equal_Previous_Item = New_Menu;
			
			New_Menu->Menu_Equal_Previous_Item = Pu_Menu;
			New_Menu->Menu_Equal_Next_Item = Pu_Menu;
			
			New_Menu->ID = Menu->Menu_Next_Item->ID + 1;
		}
		//如果不仅只有子节点 则遍历找寻尾节点 直接填入其后 ID + 1
		else
		{
			while( Pu_Menu->Menu_Equal_Next_Item != Menu->Menu_Next_Item )
			{
				Pu_Menu = Pu_Menu->Menu_Equal_Next_Item;
			}
			Pu_Menu->Menu_Equal_Next_Item = New_Menu;
			New_Menu->Menu_Equal_Previous_Item = Pu_Menu;
			
			New_Menu->Menu_Equal_Next_Item = Menu->Menu_Next_Item;
			Menu->Menu_Next_Item->Menu_Equal_Previous_Item = New_Menu;
			
			New_Menu->ID = Pu_Menu->ID + 1;
		}
	}
	//如果子节点不存在 则创建子节点 ID设置为1 并且回环
	else
	{
		Menu->Menu_Next_Item = New_Menu;

		New_Menu->Menu_Equal_Previous_Item = New_Menu;
		New_Menu->Menu_Equal_Next_Item = New_Menu;
		
		New_Menu->ID = 1;
	}
	//统一：设定上层节点 子节点空 
	New_Menu->Menu_Previous_Item = Menu;
	New_Menu->Menu_Next_Item = NULL;	
	
	for(x = 0; x < Name_Max; x++)
	{
		New_Menu->Name[x] = Name[x];
		if(New_Menu->Name[x] == 0)
		{
			break;
		}
	}
	New_Menu->Name[Name_Max - 1] = '\0';
	New_Menu->Funcation = Funcation;
	
	return New_Menu;
}



//创建下级菜单列表项
Menu_t * Create_NextMenu( Menu_t *Menu,char * Name, void (*Funcation)(void))
{
	Menu_t *New_Menu;
	uint8_t x;

	//如果子节点存在
	if(Menu->Menu_Next_Item != NULL)
	{
		printf("Menu_Next_Item had created\r\n");
		return NULL;
	}
	//如果子节点不存在 则创建子节点 ID设置为1 并且回环
	else
	{
		New_Menu = (Menu_t *)malloc(sizeof(Menu_t));
	
		if(New_Menu != NULL)
		{
			printf("creat! \r\n");
		}
		else
		{
			printf("ram error\r\n");
			free(New_Menu);
			return NULL;
		}
		Menu->Menu_Next_Item = New_Menu;

		New_Menu->Menu_Equal_Previous_Item = NULL;
		New_Menu->Menu_Equal_Next_Item = NULL;
		
		New_Menu->ID = 1;
	}
	//统一：设定上层节点 子节点空 
	New_Menu->Menu_Previous_Item = Menu;
	New_Menu->Menu_Next_Item = NULL;	
	
	for(x = 0; x < Name_Max; x++)
	{
		New_Menu->Name[x] = Name[x];
		if(New_Menu->Name[x] == 0)
		{
			break;
		}
	}
	New_Menu->Name[Name_Max - 1] = '\0';
	New_Menu->Funcation = Funcation;
	
	return New_Menu;
}

//创建同级菜单列表项
Menu_t * Create_Equal_NextMenu( Menu_t *Menu,char * Name, void (*Funcation)(void))
{
	Menu_t *New_Menu;
	uint8_t x;

	//如果节点为空
	if(Menu == NULL)
	{
		printf("Menu is NULL\r\n");
		return NULL;
	}
	//如果节点存在
	else
	{
		New_Menu = (Menu_t *)malloc(sizeof(Menu_t));
	
		if(New_Menu != NULL)
		{
			printf("creat! \r\n");
		}
		else
		{
			printf("ram error\r\n");
			free(New_Menu);
			return NULL;
		}
		Menu->Menu_Equal_Next_Item = New_Menu;

		New_Menu->Menu_Equal_Previous_Item = Menu;
		New_Menu->Menu_Equal_Next_Item = NULL;
		
		New_Menu->ID = Menu->ID + 1;
	}
	//统一：设定上层节点 子节点空 
	New_Menu->Menu_Previous_Item = Menu->Menu_Previous_Item;
	New_Menu->Menu_Next_Item = NULL;	
	
	for(x = 0; x < Name_Max; x++)
	{
		New_Menu->Name[x] = Name[x];
		if(New_Menu->Name[x] == 0)
		{
			break;
		}
	}
	New_Menu->Name[Name_Max - 1] = '\0';
	New_Menu->Funcation = Funcation;
	
	return New_Menu;
}

