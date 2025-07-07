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
	int cap;
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

ORDER* allocOrder() {
	ORDER* rt = (ORDER*)calloc(1, sizeof(ORDER));
	rt->cart = (CART*)calloc(MAXUSERNUM, sizeof(CART));
	for (int i = 0; i < MAXUSERNUM; i++) {
		rt->cart[i].user = (USER*)calloc(1, sizeof(USER));
		rt->cart[i].user->name = (char*)calloc(MAX_NAME, sizeof(char));
		rt->cart[i].cartline = (CARTLINE*)calloc(MAXCARTITEMNUM, sizeof(CARTLINE));
		for (int j = 0; j < MAXCARTITEMNUM; j++) {
			rt->cart[i].cartline[j].item = (MENU_ITEM*)calloc(1, sizeof(MENU_ITEM));
			rt->cart[i].cartline[j].item->name = (char*)calloc(MAX_NAME, sizeof(char));
		}
	}
	return rt;
}
void freeOrder(ORDER* o) {
	for (int i = 0; i < MAXUSERNUM; i++) {
		for (int j = 0; j < MAXCARTITEMNUM; j++) {
			free(o->cart[i].cartline[j].item->name);
			free(o->cart[i].cartline[j].item);
		}
		free(o->cart[i].cartline);
		free(o->cart[i].user->name);
		free(o->cart[i].user);
	}
	free(o->cart);
	free(o);
}
void UserOrder(int UID, ORDER* order, const MENU* menu) {
	CART* CurCart = &order->cart[UID];
	GetUserName(UID, CurCart);
	PrintOrderCommand();
	while (1) {
		if (ProcessOrder(UID, CurCart, menu) == -1) break;
	}
	//SaveCart(UID, CurCart);
	return;
}
void GetUserName(int UID, CART* cart) {
	printf("請輸入您的姓名(不可超過10個字):\n");
	char nametmp[100];
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}//清理緩衝區
	while (1) {
		scanf("%100s", nametmp);
		if (strlen(nametmp) <= 41) break; //10*4+1 byte 限定中文10字內
		else printf("輸入錯誤請重新輸入\n");
	}
	cart->user->uid = UID;
	strcpy(cart->user->name, nametmp);
	return;
}
void PrintOrderCommand() {
	printf("請選擇點餐功能\n");
	printf("輸入1: 主食\n");
	printf("輸入2: 飲料\n");
	printf("輸入3: 配餐\n");
	printf("輸入4: 查看菜單\n");
	printf("輸入5: 查看目前購物車\n");
	printf("輸入6: 編輯購物車\n");
	printf("輸入7: 結束點餐\n");
	printf("輸入8: 查看指令\n");
}
int ProcessOrder(int UID, CART* cart, const MENU* menu) {
	while (1) {
		int ch;
		scanf("%d", &ch);
		ITEM_TYPE t;
		int in;
		switch (ch) {
		case 1:
			t = ITEM_FOOD;
			printf("請輸入要點的主食編號:\n");
			scanf("%d", &in);
			AddItemtoCart(in, t, cart, menu);
			break;
		case 2:
			t = ITEM_DRINK;
			printf("請輸入要點的飲料編號:\n");
			scanf("%d", &in);
			AddItemtoCart(in, t, cart, menu);
			break;
		case 3:
			t = ITEM_OTHER;
			printf("請輸入要點的配餐編號:\n");
			scanf("%d", &in);
			AddItemtoCart(in, t, cart, menu);
			break;
		case 4:
			PrintMenu(menu);
			break;
		case 5:
			PrintCart(UID, cart);
			break;
		case 6:

			break;
		case 7:
			return -1;
			break;
		case 8:
			PrintOrderCommand();
			break;
		default:
			printf("輸入錯誤，請重新輸入，輸入8查看指令\n");
			break;
		}
		printf("請選擇點餐功能(輸入8查看指令)\n");
	}
}
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, const MENU* menu) {
	ID--;//轉回0-base
	switch (t) {
	case ITEM_FOOD:
		if (ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("輸入錯誤!無法查詢該餐點!\n");
			return;
		}
		break;
	case ITEM_DRINK:
		if (ID > MAXITEMNUM || menu->drink[ID].price <= 0) {
			printf("輸入錯誤!無法查詢該餐點!\n");
			return;
		}
		break;
	case ITEM_OTHER:
		if (ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("輸入錯誤!無法查詢該餐點!\n");
			return;
		}
		break;
	}
	int AddItemCost;
	bool found = false;
	char NewItemName[MAX_NAME];

	for (int i = 0; i < cart->cap; i++) {
		if (cart->cartline[i].item->type == t && cart->cartline[i].item->id == ID) {//點有在購物車的餐點
			cart->cartline[i].qty++;
			AddItemCost = cart->cartline[i].item->price;
			strcpy(NewItemName, cart->cartline[i].item->name);
			//flag = 1;
			found = true;
			break;
		}
	}
	if (!found) {//不在購物車的餐點(沒點過的)
		int idx = cart->cap++;
		cart->cartline[idx].item->id = ID;
		cart->cartline[idx].item->type = t;
		cart->cartline[idx].qty++;
		cart->cartline[idx].item->price = GetItemPrice(cart->cartline[idx].item->id, cart->cartline[idx].item->type, menu);
		switch (t) {
		case ITEM_FOOD:
			strcpy(cart->cartline[idx].item->name, menu->food[ID].name);
			//for debug
			//printf("\n\n[test] cart->cartline[cnt].item->name = %s", cart->cartline[cnt].item->name);
			//printf("[test] menu->food[%d].name = %s\n\n", ID, menu->food[ID].name);
			break;
		case ITEM_DRINK:
			strcpy(cart->cartline[idx].item->name, menu->drink[ID].name);
			//for debug
			//printf("\n\n[test] cart->cartline[cnt].item->name = %s", cart->cartline[cnt].item->name);
			//printf("[test] menu->drink[%d].name = %s\n\n", ID, menu->drink[ID].name);
			break;
		case ITEM_OTHER:
			strcpy(cart->cartline[idx].item->name, menu->other[ID].name);
			break;
		}
		AddItemCost = cart->cartline[idx].item->price;
		strcpy(NewItemName, cart->cartline[idx].item->name);
	}
	cart->personalcost += AddItemCost;//計算目前總額
	printf("%s 加入購物車! 當前花費:%d\n", NewItemName, cart->personalcost);
	found = false;
	return;
}

