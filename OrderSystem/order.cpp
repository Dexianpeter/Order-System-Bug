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

ORDER* allocOrder() {
	ORDER* rt = (ORDER*)calloc(1, sizeof(ORDER));
	rt->suborder = (SUBORDER*)calloc(MAXUSERNUM, sizeof(SUBORDER));
	for (int i = 0; i < MAXUSERNUM; i++) {
		rt->suborder[i].user = (USER*)calloc(1, sizeof(USER));//一張子訂單對應一個使用者
		rt->suborder[i].user->name = (char*)calloc(MAX_NAME, sizeof(char));
		rt->suborder[i].cart = (CART*)calloc(1, sizeof(CART));
		rt->suborder[i].cart->cartline = (CARTLINE*)calloc(MAXCARTITEMNUM, sizeof(CARTLINE));
	}
	return rt;
}
void freeOrder(ORDER* o) {
	for (int i = 0; i < MAXUSERNUM; i++) {
		free(o->suborder[i].cart->cartline);
		free(o->suborder[i].user->name);
		free(o->suborder[i].user);
	}
	free(o->suborder);
	free(o);
}
void UserOrder(int UID, ORDER* order, MENU* menu) {
	SUBORDER* CurSubOrder = order->suborder + UID;//current suborder
	GetUserName(UID, CurSubOrder);
	PrintCommand();
	CART* CurCart = CurSubOrder->cart;
	while (1) {
		if (ProcessOrder(UID, CurCart, menu) == -1) break;
	}
	//SaveSubOrder(UID, CurSubOrder);
	return;
}
void GetUserName(int UID, SUBORDER* SubOrder) {
	printf("請輸入您的姓名(不可超過10個字):\n");
	char nametmp[100];
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}//清理緩衝區
	while (1) {
		scanf("%100s", nametmp);
		if (strlen(nametmp) <= 41) break; //10*4+1 byte
		else printf("輸入錯誤請重新輸入\n");
	}
	SubOrder->user->uid = UID;
	strcpy(SubOrder->user->name, nametmp);
	return;
}
void PrintCommand() {
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
int ProcessOrder(int UID, CART* cart, MENU* menu) {
	while (1) {
		int ch;
		scanf("%d", &ch);
		//while ((ch = getchar()) != '\n' && ch != EOF) {}//清理緩衝區
		ITEM_TYPE t;
		int in;
		switch (ch) {
		case 1:
			t = ITEM_FOOD;
			printf("請輸入要點的主餐編號:\n");
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
			PrintCart(UID, cart, menu);
			break;
		case 6:

			break;
		case 7:
			return -1;
			break;
		case 8:
			PrintCommand();
			break;
		default:
			printf("輸入錯誤，請重新輸入，輸入8查看指令\n");
			break;
		}
		printf("請選擇點餐功能\n");
	}
}
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, MENU* menu) {
	static int cnt = 0; int flag = 0, AddItemCost;
	ID--;//轉回0-base
	for (int i = 0; i < cnt; i++) {
		if (cart->cartline[i].type == t && cart->cartline[i].id == ID) {//點有在購物車的餐點
			cart->cartline[i].qty++;
			AddItemCost = cart->cartline[i].price;
			flag = 1;
		}
	}

	if (flag == 0) {//不在購物車的餐點(沒點過的)
		cart->cartline[cnt].id = ID;
		cart->cartline[cnt].type = t;
		cart->cartline[cnt].qty++;
		cart->cartline[cnt].price = GetItemPrice(cart->cartline[cnt].id, cart->cartline[cnt].type, menu);
		AddItemCost = cart->cartline[cnt].price;
		cnt++;
	}
	cart->tmpcost += AddItemCost;//計算目前總額
	printf("%s加入購物車!當前花費:%d\n", "等等補", cart->tmpcost);
	flag = 0;
	return;
}

void PrintCart(int UID, CART* cart, MENU* menu) {
	if (!cart || !cart->cartline) {
		puts("error 購物車尚未初始化！");
		return;
	}

	printf("\n========== 使用者 %d 的購物車 ==========\n", UID + 1);
	printf("%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "ID", "類別", NAME_W, "名稱", "數量", "單價", "小計");

	int total = 0, shown = 0;

	for (int i = 0; i < MAXCARTITEMNUM; i++) {
		CARTLINE* cl = &cart->cartline[i];
		if (cl->qty == 0) continue;          // 空格子跳過

		char itemName[20] = "??";
		// 直接從 menu 拿名稱

		switch (cl->type) {
		case ITEM_FOOD:
			strcpy(itemName, menu->food[cl->id].name);
			break;
		case ITEM_DRINK:
			strcpy(itemName, menu->drink[cl->id].name);
			break;
		case ITEM_OTHER:
			strcpy(itemName, menu->other[cl->id].name);
			break;
		}

		int lineCost = cl->price * cl->qty;
		total += lineCost;

		printf("%-3d %-4d %-6s %-*s %-3d %-7d %-7d\n",
			++shown,//編號
			cl->id,//食物編號
			(cl->type == ITEM_FOOD) ? "主食" : (cl->type == ITEM_DRINK) ? "飲料" : "配餐",//type
			NAME_W, itemName,//品名
			cl->qty,//數量
			cl->price,//price
			lineCost);
	}

	if (shown == 0) printf("（目前購物車為空）\n");
	printf("---------------------------------------\n");
	printf("小計：%d 元\n\n", total);
}
#endif