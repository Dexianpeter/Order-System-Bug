//BY NCKU CE 117 HSIAO,TE-HSIEN
#define _CRT_SECURE_NO_WARNINGS
#ifndef __MENU_H__
#define __MENU_H__
#define MAX_NAME 100
#define MAXITEMNUM 20
#define NO_W     4 
#define NAME_W   20 
#define PRICE_W  5
#define DRINK_PATH "drink.txt" 
#define FOOD_PATH "food.txt" 
#define OTHER_PATH "other.txt" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum  _ITEM_TYPE {
	ITEM_FOOD,
	ITEM_DRINK,
	ITEM_OTHER
} ITEM_TYPE;

typedef struct {
	int id;
	char* name;
	int price;
	ITEM_TYPE type;
	char tmp; // 0=cold, 1=hot
} MENU_ITEM;

typedef struct MENU {
	MENU_ITEM* food;
	MENU_ITEM* drink;
	MENU_ITEM* other;
}MENU;

MENU* allocmenu();
void freemenu(MENU* menu);

int ReadDrinkMenu(MENU* menu);
int ReadFoodMenu(MENU* menu);
int ReadOtherMenu(MENU* menu);
int ReadMenu(MENU* menu);
int GetItemPrice(int ID, ITEM_TYPE t, const MENU* menu);
void PrintMenu(const MENU* menu);
#endif

