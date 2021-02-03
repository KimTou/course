#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTree.h"

/*
* �˵�
*/
int view() {
	int choice;
	printf("\n\n");
	printf("        ====================== �½��� 3119005086 �������3�� ========================\n\n");
	printf("        ***************************** B�����ܲ˵� ***********************************\n");
	printf("        *                                                                           *\n");
	printf("        *            1:����ؼ���               2:ɾ���ؼ���                        *\n");
	printf("        *            3:��ӡB��                  4:��α���B��                       *\n");
	printf("        *            5:�������һ��B��          6:����B��                           *\n");
	printf("        *            7:�������                 8:ǰ�����                          *\n");
	printf("        *            9:����ؼ��ָ���           0:��������                          *\n");
	printf("        *                                                                           *\n");
	printf("        *****************************************************************************\n\n");
	printf("������������[0-9] :");
	scanf("%d", &choice);
	while (choice < 0 || choice > 9) {
		while (getchar() != '\n') {	//�����������
			continue;
		}
		printf("\n������������������!\n");
		printf("\n������������[0-9] :");
		scanf("%d", &choice);
	}
	return choice;
}

/*
* ������
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
			printf("������Ҫ�������ֵ��");
			scanf("%d", &key);
			if (key < 0 || key > 1000) {
				printf("����ʧ��\t�����������ֵ�� 0 - 999 ��\n");
				break;
			}
			SearchBTree(t, key, r);
			if (r.tag == 1) {
				printf("B�������иý��\n");
			}
			else {
				InsertBTree(t, key, r.pt, r.i);
				printf("����ɹ�\n");
			}
			break;
		case 2:
			if (t == NULL) {
				printf("����Ϊ����\n");
				break;
			}
			printf("������Ҫɾ������ֵ��");
			scanf("%d", &key);
			SearchBTree(t, key, r);
			if (r.tag == 0) {
				printf("δ��B���в��ҵ��ùؼ���\n");
			}
			else {
				DeleteBTree(t,r.pt, r.i);
				printf("ɾ���ɹ�\n");
			}
			break;
		case 3:
			if (t == NULL) {
				printf("����Ϊ����\n");
			}
			else {
				PrintBTree(t, 5);
			}
			break;
		case 4:
			if (t == NULL) {
				printf("����Ϊ����\n");
			}
			else {
				LevelOrderTraverse(t);
			}
			break;
		case 5:
			printf("�������½�B���ؼ��ָ�����");
			scanf("%d", &num);
			if (num >= 0 && num < 100) {
				t = InitBTree(num);
				printf("�½��ɹ���\n");
			}
			else {
				printf("�½�ʧ��\t�����������ֵ�� 0 - 99 ��\n");
			}
			break;
		case 6:
			if (t == NULL) {
				printf("����Ϊ����\n");
			}
			else {
				DestroyBTree(t);
				printf("�ݻ���ɣ�\n");
			}
			break;
		case 7:
			if (t == NULL) {
				printf("����Ϊ����\n");
			}
			else {
				InOrderTraverse(t);
			}
			break;
		case 8:
			if (t == NULL) {
				printf("����Ϊ����\n");
			}
			else {
				PreOrderTraverse(t);
			}
			break;
		case 9:
			num = CountKeyNums(t);
			printf("�ܹ���%d���ؼ���\n", num);
			break;
		case 0:
			exit(0);
			break;
		}
	}
	return 0;
}
