//BY NCKU CE 117 HSIAO,TE-HSIEN
#define _CRT_SECURE_NO_WARNINGS
#ifndef __ORDER_H__
#define __ORDER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "save.h"
#define MAXUSERNUM 10
#define MAXCARTITEMNUM 30
#define MAXONCEORDERNUM 100
typedef struct _USER {
	int uid;
	char* name;
}USER;
typedef struct _CARTLINE {
	MENU_ITEM* item;
	_CARTLINE* Next;
	_CARTLINE* Prev;
	int qty;
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
int ReadNum(OPERATE o, int lb, int hb);//lb : lowerbond hb : higherbond

void UserOrder(int UID, CART* cart, const MENU* menu) {
	GetUserName(UID, cart);
	PrintOrderCommand();
	ProcessOrder(UID, cart, menu);
	//SaveCart(UID, CurCart);
	return;
}
void GetUserName(int UID, CART* cart) {
	printf("請輸入您的姓名(不可超過10個字):\n");
	char nametmp[100];
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
	printf("輸入6: 刪除購物車內容\n");
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
			PrintDelCommand();
			scanf("%d", &ch);
			switch (ch) {
			case 1: 
				t = ITEM_FOOD;
				printf("請輸入要刪除的主食編號\n");
				scanf("%d", &in);
				DeleteCartLine(in, t, cart);
				break;
			case 2:
				t = ITEM_DRINK;
				printf("請輸入要刪除的飲料編號\n");
				scanf("%d", &in);
				DeleteCartLine(in, t, cart);
				break;
			case 3:
				t = ITEM_OTHER;
				printf("請輸入要刪除的配餐編號\n");
				scanf("%d", &in);
				DeleteCartLine(in, t, cart);
				break;
			}
			break;
		case 7:
			printf("請再次確認您訂購的餐點:\n");
			PrintCart(UID, cart);
			if (CheckLeave() == 1) {
				//for debug
				printf("[Before Save] cart=%p head=%p next=%p\n",
					cart, cart->InitCartLine,
					cart->InitCartLine ? cart->InitCartLine->Next : NULL);
				SaveCart(UID, cart);
				return -1;
			}
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
		if (ID < 0 || ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("輸入錯誤!無法查詢該餐點!\n");
			return;
		}
		break;
	case ITEM_DRINK:
		if (ID < 0 || ID > MAXITEMNUM || menu->drink[ID].price <= 0) {
			printf("輸入錯誤!無法查詢該餐點!\n");
			return;
		}
		break;
	case ITEM_OTHER:
		if (ID < 0 || ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("輸入錯誤!無法查詢該餐點!\n");
			return;
		}
		break;
	}
	int num = ReadNum(Add, 0, MAXONCEORDERNUM);
	int AddItemCost;
	bool found = false;
	char NewItemName[MAX_NAME];
	CARTLINE* CurCartLine = cart->InitCartLine->Next;
	CARTLINE* tail = cart->InitCartLine;
	if (CurCartLine) {
		while (CurCartLine) {//點有在購物車的餐點
			if (CurCartLine->item->type == t && CurCartLine->item->id == ID) {
				CurCartLine->qty += num;
				AddItemCost = CurCartLine->item->price * num;
				strcpy(NewItemName, CurCartLine->item->name);
				found = true;
				break;
			}
			tail = CurCartLine;
			CurCartLine = CurCartLine->Next;//造訪下一個
		}
	}
	if (!found) {//不在購物車的餐點(沒點過的)
		tail->Next = allocCartLine(tail);
		CurCartLine = tail->Next;
		CurCartLine->item->id = ID;
		CurCartLine->item->type = t;
		CurCartLine->qty += num;
		CurCartLine->item->price = GetItemPrice(CurCartLine->item->id, CurCartLine->item->type, menu);
		cart->cap++;
		switch (t) {
		case ITEM_FOOD:
			strcpy(CurCartLine->item->name, menu->food[ID].name);
			break;
		case ITEM_DRINK:
			strcpy(CurCartLine->item->name, menu->drink[ID].name);
			break;
		case ITEM_OTHER:
			strcpy(CurCartLine->item->name, menu->other[ID].name);
			break;
		}
		AddItemCost = CurCartLine->item->price * num;
		strcpy(NewItemName, CurCartLine->item->name);
	}
	cart->cost += AddItemCost;//計算目前總額
	printf("%s 加入購物車 %d 份! 當前花費: %d 元\n", NewItemName, num, cart->cost);
	return;
}
void DeleteCartLine(int ID, ITEM_TYPE t, CART* cart) {
	ID--;
	CARTLINE* CurCartLine = cart->InitCartLine->Next;
	bool found = false;
	while (!found && CurCartLine) {
		if (CurCartLine->item->type == t && CurCartLine->item->id == ID) {
			found = true;
			int num = ReadNum(Del, 1, CurCartLine->qty);
			CurCartLine->qty -= num;
			cart->cost -= num * CurCartLine->item->price;
			printf("%s 已從購物車中刪除 %d 份!目前花費 %d 元\n", CurCartLine->item->name, num, cart->cost);
			if (CurCartLine->qty == 0) freeCartLine(CurCartLine);
		}
		CurCartLine = CurCartLine->Next;
	}
	if (!found) printf("購物車中未找到該品項!\n");
}
void PrintCart(int UID, CART* cart) {
	if (!cart || !cart->InitCartLine) {
		printf("error 購物車尚未初始化！\n");
		return;
	}

	printf("\n=================== %s 的購物車 ==================\n", cart->user->name);
	printf("%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "編號", "類別", NAME_W, "名稱", "數量", "單價", "小計");

	int shown = 0;
	CARTLINE* cl = cart->InitCartLine->Next;
	int CartCap = cart->cap;
	while (cl) {
		int lineCost = cl->item->price * cl->qty;

		ITEM_TYPE t = cl->item->type;

		printf("%-3d %-4d %-6s %-*s %-3d %-7d %-7d\n",
			++shown,//編號
			cl->item->id + 1,//食物編號
			(t == ITEM_FOOD) ? "主食" : (t == ITEM_DRINK) ? "飲料" : "配餐",//type
			NAME_W, cl->item->name,//品名
			cl->qty,//數量
			cl->item->price,//price
			lineCost);

		cl = cl->Next;
	}
	if (shown == 0) printf("（目前購物車空空如也）\n");
	printf("-----------------------------------------------------------\n");
	printf("小計：%d 元\n\n", cart->cost);
}
CART* allocCart() {
	CART* rt = (CART*)calloc(1, sizeof(CART));
	rt->user = (USER*)calloc(1, sizeof(USER));
	rt->user->name = (char*)calloc(MAX_NAME, sizeof(char));
	rt->InitCartLine = CreateInitCartLine();
	return rt;
}
void freeCart(CART* c) {
	CARTLINE* cl = c->InitCartLine;
	while (cl) {
		CARTLINE* tmp = cl->Next;
		freeCartLine(cl);
		cl = tmp;
	}
	free(c->user->name);
	free(c->user);
	free(c);
}
CARTLINE* CreateInitCartLine() {
	CARTLINE* rt = (CARTLINE*)calloc(1, sizeof(CARTLINE));
	rt->item = NULL;//不存東西
	rt->Next = NULL;
	rt->Prev = NULL;
	return rt;
}
CARTLINE* allocCartLine(CARTLINE* prev) {
	CARTLINE* rt = (CARTLINE*)calloc(1, sizeof(CARTLINE));
	rt->item = (MENU_ITEM*)calloc(1, sizeof(MENU_ITEM));
	rt->item->name = (char*)calloc(MAX_NAME, sizeof(char));
	if (prev) {
		prev->Next = rt;
		rt->Prev = prev;
	}
	return rt;
}
void freeCartLine(CARTLINE* c) {
	if (!c) return;
	CARTLINE* p = c->Prev;
	CARTLINE* n = c->Next;
	if (p) p->Next = n;
	if (n) n->Prev = p;
	if (c->item) {
		free(c->item->name);
		free(c->item);
	}
	free(c);
	return;
}
void PrintDelCommand() {
	printf("請輸入要刪除的品項\n");
	printf("(1) 主食 ");
	printf("(2) 飲料 ");
	printf("(3) 配餐 \n");
}
int ReadNum(OPERATE o, int lb, int hb) {//lowerbond higherbond
	int num;
	bool valid = false;
	while (!valid) {
		printf("請輸入數量\n");
		scanf("%d", &num);
		switch (o) {
		case Add:
			if (num >= lb && num <= hb) {
				valid = true;
				return num;
			}
			printf("請重新輸入，新增餐點時，只能增加1~%d份\n", MAXONCEORDERNUM);
			break;
		case Del:
			if (num >= lb && num <= hb) {
				valid = true;
				return num;
			}
			printf("請重新輸入，該品項您只點了%d份\n", hb);
			break;
		}
	}
}
int CheckLeave() {
	printf("輸入1: 確認餐點並送出\n");
	printf("輸入0: 繼續點餐\n");
	int rt;
	scanf("%d", &rt);
	return rt;
}
#endif