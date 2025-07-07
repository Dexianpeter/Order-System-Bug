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
	printf("�п�J�z���m�W(���i�W�L10�Ӧr):\n");
	char nametmp[100];
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}//�M�z�w�İ�
	while (1) {
		scanf("%100s", nametmp);
		if (strlen(nametmp) <= 41) break; //10*4+1 byte ���w����10�r��
		else printf("��J���~�Э��s��J\n");
	}
	cart->user->uid = UID;
	strcpy(cart->user->name, nametmp);
	return;
}
void PrintOrderCommand() {
	printf("�п���I�\�\��\n");
	printf("��J1: �D��\n");
	printf("��J2: ����\n");
	printf("��J3: �t�\\n");
	printf("��J4: �d�ݵ��\n");
	printf("��J5: �d�ݥثe�ʪ���\n");
	printf("��J6: �s���ʪ���\n");
	printf("��J7: �����I�\\n");
	printf("��J8: �d�ݫ��O\n");
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
			printf("�п�J�n�I���D���s��:\n");
			scanf("%d", &in);
			AddItemtoCart(in, t, cart, menu);
			break;
		case 2:
			t = ITEM_DRINK;
			printf("�п�J�n�I�����ƽs��:\n");
			scanf("%d", &in);
			AddItemtoCart(in, t, cart, menu);
			break;
		case 3:
			t = ITEM_OTHER;
			printf("�п�J�n�I���t�\�s��:\n");
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
			printf("��J���~�A�Э��s��J�A��J8�d�ݫ��O\n");
			break;
		}
		printf("�п���I�\�\��(��J8�d�ݫ��O)\n");
	}
}
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, const MENU* menu) {
	ID--;//��^0-base
	switch (t) {
	case ITEM_FOOD:
		if (ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("��J���~!�L�k�d�߸��\�I!\n");
			return;
		}
		break;
	case ITEM_DRINK:
		if (ID > MAXITEMNUM || menu->drink[ID].price <= 0) {
			printf("��J���~!�L�k�d�߸��\�I!\n");
			return;
		}
		break;
	case ITEM_OTHER:
		if (ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("��J���~!�L�k�d�߸��\�I!\n");
			return;
		}
		break;
	}
	int AddItemCost;
	bool found = false;
	char NewItemName[MAX_NAME];

	for (int i = 0; i < cart->cap; i++) {
		if (cart->cartline[i].item->type == t && cart->cartline[i].item->id == ID) {//�I���b�ʪ������\�I
			cart->cartline[i].qty++;
			AddItemCost = cart->cartline[i].item->price;
			strcpy(NewItemName, cart->cartline[i].item->name);
			//flag = 1;
			found = true;
			break;
		}
	}
	if (!found) {//���b�ʪ������\�I(�S�I�L��)
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
	cart->personalcost += AddItemCost;//�p��ثe�`�B
	printf("%s �[�J�ʪ���! ��e��O:%d\n", NewItemName, cart->personalcost);
	found = false;
	return;
}

void PrintCart(int UID, CART* cart) {
	if (!cart || !cart->cartline) {
		printf("error �ʪ����|����l�ơI\n");
		return;
	}

	printf("\n============== %s ���ʪ��� =============\n", cart->user->name);
	printf("%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "ID", "���O", NAME_W, "�W��", "�ƶq", "���", "�p�p");

	int total = 0, shown = 0;

	for (int i = 0; i < MAXCARTITEMNUM; i++) {
		CARTLINE* cl = &cart->cartline[i];
		if (cl->qty == 0) continue; // �Ů�l���L

		int lineCost = cl->item->price * cl->qty;
		total += lineCost;

		char itemName[20]; strcpy(itemName, cl->item->name);//���X�n�d�ݪ��ƾ�
		int showid = cl->item->id;							//���X�n�d�ݪ��ƾ�
		showid++;											//��^1-base
		ITEM_TYPE t = cl->item->type;						//���X�n�d�ݪ��ƾ�
		int ItemQty = cl->qty, ItemPrice = cl->item->price; //���X�n�d�ݪ��ƾ�

		printf("%-3d %-4d %-6s %-*s %-3d %-7d %-7d\n",
			++shown,//�s��
			showid,//�����s��
			(t == ITEM_FOOD) ? "�D��" : (t == ITEM_DRINK) ? "����" : "�t�\",//type
			NAME_W, itemName,//�~�W
			ItemQty,//�ƶq
			ItemPrice,//price
			lineCost);
	}

	if (shown == 0) printf("�]�ثe�ʪ����ŪŦp�]�^\n");
	printf("------------------------------------------------\n");
	printf("�p�p�G%d ��\n\n", total);
}
#endif