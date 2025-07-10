//BY NCKU CE 117 HSIAO,TE-HSIEN
#define _CRT_SECURE_NO_WARNINGS
#ifndef __SAVE_H__
#define __SAVE_H__
#include "menu.h"
#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <time.h>
typedef struct _CART CART;
void SaveCart(int UID, const CART* cart);
#endif
