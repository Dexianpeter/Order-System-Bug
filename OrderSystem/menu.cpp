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
int GetItemPrice(int ID, ITEM_TYPE t, MENU* menu);
void PrintMenu(const MENU* menu);

MENU* allocmenu() {
	MENU* rt = (MENU*)calloc(1, sizeof(MENU));
	rt->food = (MENU_ITEM*)calloc(MAXITEMNUM, sizeof(MENU_ITEM));
	for (int i = 0; i < MAXITEMNUM; i++) {
		(rt->food + i)->name = (char*)calloc(MAX_NAME, sizeof(char));
		(rt->food + i)->price = -1;
	}
	rt->drink = (MENU_ITEM*)calloc(MAXITEMNUM, sizeof(MENU_ITEM));
	for (int i = 0; i < MAXITEMNUM; i++) {
		(rt->drink + i)->name = (char*)calloc(MAX_NAME, sizeof(char));
		(rt->drink + i)->price = -1;
	}
	rt->other = (MENU_ITEM*)calloc(MAXITEMNUM, sizeof(MENU_ITEM));
	for (int i = 0; i < MAXITEMNUM; i++) {
		(rt->other + i)->name = (char*)calloc(MAX_NAME, sizeof(char));
		(rt->other + i)->price = -1;
	}
	return rt;
}
void freemenu(MENU* menu) {
	for (int i = 0; i < MAXITEMNUM; i++) {
		free((menu->food + i)->name);
		free((menu->drink + i)->name);
		free((menu->other + i)->name);
	}
	free(menu->food);
	free(menu->drink);
	free(menu->other);
	free(menu);
}

int ReadDrinkMenu(MENU* menu) {
	FILE *f = fopen(DRINK_PATH, "r");
	if (!f) {
		printf("ERROR: drink.txt cannot open\n");
		return -1;
	}
	else {
		int no, pricetmp;
		char nametmp[MAX_NAME], in;
		while (fscanf(f, "%d %s %d %c\n", &no, nametmp, &pricetmp, &in) == 4 && no <= MAXITEMNUM) {
			menu->drink[no - 1].id = no - 1;
			menu->drink[no - 1].price = pricetmp;
			menu->drink[no - 1].tmp = in;
			menu->drink[no - 1].type = ITEM_DRINK;
			strcpy((menu->drink + no - 1)->name, nametmp);
		}
		fclose(f);
		return 1;
	}
}
int ReadFoodMenu(MENU* menu) {
	FILE *f = fopen(FOOD_PATH, "r");
	if (!f) {
		printf("ERROR: food.txt cannot open\n");
		return -1;
	}
	else {
		int no, pricetmp;
		char nametmp[MAX_NAME];
		while (fscanf(f, "%d %s %d\n", &no, nametmp, &pricetmp) == 3 && no <= MAXITEMNUM) {
			menu->food[no - 1].id = no - 1;
			menu->food[no - 1].price = pricetmp;
			menu->food[no - 1].type = ITEM_FOOD;
			strcpy((menu->food + no - 1)->name, nametmp);
		}
		fclose(f);
		return 1;
	}
	/*menu->food[0].id = 0;
	strcpy((menu->food[0]).name, "atestfood");
	menu->food[0].price = 10;
	menu->food[0].tmp = 0;
	menu->food[0].type = ITEM_FOOD;

	menu->food[1].id = 1;
	strcpy((menu->food[1]).name, "btestfood");
	menu->food[1].price = 20;
	menu->food[1].tmp = 0;
	menu->food[1].type = ITEM_FOOD;*/
	return 1;
}
int ReadOtherMenu(MENU* menu) {
	FILE *f = fopen(OTHER_PATH, "r");
	if (!f) {
		printf("ERROR: other.txt cannot open\n");
		return -1;
	}
	else {
		int no, pricetmp;
		char nametmp[MAX_NAME];
		while (fscanf(f, "%d %s %d\n", &no, nametmp, &pricetmp) == 3 && no <= MAXITEMNUM) {
			menu->other[no - 1].id = no - 1;
			menu->other[no - 1].price = pricetmp;
			menu->other[no - 1].type = ITEM_OTHER;
			strcpy((menu->other + no - 1)->name, nametmp);
		}
		fclose(f);
		return 1;
	}
}

int ReadMenu(MENU* menu) {
	if (ReadDrinkMenu(menu) &&
		ReadFoodMenu(menu) &&
		ReadOtherMenu(menu))return 1;
	else return 0;
}
int GetItemPrice(int ID, ITEM_TYPE t, MENU* menu) {
	switch (t) {
	case ITEM_DRINK:
		return (menu->drink[ID].price);
	case ITEM_FOOD:
		return (menu->food[ID].price);
	case ITEM_OTHER:
		return (menu->other[ID].price);
	default:
		printf("GetItemPrice() fuction error\n");
		return -1;
	}
}
void PrintMenu(const MENU *menu) {
	puts("=============•D¿\=============");
	printf("%-*s%-*s %-*s\n", NO_W, "NO",
		NAME_W, "NAME",
		PRICE_W, "PRICE");

	for (int i = 0; i < MAXITEMNUM && menu->food[i].price != -1; ++i)
		printf("%2d. %-*s%*d$\n",
			i + 1,
			NAME_W, menu->food[i].name,
			PRICE_W, menu->food[i].price);

	puts("=============∂ºÆ∆=============");
	printf("%-*s%-*s %-*s\n", NO_W, "NO",
		NAME_W, "NAME",
		PRICE_W, "PRICE");

	for (int i = 0; i < MAXITEMNUM && menu->drink[i].price != -1; ++i)
		printf("%2d. %-*s%*d$\n",
			i + 1,
			NAME_W, menu->drink[i].name,
			PRICE_W, menu->drink[i].price);

	puts("=============∞t¿\=============");
	printf("%-*s%-*s %-*s\n", NO_W, "NO",
		NAME_W, "NAME",
		PRICE_W, "PRICE");

	for (int i = 0; i < MAXITEMNUM && menu->other[i].price != -1; ++i)
		printf("%2d. %-*s%*d$\n",
			i + 1,
			NAME_W, menu->other[i].name,
			PRICE_W, menu->other[i].price);
}

#endif