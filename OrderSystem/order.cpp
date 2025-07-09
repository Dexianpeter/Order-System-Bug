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
	printf("��J6: �R���ʪ������e\n");
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
			PrintDelCommand();
			scanf("%d", &ch);
			switch (ch) {
			case 1: 
				t = ITEM_FOOD;
				printf("�п�J�n�R�����D���s��\n");
				scanf("%d", &in);
				DeleteCartLine(in, t, cart);
				break;
			case 2:
				t = ITEM_DRINK;
				printf("�п�J�n�R�������ƽs��\n");
				scanf("%d", &in);
				DeleteCartLine(in, t, cart);
				break;
			case 3:
				t = ITEM_OTHER;
				printf("�п�J�n�R�����t�\�s��\n");
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
		if (ID < 0 || ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("��J���~!�L�k�d�߸��\�I!\n");
			return;
		}
		break;
	case ITEM_DRINK:
		if (ID < 0 || ID > MAXITEMNUM || menu->drink[ID].price <= 0) {
			printf("��J���~!�L�k�d�߸��\�I!\n");
			return;
		}
		break;
	case ITEM_OTHER:
		if (ID < 0 || ID > MAXITEMNUM || menu->food[ID].price <= 0) {
			printf("��J���~!�L�k�d�߸��\�I!\n");
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
		while (CurCartLine) {//�I���b�ʪ������\�I
			if (CurCartLine->item->type == t && CurCartLine->item->id == ID) {
				CurCartLine->qty++;
				AddItemCost = CurCartLine->item->price;
				strcpy(NewItemName, CurCartLine->item->name);
				found = true;
				break;
			}
			tail = CurCartLine;
			CurCartLine = CurCartLine->Next;//�y�X�U�@��
		}
	}
	if (!found) {//���b�ʪ������\�I(�S�I�L��)
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
	cart->personalcost += AddItemCost;//�p��ثe�`�B
	printf("%s �[�J�ʪ���! ��e��O:%d\n", NewItemName, cart->personalcost);
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
	if (!found) printf("�ʪ����������ӫ~��!\n");
}
void PrintCart(int UID, CART* cart) {
	if (!cart || !cart->InitCartLine) {
		printf("error �ʪ����|����l�ơI\n");
		return;
	}

	printf("\n=================== %s ���ʪ��� ==================\n", cart->user->name);
	printf("%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "�s��", "���O", NAME_W, "�W��", "�ƶq", "���", "�p�p");

	int total = 0, shown = 0;
	CARTLINE* cl = cart->InitCartLine->Next;
	int CartCap = cart->cap;
	while (cl) {
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

		cl = cl->Next;
	}
	if (shown == 0) printf("�]�ثe�ʪ����ŪŦp�]�^\n");
	printf("------------------------------------------------\n");
	printf("�p�p�G%d ��\n\n", total);
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
	rt->item = NULL;//���s�F��
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
	printf("�п�J�n�R�����~��\n");
	printf("(1)�D��\n");
	printf("(2)����\n");
	printf("(3)�t�\\n");
}

#endif