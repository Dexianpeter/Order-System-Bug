#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"
#include "order.h"
#define SHOPNAME "Happy"
void PrintMainCommand();
int main() {
	printf("���b���J���");
	MENU* menu = allocmenu();
	ReadMenu(menu);
	ORDER* order = allocOrder();
	printf("\r====%s���\��====\n", SHOPNAME);
	while (1) {
		PrintMainCommand();
		int choise, UID = 0;
		scanf("%d", &choise);
		switch (choise) {
		case 1:
			PrintMenu(menu);
			break;
		case 2:
			UserOrder(UID++, order, menu);
			break;
		case 3:
			//SaveOrder(order);
			break;
		default:
			printf("��J���~\n");
			break;
		}
		if (choise == 3) {
			printf("�P�±z�ϥΥ��t��\n");
			break;
		}
	}
	free(menu);
	free(order);
	return 0;
}

void PrintMainCommand(){
	printf("��ܥ\��\n");
	printf("��J1: �d�ݵ��\n");
	printf("��J2: �s�ϥΪ̶}�l�I�\\n");
	printf("��J3: �����I�\\n");
}