#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FamilyTree.h"

/*
* �˵�
*/
int view() {
	int choice;
	printf("\n\n");
	printf("        ====================== �½��� 3119005086 �������3�� ========================\n\n");
	printf("        ****************************** ����ϵͳ *************************************\n");
	printf("        *                                                                           *\n");
	printf("        *            1:��ȡ�ļ���ʼ��������      2:�½������ļ�                     *\n");
	printf("        *            3:����������Ա              4:�鿴��Ա��Ϣ                     *\n");
	printf("        *            5:�޸ĳ�Ա��Ϣ              6:ɾ����Ա                         *\n");
	printf("        *            7:��α�������              8:��ӡ�������νṹ                 *\n");
	printf("        *            9:�ж����˼����ϵ          10:��ѯ�����ϵ                    *\n");
	printf("        *            11:��ѯ���ȹ�ϵ             12:��ѯ���ֵܹ�ϵ                  *\n");
	printf("        *            13:������������ļ�         14:���ټ���                        *\n");
	printf("        *            15:�ж�������Ա�Ƿ�����ֱϵ����ϵ������ϵ                      *\n");
	printf("        *            0:��������                                                     *\n");
	printf("        *                                                                           *\n");
	printf("        *****************************************************************************\n\n");
	printf("������������[0-15] :");
	scanf("%d", &choice);
	while (choice < 0 || choice > 15) {
		while (getchar() != '\n') {	//�����������
			continue;
		}
		printf("\n������������������!\n");
		printf("\n������������[0-15] :");
		scanf("%d", &choice);
	}
	return choice;
}

/*
* ������
*/
int main() {
	int choice;
	char name[MAX];
	char name1[MAX];
	char name2[MAX];
	char parentName[MAX];
	CSTree T = NULL;
	CSTree node = NULL;
	while (1) {
		choice = view();
		switch(choice) {
		case 1:
			T = InitTree();
			break;
		case 2:
			NewFile();
			break;
		case 3:
			printf("�����븸ĸ������ :");
			scanf("%s", parentName);
			InsertNode(T, parentName);
			break;
		case 4:
			printf("������Ҫ���ҳ�Ա������:");
			scanf("%s", name);
			node = Search(T, name);
			visit(node);
			break;
		case 5:
			printf("������Ҫ�޸ĳ�Ա������:");
			scanf("%s", name);
			ModifyNode(T, name);
			break;
		case 6:
			printf("������Ҫɾ����Ա������:");
			scanf("%s", name);
			DeleteNode(T, name);
			break;
		case 7:
			LevelOrderTraverse(T);
			break;
		case 8:
			PrintTree(T);
			break;
		case 9:
			printf("�������Ա1��������");
			scanf("%s", name1);
			printf("�������Ա2��������");
			scanf("%s", name2);
			Relationship(T, name1, name2);
			break;
		case 10:
			printf("������Ҫ���������ѯ�ĳ�Ա������:");
			scanf("%s", name);
			Descendant(T, name);
			break;
		case 11:
			printf("������Ҫ�������Ȳ�ѯ�ĳ�Ա������:");
			scanf("%s", name);
			Ancestor(T, name);
			break;
		case 12:
			printf("������Ҫ�������ֵܲ�ѯ�ĳ�Ա������:");
			scanf("%s", name);
			Cousin(T, name);
			break;
		case 13:
			SaveFile(T);
			break;
		case 14:
			DestoryTree(T);
			printf("���ٳɹ���\n");
			break;
		case 15:
			printf("�������Ա1��������");
			scanf("%s", name1);
			printf("�������Ա2��������");
			scanf("%s", name2);
			GenerationRelationship(T, name1, name2);
			break;
		case 0:
			DestoryTree(T);
			exit(0);
			break;
		}
	}
	return 0;
}



void test() {
	CSTree T = (CSTree)malloc(sizeof(CSTNode));
	T->firstChild = T->nextSibling = NULL;
	strcpy(T->data.name, "����");
	strcpy(T->data.birth, "231��");
	strcpy(T->data.parent, "��");
	T->data.generation = 1;
	char parentName[MAX];
	printf("�����븸ĸ������ :");
	scanf("%s", parentName);
	InsertNode(T, parentName);
	while (T != NULL) {
		printf("%s", T->data.name);
		printf("%s", T->data.birth);
		T = T->firstChild;
	}
}