void PrintCart(int UID, CART* cart) {
	if (!cart || !cart->cartline) {
		printf("error 購物車尚未初始化！\n");
		return;
	}

	printf("\n============== %s 的購物車 =============\n", cart->user->name);
	printf("%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "ID", "類別", NAME_W, "名稱", "數量", "單價", "小計");

	int total = 0, shown = 0;

	for (int i = 0; i < MAXCARTITEMNUM; i++) {
		CARTLINE* cl = &cart->cartline[i];
		if (cl->qty == 0) continue; // 空格子跳過

		int lineCost = cl->item->price * cl->qty;
		total += lineCost;

		char itemName[20]; strcpy(itemName, cl->item->name);//取出要查看的數據
		int showid = cl->item->id;							//取出要查看的數據
		showid++;											//轉回1-base
		ITEM_TYPE t = cl->item->type;						//取出要查看的數據
		int ItemQty = cl->qty, ItemPrice = cl->item->price; //取出要查看的數據

		printf("%-3d %-4d %-6s %-*s %-3d %-7d %-7d\n",
			++shown,//編號
			showid,//食物編號
			(t == ITEM_FOOD) ? "主食" : (t == ITEM_DRINK) ? "飲料" : "配餐",//type
			NAME_W, itemName,//品名
			ItemQty,//數量
			ItemPrice,//price
			lineCost);
	}

	if (shown == 0) printf("（目前購物車空空如也）\n");
	printf("------------------------------------------------\n");
	printf("小計：%d 元\n\n", total);
}
#endif