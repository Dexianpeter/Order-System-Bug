//BY NCKU CE 117 HSIAO,TE-HSIEN
#define _CRT_SECURE_NO_WARNINGS
#ifndef __ORDER_H__
#define __ORDER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#define MAXUSERNUM 10
#define MAXCARTITEMNUM 30
#define MAXONCEORDERNUM 100
typedef struct _USER {
	int uid;
	char* name;
}USER;
typedef struct _CARTLINE {
	MENU_ITEM* item;
	int qty;
	_CARTLINE* Next;
	_CARTLINE* Prev;
}CARTLINE;
typedef struct _CART {
	USER* user;
	CARTLINE* InitCartLine;
	int cost;
	int cap;
}CART;
typedef enum _OPERATE {
	Add,
	Del
}OPERATE;

void UserOrder(int UID, CART* cart, const MENU* menu);
void GetUserName(int UID, CART* cart);
void PrintOrderCommand();
int ProcessOrder(int UID, CART* cart, const MENU* menu);
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, const MENU* menu);
void DeleteCartLine(int ID, ITEM_TYPE t, CART* cart);
void PrintDelCommand();
void PrintCart(int UID, CART* cart);
int CheckLeave();

CART* allocCart();
void freeCart(CART* c);
CARTLINE* allocCartLine(CARTLINE* prev);
CARTLINE* CreateInitCartLine();
void freeCartLine(CARTLINE* c);
int ReadNum(OPERATE o, int lowerbound, int heigherbound);
#endif
