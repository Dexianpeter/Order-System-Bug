//BY NCKU CE 117 HSIAO,TE-HSIEN
#define _CRT_SECURE_NO_WARNINGS
#ifndef __SAVE_H__
#define __SAVE_H__
#include <windows.h>     
#include <shlwapi.h>     
#include <direct.h>      
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include "menu.h"
#include "order.h"
#pragma comment(lib, "Shlwapi.lib")
#define PATH_SEP '\\'
#define SHOPNAME "Happy"

int MakeDir(const char *p);
void SaveCart(int UID, CART* cart);
void TimeStr(char *Dest, size_t len);

int MakeDir(const char *p) {
	if (_mkdir(p) == 0) return 0;
	if (errno == EEXIST) return 0;
	return -1;
}
void TimeStr(char *Dest, size_t len){
	time_t now = time(NULL);
	struct tm  tmv;
	localtime_s(&tmv, &now);
	strftime(Dest, len, "%m%d_%H_%M_%S", &tmv);
}
void SaveCart(int UID, const CART* cart) {
	//for debug
	printf("[fun take Cart Before Save] cart=%p head=%p next=%p\n",
		cart, cart->InitCartLine,
		cart->InitCartLine ? cart->InitCartLine->Next : NULL);

	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH); // ���o exe ������|
	PathRemoveFileSpecA(exePath); // �鱼�ɦW�A�Ѹ�Ƨ�
	PathRemoveFileSpecA(exePath); // �A��@���A�M�׮ڥؿ�

	char parentDir[MAX_PATH];
	snprintf(parentDir, sizeof(parentDir), "%s%cOrders", exePath, PATH_SEP);
	if (MakeDir(parentDir) != 0) {
		perror("���h Orders �إߥ���");
		return ;
	}

	char ts[32];
	TimeStr(ts, sizeof(ts));
	char subDir[MAX_PATH];
	snprintf(subDir, sizeof(subDir), "%s%c%s", parentDir, PATH_SEP, ts);
	if (MakeDir(subDir) != 0) {
		perror("�إߤl��Ƨ�����");
		return;
	}
	char filePath[MAX_PATH];
	snprintf(filePath, sizeof(filePath), "%s%c%d.txt", subDir, PATH_SEP, UID);

	FILE *fp = fopen(filePath, "w");
	if (!fp) { 
		perror("�g�ɥ���");
		return; 
	}

	//�L����
	fprintf(fp, "========= %s ���\�� ===========\n", SHOPNAME);
	fprintf(fp, "�Ӧ� %s ���q�� UID : %d �e��ɶ�: %s\n", cart->user->name, UID, ts);
	fprintf(fp, "%-3s %-4s %-6s %-*s %-3s %-7s %-7s\n",
		"No", "�s��", "���O", NAME_W, "�W��", "�ƶq", "���", "�p�p");
	int shown = 0;
	CARTLINE* cl = cart->InitCartLine->Next;
	//for debug
	printf("[After Save] cart=%p head=%p next=%p\n",
		cart, cart->InitCartLine,
		cart->InitCartLine ? cart->InitCartLine->Next : NULL);
	int Qty = 0;
	while (cl) {
		const int lineCost = cl->item->price * cl->qty;
		const int showid = cl->item->id + 1;      // 1-base
		const ITEM_TYPE t = cl->item->type;

		fprintf(fp, "%-3d %-4d %-6s %-*.*s %-3d %-7d %-7d\n",
			++shown,
			showid,
			(t == ITEM_FOOD) ? "�D��" : (t == ITEM_DRINK) ? "����" : "�t�\",
			NAME_W, NAME_W,                       // ��e�P�̤j��X����
			cl->item->name,
			cl->qty,
			cl->item->price,
			lineCost);

		Qty += cl->qty;
		cl = cl->Next;
	}
	fprintf(fp, "�@%d��, �`�p%d��\n", Qty, cart->cost);
	fclose(fp);
}
#endif 