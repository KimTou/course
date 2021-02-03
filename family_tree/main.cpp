#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FamilyTree.h"

/*
* 菜单
*/
int view() {
	int choice;
	printf("\n\n");
	printf("        ====================== 陈金涛 3119005086 软件工程3班 ========================\n\n");
	printf("        ****************************** 家谱系统 *************************************\n");
	printf("        *                                                                           *\n");
	printf("        *            1:读取文件初始化家谱树      2:新建家谱文件                     *\n");
	printf("        *            3:插入新增成员              4:查看成员信息                     *\n");
	printf("        *            5:修改成员信息              6:删除成员                         *\n");
	printf("        *            7:层次遍历家谱              8:打印家谱树形结构                 *\n");
	printf("        *            9:判断两人家族关系          10:查询子孙关系                    *\n");
	printf("        *            11:查询祖先关系             12:查询堂兄弟关系                  *\n");
	printf("        *            13:保存家谱树到文件         14:销毁家谱                        *\n");
	printf("        *            15:判断两个成员是否属于直系或旁系三代关系                      *\n");
	printf("        *            0:结束程序                                                     *\n");
	printf("        *                                                                           *\n");
	printf("        *****************************************************************************\n\n");
	printf("请输入操作序号[0-15] :");
	scanf("%d", &choice);
	while (choice < 0 || choice > 15) {
		while (getchar() != '\n') {	//清除错误输入
			continue;
		}
		printf("\n输入有误，请重新输入!\n");
		printf("\n请输入操作序号[0-15] :");
		scanf("%d", &choice);
	}
	return choice;
}

/*
* 主程序
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
			printf("请输入父母的姓名 :");
			scanf("%s", parentName);
			InsertNode(T, parentName);
			break;
		case 4:
			printf("请输入要查找成员的姓名:");
			scanf("%s", name);
			node = Search(T, name);
			visit(node);
			break;
		case 5:
			printf("请输入要修改成员的姓名:");
			scanf("%s", name);
			ModifyNode(T, name);
			break;
		case 6:
			printf("请输入要删除成员的姓名:");
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
			printf("请输入成员1的姓名：");
			scanf("%s", name1);
			printf("请输入成员2的姓名：");
			scanf("%s", name2);
			Relationship(T, name1, name2);
			break;
		case 10:
			printf("请输入要进行子孙查询的成员的姓名:");
			scanf("%s", name);
			Descendant(T, name);
			break;
		case 11:
			printf("请输入要进行祖先查询的成员的姓名:");
			scanf("%s", name);
			Ancestor(T, name);
			break;
		case 12:
			printf("请输入要进行堂兄弟查询的成员的姓名:");
			scanf("%s", name);
			Cousin(T, name);
			break;
		case 13:
			SaveFile(T);
			break;
		case 14:
			DestoryTree(T);
			printf("销毁成功！\n");
			break;
		case 15:
			printf("请输入成员1的姓名：");
			scanf("%s", name1);
			printf("请输入成员2的姓名：");
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
	strcpy(T->data.name, "陈先");
	strcpy(T->data.birth, "231年");
	strcpy(T->data.parent, "无");
	T->data.generation = 1;
	char parentName[MAX];
	printf("请输入父母的姓名 :");
	scanf("%s", parentName);
	InsertNode(T, parentName);
	while (T != NULL) {
		printf("%s", T->data.name);
		printf("%s", T->data.birth);
		T = T->firstChild;
	}
}