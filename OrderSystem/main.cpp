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
			UserOrder(UID, order, menu);
			break;
		/*case 3:
			UserOrder(UID++, order, menu);
			break;*/
		case 4:
			//SaveOrder(order);
			break;
		default:
			printf("輸入錯誤\n");
			break;
		}
		if (choise == 4) {
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
	printf("(1)查看菜單\n");
	printf("(2)開始點餐\n");
	//printf("(3)下一位使用者點餐\n");
	printf("(4)結束點餐\n");
}