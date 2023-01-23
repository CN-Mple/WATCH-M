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
	//����ӽڵ����
	if(Menu->Menu_Next_Item != NULL)
	{
		Pu_Menu = Menu->Menu_Next_Item;
		//���ֻ���ӽڵ� ��ֱ��������� ID + 1
		if(Pu_Menu->Menu_Equal_Next_Item == Menu->Menu_Next_Item)
		{
			Pu_Menu->Menu_Equal_Next_Item = New_Menu;
			Pu_Menu->Menu_Equal_Previous_Item = New_Menu;
			
			New_Menu->Menu_Equal_Previous_Item = Pu_Menu;
			New_Menu->Menu_Equal_Next_Item = Pu_Menu;
			
			New_Menu->ID = Menu->Menu_Next_Item->ID + 1;
		}
		//�������ֻ���ӽڵ� �������Ѱβ�ڵ� ֱ��������� ID + 1
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
	//����ӽڵ㲻���� �򴴽��ӽڵ� ID����Ϊ1 ���һػ�
	else
	{
		Menu->Menu_Next_Item = New_Menu;

		New_Menu->Menu_Equal_Previous_Item = New_Menu;
		New_Menu->Menu_Equal_Next_Item = New_Menu;
		
		New_Menu->ID = 1;
	}
	//ͳһ���趨�ϲ�ڵ� �ӽڵ�� 
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



//�����¼��˵��б���
Menu_t * Create_NextMenu( Menu_t *Menu,char * Name, void (*Funcation)(void))
{
	Menu_t *New_Menu;
	uint8_t x;

	//����ӽڵ����
	if(Menu->Menu_Next_Item != NULL)
	{
		printf("Menu_Next_Item had created\r\n");
		return NULL;
	}
	//����ӽڵ㲻���� �򴴽��ӽڵ� ID����Ϊ1 ���һػ�
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
	//ͳһ���趨�ϲ�ڵ� �ӽڵ�� 
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

//����ͬ���˵��б���
Menu_t * Create_Equal_NextMenu( Menu_t *Menu,char * Name, void (*Funcation)(void))
{
	Menu_t *New_Menu;
	uint8_t x;

	//����ڵ�Ϊ��
	if(Menu == NULL)
	{
		printf("Menu is NULL\r\n");
		return NULL;
	}
	//����ڵ����
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
	//ͳһ���趨�ϲ�ڵ� �ӽڵ�� 
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

