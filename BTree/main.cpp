#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTree.h"

/*
* 菜单
*/
int view() {
	int choice;
	printf("\n\n");
	printf("        ====================== 陈金涛 3119005086 软件工程3班 ========================\n\n");
	printf("        ***************************** B树功能菜单 ***********************************\n");
	printf("        *                                                                           *\n");
	printf("        *            1:插入关键字               2:删除关键字                        *\n");
	printf("        *            3:打印B树                  4:层次遍历B树                       *\n");
	printf("        *            5:随机生成一颗B树          6:销毁B树                           *\n");
	printf("        *            7:中序遍历                 8:前序遍历                          *\n");
	printf("        *            9:计算关键字个数           0:结束程序                          *\n");
	printf("        *                                                                           *\n");
	printf("        *****************************************************************************\n\n");
	printf("请输入操作序号[0-9] :");
	scanf("%d", &choice);
	while (choice < 0 || choice > 9) {
		while (getchar() != '\n') {	//清除错误输入
			continue;
		}
		printf("\n输入有误，请重新输入!\n");
		printf("\n请输入操作序号[0-9] :");
		scanf("%d", &choice);
	}
	return choice;
}

/*
* 主程序
*/
int main() {
	int choice;
	int num;
	result r;
	KeyType key;
	BTree t = NULL;
	while (1) {
		choice = view();
		switch (choice) {
		case 1:
			printf("请输入要插入的数值：");
			scanf("%d", &key);
			if (key < 0 || key > 1000) {
				printf("插入失败\t请输入合理数值（ 0 - 999 ）\n");
				break;
			}
			SearchBTree(t, key, r);
			if (r.tag == 1) {
				printf("B树中已有该结点\n");
			}
			else {
				InsertBTree(t, key, r.pt, r.i);
				printf("插入成功\n");
			}
			break;
		case 2:
			if (t == NULL) {
				printf("该树为空树\n");
				break;
			}
			printf("请输入要删除的数值：");
			scanf("%d", &key);
			SearchBTree(t, key, r);
			if (r.tag == 0) {
				printf("未在B树中查找到该关键字\n");
			}
			else {
				DeleteBTree(t,r.pt, r.i);
				printf("删除成功\n");
			}
			break;
		case 3:
			if (t == NULL) {
				printf("该树为空树\n");
			}
			else {
				PrintBTree(t, 5);
			}
			break;
		case 4:
			if (t == NULL) {
				printf("该树为空树\n");
			}
			else {
				LevelOrderTraverse(t);
			}
			break;
		case 5:
			printf("请输入新建B树关键字个数：");
			scanf("%d", &num);
			if (num >= 0 && num < 100) {
				t = InitBTree(num);
				printf("新建成功！\n");
			}
			else {
				printf("新建失败\t请输入合理数值（ 0 - 99 ）\n");
			}
			break;
		case 6:
			if (t == NULL) {
				printf("该树为空树\n");
			}
			else {
				DestroyBTree(t);
				printf("摧毁完成！\n");
			}
			break;
		case 7:
			if (t == NULL) {
				printf("该树为空树\n");
			}
			else {
				InOrderTraverse(t);
			}
			break;
		case 8:
			if (t == NULL) {
				printf("该树为空树\n");
			}
			else {
				PreOrderTraverse(t);
			}
			break;
		case 9:
			num = CountKeyNums(t);
			printf("总共有%d个关键字\n", num);
			break;
		case 0:
			exit(0);
			break;
		}
	}
	return 0;
}
