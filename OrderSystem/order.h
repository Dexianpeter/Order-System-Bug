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
	MENU_ITEM* item;
	int qty;
}CARTLINE;
typedef struct _CART {
	USER* user;
	CARTLINE* cartline;
	int personalcost;
}CART;

typedef struct _ORDER {
	CART* cart;
	int allcost;
}ORDER;

ORDER* allocOrder();
void freeOrder(ORDER* o);

void UserOrder(int UID, ORDER* order, const MENU* menu);
void GetUserName(int UID, CART* cart);
void PrintOrderCommand();
int ProcessOrder(int UID, CART* cart, const MENU* menu);
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, const MENU* menu);

void SaveCart(int UID, CART* cart);
void PrintCart(int UID, CART* cart);
void SaveOrder(ORDER* order);
#endif
