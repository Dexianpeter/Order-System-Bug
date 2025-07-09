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
	int personalcost;
	int cap;
}CART;

typedef struct _ORDER {
	CART* cart;
	int allcost;
}ORDER;

void UserOrder(int UID, ORDER* order, const MENU* menu);
void GetUserName(int UID, CART* cart);
void PrintOrderCommand();
int ProcessOrder(int UID, CART* cart, const MENU* menu);
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, const MENU* menu);
void DeleteCartLine(int ID, ITEM_TYPE t, CART* cart);
void PrintDelCommand();
void PrintCart(int UID, CART* cart);

ORDER* allocOrder();
void freeOrder(ORDER* o);
CARTLINE* allocCartLine(CARTLINE* prev);
CARTLINE* CreateInitCartLine();
void freeCartLine(CARTLINE* c);



void SaveCart(int UID, CART* cart);
void SaveOrder(ORDER* order);

void UserOrder(int UID, ORDER* order, const MENU* menu) {
	CART* CurCart = order->cart + UID;
	GetUserName(UID, CurCart);
	PrintOrderCommand();
	ProcessOrder(UID, CurCart, menu);
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

	int AddItemCost;
	bool found = false;
	char NewItemName[MAX_NAME];
	CARTLINE* CurCartLine = cart->InitCartLine->Next;
	CARTLINE* tail = cart->InitCartLine;
	if (CurCartLine) {
		while (CurCartLine) {//點有在購物車的餐點
			if (CurCartLine->item->type == t && CurCartLine->item->id == ID) {
				CurCartLine->qty++;
				AddItemCost = CurCartLine->item->price;
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
		CurCartLine->qty++;
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
		AddItemCost = CurCartLine->item->price;
		strcpy(NewItemName, CurCartLine->item->name);
	}
	cart->personalcost += AddItemCost;//計算目前總額
	printf("%s 加入購物車! 當前花費:%d\n", NewItemName, cart->personalcost);
	found = false;
	return;
}
void DeleteCartLine(int ID, ITEM_TYPE t, CART* cart) {
	ID--;
	CARTLINE* CurCartLine = cart->InitCartLine->Next;
	bool found = false;
	while (!found && CurCartLine) {
		if (CurCartLine->item->type == t && CurCartLine->item->id == ID) {
			found = true;
			CurCartLine->qty--;
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

	int total = 0, shown = 0;
	CARTLINE* cl = cart->InitCartLine->Next;
	int CartCap = cart->cap;
	while (cl) {
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

		cl = cl->Next;
	}
	if (shown == 0) printf("（目前購物車空空如也）\n");
	printf("------------------------------------------------\n");
	printf("小計：%d 元\n\n", total);
}
ORDER* allocOrder() {
	ORDER* rt = (ORDER*)calloc(1, sizeof(ORDER));
	rt->cart = (CART*)calloc(MAXUSERNUM, sizeof(CART));
	for (int i = 0; i < MAXUSERNUM; i++) {
		rt->cart[i].user = (USER*)calloc(1, sizeof(USER));
		rt->cart[i].user->name = (char*)calloc(MAX_NAME, sizeof(char));
		rt->cart[i].InitCartLine = CreateInitCartLine();
	}
	return rt;
}
void freeOrder(ORDER* o) {
	for (int i = 0; i < MAXUSERNUM; i++) {
		CARTLINE* c = o->cart[i].InitCartLine;
		while (c) {
			CARTLINE* tmp = c->Next;
			freeCartLine(c);
			c = tmp;
		}
		free(o->cart[i].user->name);
		free(o->cart[i].user);
	}
	free(o->cart);
	free(o);
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
	free(c->item->name);
	free(c->item);
	free(c);
	return;
}
void PrintDelCommand() {
	printf("請輸入要刪除的品項\n");
	printf("(1)主食\n");
	printf("(2)飲料\n");
	printf("(3)配餐\n");
}

#endif