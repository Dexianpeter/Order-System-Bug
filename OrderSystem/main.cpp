#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"
#include "order.h"
#include "save.h"
#define SHOPNAME "Happy"
int main() {
	printf("正在載入菜單");
	MENU* menu = allocmenu();
	ReadMenu(menu);
	CART* cart = allocCart();

	printf("\r========%s早餐店========\n", SHOPNAME);
	UserOrder(0, cart, menu);
	printf("感謝您使用本系統\n");
	freemenu(menu);
	freeCart(cart);
	system("pause");
	return 0;
	
}
