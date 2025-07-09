#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"
#include "order.h"
#define SHOPNAME "Happy"
void PrintMainCommand();
int main() {
	printf("正在載入菜單");
	MENU* menu = allocmenu();
	ReadMenu(menu);
	ORDER* order = allocOrder();
	printf("\r====%s早餐店====\n", SHOPNAME);
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
			printf("輸入錯誤\n");
			break;
		}
		if (choise == 3) {
			printf("感謝您使用本系統\n");
			break;
		}
	}
	free(menu);
	free(order);
	return 0;
}

void PrintMainCommand(){
	printf("選擇功能\n");
	printf("輸入1: 查看菜單\n");
	printf("輸入2: 新使用者開始點餐\n");
	printf("輸入3: 結束點餐\n");
}