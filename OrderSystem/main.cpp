#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"
#include "order.h"
#include "save.h"
#define SHOPNAME "Happy"
int main() {
	printf("���b���J���");
	MENU* menu = allocmenu();
	ReadMenu(menu);
	CART* cart = allocCart();

	printf("\r========%s���\��========\n", SHOPNAME);
	UserOrder(0, cart, menu);
	printf("�P�±z�ϥΥ��t��\n");
	freemenu(menu);
	freeCart(cart);
	system("pause");
	return 0;
	
}
