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
			UserOrder(UID, order, menu);
			break;
		/*case 3:
			UserOrder(UID++, order, menu);
			break;*/
		case 4:
			//SaveOrder(order);
			break;
		default:
			printf("��J���~\n");
			break;
		}
		if (choise == 4) {
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
	printf("(1)�d�ݵ��\n");
	printf("(2)�}�l�I�\\n");
	//printf("(3)�U�@��ϥΪ��I�\\n");
	printf("(4)�����I�\\n");
}