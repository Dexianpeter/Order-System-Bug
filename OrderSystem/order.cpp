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
		rt->suborder[i].user = (USER*)calloc(1, sizeof(USER));//�@�i�l�q������@�ӨϥΪ�
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
	printf("�п�J�z���m�W(���i�W�L10�Ӧr):\n");
	char nametmp[100];
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}//�M�z�w�İ�
	while (1) {
		scanf("%100s", nametmp);
		if (strlen(nametmp) <= 41) break; //10*4+1 byte
		else printf("��J���~�Э��s��J\n");
	}
	SubOrder->user->uid = UID;
	strcpy(SubOrder->user->name, nametmp);
	return;
}
void PrintCommand() {
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
int ProcessOrder(int UID, CART* cart, MENU* menu) {
	while (1) {
		int ch;
		scanf("%d", &ch);
		//while ((ch = getchar()) != '\n' && ch != EOF) {}//�M�z�w�İ�
		ITEM_TYPE t;
		int in;
		switch (ch) {
		case 1:
			t = ITEM_FOOD;
			printf("�п�J�n�I���D�\�s��:\n");
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
			printf("��J���~�A�Э��s��J�A��J8�d�ݫ��O\n");
			break;
		}
		printf("�п���I�\�\��\n");
	}
}
void AddItemtoCart(int ID, ITEM_TYPE t, CART* cart, MENU* menu) {
	static int cnt = 0; int flag = 0, AddItemCost;
	ID--;//��^0-base
	for (int i = 0; i < cnt; i++) {
		if (cart->cartline[i].type == t && cart->cartline[i].id == ID) {//�I���b�ʪ������\�I
			cart->cartline[i].qty++;
			AddItemCost = cart->cartline[i].price;
			flag = 1;
		}
	}

	if (flag == 0) {//���b�ʪ������\�I(�S�I�L��)
		cart->cartline[cnt].id = ID;
		cart->cartline[cnt].type = t;
		cart->cartline[cnt].qty++;
		cart->cartline[cnt].price = GetItemPrice(cart->cartline[cnt].id, cart->cartline[cnt].type, menu);
		AddItemCost = cart->cartline[cnt].price;
		cnt++;
	}
	cart->tmpcost += AddItemCost;//�p��ثe�`�B
	printf("%s�[�J�ʪ���!��e��O:%d\n", "������", cart->tmpcost);
	flag = 0;
	return;
}

void PrintCart(int UID, CART* cart, MENU* menu) {
	if (!cart || !cart->cartline) {
		puts("error �ʪ����|����l�ơI");
		return;
	}

	printf("\n========== �ϥΪ� %d ���ʪ��� ==========\n", UID + 1);
	printf("%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "ID", "���O", NAME_W, "�W��", "�ƶq", "���", "�p�p");

	int total = 0, shown = 0;

	for (int i = 0; i < MAXCARTITEMNUM; i++) {
		CARTLINE* cl = &cart->cartline[i];
		if (cl->qty == 0) continue;          // �Ů�l���L

		char itemName[20] = "??";
		// �����q menu ���W��

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
			++shown,//�s��
			cl->id,//�����s��
			(cl->type == ITEM_FOOD) ? "�D��" : (cl->type == ITEM_DRINK) ? "����" : "�t�\",//type
			NAME_W, itemName,//�~�W
			cl->qty,//�ƶq
			cl->price,//price
			lineCost);
	}

	if (shown == 0) printf("�]�ثe�ʪ������š^\n");
	printf("---------------------------------------\n");
	printf("�p�p�G%d ��\n\n", total);
}
#endif