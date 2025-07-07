//BY NCKU CE 117 HSIAO,TE-HSIEN
#define _CRT_SECURE_NO_WARNINGS
#ifndef __ORDER_H__
#define __ORDER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#define MAXUSERNUM 15
#define MAXCARTITEMNUM 30
typedef struct _USER {
	int uid;
	char* name;
}USER;
typedef struct _CARTLINE {
	int id;// item id
	ITEM_TYPE type;
	int qty;
	char tmp;// 0=cold 1=hot
	int price;
}CARTLINE;
typedef struct _CART {
	CARTLINE* cartline;
	int tmpcost;
}CART;

typedef struct _SUBORDER {
	USER* user;
	CART* cart;
	int personalcost;
}SUBORDER;

typedef struct _ORDER {
	SUBORDER* suborder;
	int allcost;
}ORDER;

ORDER* allocOrder();
void freeOrder(ORDER* o);

void UserOrder(int UID, ORDER* order, MENU* menu);
void GetUserName(int UID, SUBORDER* o);
void PrintCommand();
int ProcessOrder(int UID, CART* cart, MENU* menu);
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, MENU* menu);

void SaveSubOrder(int UID, SUBORDER* SubOrder);
void PrintCart(int UID, CART* cart, MENU* menu);
void SaveOrder(ORDER* order);
#endif
