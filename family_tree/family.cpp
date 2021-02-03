#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FamilyTree.h"

/*
* 读取文件初始化家谱树
*/
CSTree InitTree() {
	int i,n;
	CSTree T = NULL;
	char name[MAX];
	char birth[MAX];
	char parentName[MAX];
	FILE* fp;	//指向文件型数据的指针变量
	if ((fp = fopen("family.txt", "r")) == NULL) {	//以读入方式打开family文本文件
		printf("open file error!\n");
		exit(0);
	}
	fscanf(fp, "%d", &n);
	for (i = 0; i < n; i++) {	//按顺序读入成员信息，并构建家谱树
		fscanf(fp, "%s%s%s", name, birth, parentName);
		InsertNode_auto(T, name, birth, parentName);
	}
	fclose(fp);
	printf("读取完成！\n");
	return T;
}

/*
* 新建家谱文件
*/
void NewFile() {
	int i, n;
	char name[MAX];
	char birth[MAX];
	char parentName[MAX];
	FILE* fp;	//指向文件型数据的指针变量
	CSTree p = (CSTree)malloc(sizeof(CSTNode));
	if ((fp = fopen("family.txt", "w")) == NULL){	//以写入的方式打开family文本文件
		printf("open file error!\n");
		exit(0);
	}
	printf("请输入存入的人数：");
	scanf("%d", &n);
	while (n < 0 || n > 99) {
		printf("请输入合理的数值（0-99）\n");
		printf("请输入存入的人数：");
		scanf("%d", &n);
	}
	fprintf(fp, "%d ", n);
	printf("现在开始输入家谱成员信息（第1位成员请输入最顶层祖先，其父亲姓名为“无”）\n\n");
	for (i = 1; i <= n; i++) {
		printf("请输入第%d个成员的姓名、出生日期、父亲姓名：", i);
		scanf("%s%s%s", name, birth, parentName);
		fprintf(fp, "%s %s %s ", name, birth, parentName);
	}
	fclose(fp);
	printf("新建完成！\n");
}

/*
* 保存家谱树到文件
*/
void SaveFile(CSTree T) {
	if (NULL == T) {
		printf("此树为空！\n");
		return;
	}
	int i = 1, sum = 0;
	CSTree p = T;
	LQueue Q;
	InitQueue(Q);
	FILE* fp;	//指向文件型数据的指针变量
	if ((fp = fopen("family.txt", "w")) == NULL)	//以写入的方式打开family文本文件
	{
		printf("open file error!\n");
		exit(0);
	}
	fprintf(fp, "    ");	//为家谱总人数预留位置
	while (p != NULL) {
		sum++;
		EnQueue(Q, p);
		p = p->nextSibling;
	}
	while (OK == DeQueue(Q, p)) {		//直到队列为空
		fprintf(fp, "%s %s %s ", p->data.name, p->data.birth, p->data.parent);
		p = p->firstChild;
		while (p != NULL) {
			sum++;					//统计一代人的人数
			EnQueue(Q, p);
			p = p->nextSibling;
		}
	}
	rewind(fp);		//文件指针移回到文件头
	fprintf(fp, "%d ", sum);	//写入成员个数
	fclose(fp);
	printf("保存成功！\n");
}

/*
* 读取文件后自动插入家谱成员
*/
Status InsertNode_auto(CSTree& T, char name[], char birth[], char parentName[]) {
	CSTree parent = NULL;
	CSTree child = NULL;
	//如果插入的不是第一代祖先或无法查找到父亲，返回失败
	if (strcmp(parentName, "无") != 0 && (parent = Search(T, parentName)) == NULL) {
		return ERROR;
	}
	if ((child = CreatNode(name, birth, parentName)) == NULL) {	//创建结点
		return ERROR;
	}
	if (parent == NULL && strcmp(parentName, "无") == 0) {	//插入祖先结点
		child->data.generation = 1;
		T = child;
		return OK;
	}
	child->data.generation = parent->data.generation + 1;	//计算成员辈分
	child->nextSibling = parent->firstChild;	//使用头插法，时间复杂度O(1)
	parent->firstChild = child;
	//if (parent->firstChild == NULL) {
	//	parent->firstChild = child;
	//}
	//else {
	//	CSTree node = parent->firstChild;
	//	while (node->nextSibling != NULL) {
	//		node = node->nextSibling;			//尾插法，时间复杂度O(n)
	//	}
	//	node->nextSibling = child;
	//}
	return OK;
}

/*
* 查找成员
*/
CSTree Search(CSTree T, char name[]) {
	//result为目标结点
	CSTNode* result = NULL;
	if (NULL == T) {
		return NULL;
	}
	//通过名字查找，当姓名对应，则返回该结点
	if (strcmp(T->data.name, name) == 0) {
		return T;
	}		//深度优先遍历
	if ((result = Search(T->firstChild, name)) != NULL) {
		return result;
	}
	return Search(T->nextSibling, name);
}

/*
* 创建结点
*/
CSTree CreatNode(char name[], char birth[], char parent[]) {
	CSTree p = (CSTree)malloc(sizeof(CSTNode));
	if (NULL == p) {
		return NULL;
	}
	strcpy(p->data.name, name);		//初始化信息
	strcpy(p->data.birth, birth);
	strcpy(p->data.parent, parent);
	p->firstChild = NULL;
	p->nextSibling = NULL;
	return p;
}

/*
* 插入家谱成员
*/
Status InsertNode(CSTree &T,char parentName[]) {
	CSTree parent = NULL;
	CSTree child = NULL;
	char name[MAX];
	char birth[MAX];
	//如果插入的不是第一代祖先或无法查找到父亲
	if (strcmp(parentName, "无") != 0 && (parent = Search(T, parentName)) == NULL) {
		printf("家谱中无该成员\n");
		return ERROR;
	}
	printf("请输入新增成员的姓名 :");
	scanf("%s", name);
	while ((child = Search(T, name)) != NULL) {
		printf("该成员已存在，请重新输入姓名 :");
		scanf("%s", name);
	}
	printf("请输入新增成员的出生日期 :");
	scanf("%s", birth);
	if ((child = CreatNode(name, birth, parentName)) == NULL) {
		printf("插入失败\n");
		return ERROR;
	}
	if (parent == NULL && strcmp(parentName, "无") == 0) {	//插入祖先结点
		child->data.generation =  1;
		T = child;
		printf("插入成功！\n");
		return OK;
	}
	child->data.generation = parent->data.generation + 1;	//计算辈分
	child->nextSibling = parent->firstChild;
	parent->firstChild = child;		//头插法，时间复杂度为O(1)
	printf("插入成功！\n");
	return OK;
}

/*
* 修改成员信息
*/
Status ModifyNode(CSTree T, char name[]) {
	CSTree node = Search(T, name);
	if (node == NULL) {		//若未查找到该成员
		printf("家谱中无该成员\n");
		return ERROR;
	}
	printf("原姓名:%s\t", node->data.name);
	printf("现姓名:");
	scanf("%s", node->data.name);
	printf("原出生日期:%s\t", node->data.birth);
	printf("现出生日期:");
	scanf("%s", node->data.birth);
	printf("修改完成！\n");
	return OK;
}

/*
* 删除成员
*/
Status DeleteNode(CSTree &T, char name[]) {
	CSTree node = Search(T, name);
	if (NULL == node) {		//若未查找到该成员
		printf("家谱中无该成员\n");
		return ERROR;
	}
	//将其从双亲结点的孩子域中的兄弟域中删除
	CSTree parent = Search(T, node->data.parent);
	if (node == parent->firstChild) {
		parent->firstChild = node->nextSibling;
	}
	else {
		parent = parent->firstChild;
		while (parent != NULL) {
			if (parent->nextSibling == node) {
				parent->nextSibling = node->nextSibling;
				break;
			}
			parent = parent->nextSibling;
		}
	}
	//删除成员并将其子孙后代都删除
	DestoryTree(node);
	printf("删除成功！\n");
	return OK;
}

/*
* 销毁家谱树
*/
void DestoryTree(CSTree& T) {
	if (NULL == T) {
		return;
	}
	if (T->firstChild != NULL) {
		DestoryTree(T->firstChild);
	}
	if (T->nextSibling != NULL) {
		DestoryTree(T->nextSibling);
	}
	free(T);
	T = NULL;
}

/*
* 判断两个人的家族关系
*/
void Relationship(CSTree T, char name1[], char name2[]) {
	CSTree p1 = Search(T, name1);
	CSTree p2 = Search(T, name2);
	//不能为空且不能是同一个人
	if (p1 == NULL) printf("家族中无 %s 此成员\n", name1);
	else if (p2 == NULL) printf("家族中无 %s 此成员\n", name2);
	else if (p1 == p2) printf("同一个人\n");
	else {
		printf("二者的关系为：");
		if (p1->data.generation == p2->data.generation) {	//若二者辈分相同
			if (strcmp(p1->data.parent, p2->data.parent)==0) {
				printf("%s 和 %s 是亲兄弟\n", name1, name2);
			}
			else {
				printf("%s 和 %s 是堂兄弟\n", name1, name2);
			}
		}
		else {		//若辈分不同
			if (p1->data.generation < p2->data.generation) {	//若p1辈分大
				if (p2->data.generation - p1->data.generation == 1) {	//大一辈
					if (strcmp(name1, p2->data.parent) == 0) {
						printf("%s 是 %s 的父亲\n", name1, name2);
					}
					else {
						printf("%s 是 %s 的叔叔（伯伯）\n", name1, name2);
					}
				}
				else {
					printf("%s 是 %s 大 %d 代的长辈\n", name1, name2, p2->data.generation - p1->data.generation);
				}
			}
			else {		//若p2辈分大
				if (p1->data.generation - p2->data.generation == 1) {	//大一辈
					if (strcmp(p1->data.parent, name2) == 0) {
						printf("%s 是 %s 的父亲\n", name2, name1);
					}
					else {
						printf("%s 是 %s 的叔叔（伯伯）\n", name2, name1);
					}
				}
				else {
					printf("%s 是 %s 大 %d 代的长辈\n", name2, name1, p1->data.generation - p2->data.generation);
				}
			}
		}
	}
}

/*
* 判断两个成员是否属于直系或旁系三代关系
*/
void GenerationRelationship(CSTree T, char name1[], char name2[]) {
	CSTree p1 = Search(T, name1);
	CSTree p2 = Search(T, name2);
	//不能为空且不能是同一个人
	if (p1 == NULL) printf("家族中无 %s 此成员\n", name1);
	else if (p2 == NULL) printf("家族中无 %s 此成员\n", name2);
	else if (p1 == p2) printf("同一个人\n");
	else {		//若辈分相同，则必定不是直系且必定是旁系三代关系
		if (p1->data.generation == p2->data.generation) {	
			printf("是（二者为旁系三代关系）\n");
		}
		else {
			//从辈分小的成员向上查找
			if (p1->data.generation > p2->data.generation) {
				CSTree ancestor = Search(T, p1->data.parent);
				//若查找到另一成员，则为直系，否则为判断是否为旁系三代关系
				while (ancestor != NULL) {
					if (ancestor == p2) {
						printf("是（二者为直系亲属）\n");
						return;
					}
					ancestor = Search(T, ancestor->data.parent);
				}
				if (p1->data.generation - p2->data.generation == 1) {
					printf("是（二者为旁系三代关系）\n");
				}
				else {
					printf("不是\n");
				}
			}
			else {
				CSTree ancestor = Search(T, p2->data.parent);
				//若查找到另一成员，则为直系，否则为判断是否为旁系三代关系
				while (ancestor != NULL) {
					if (ancestor == p1) {
						printf("是（二者为直系亲属）\n");
						return;
					}
					ancestor = Search(T, ancestor->data.parent);
				}
				if (p2->data.generation - p1->data.generation == 1) {
					printf("是（二者为旁系三代关系）\n");
				}
				else {
					printf("不是\n");
				}
			}
		}
	}
}

/*
* 查找成员的子孙
*/
void Descendant(CSTree T, char name[]) {
	CSTree p = Search(T, name);
	if (NULL == p) {
		printf("家谱中无该成员\n");
		return;
	}
	printf("以下为%s的子孙（隔代换行显示）\n", name);
	LevelOrderTraverse(p);		//层次遍历该结点
}

/*
* 查找成员的祖先
*/
void Ancestor(CSTree T, char name[]) {
	CSTree p = Search(T, name);
	if (NULL == p) {
		printf("家谱中无该成员\n");
		return;
	}
	if (strcmp(p->data.parent, "无") == 0) {
		printf("家谱中无该成员的祖先\n");
		return;
	}
	printf("祖先路径为： %s", name);
	p = Search(T, p->data.parent);
	//非递归：循环向上查找输出	||	也可使用递归：先查找到最顶端再输出
	while (p != NULL) {
		printf("<- %s ", p->data.name);
		p = Search(T, p->data.parent);
	}
}

/*
* 查询成员的堂兄弟
*/
void Cousin(CSTree T, char name[]) {
	CSTree p0 = Search(T, name);
	CSTree cousin, p;
	if (NULL == p0) {
		printf("家谱中无该成员\n");
		return;
	}
	if (strcmp(p0->data.parent, "无") == 0) {
		printf("家谱中无该成员的堂兄弟\n");
		return;
	}
	p0 = Search(T, p0->data.parent);//p0为该成员父亲
	if (strcmp(p0->data.parent, "无") == 0) {
		printf("家谱中无该成员的堂兄弟\n");
		return;
	}
	p = Search(T, p0->data.parent);	//查询该成员父亲的父亲
	p = p->firstChild;				//从第一个孩子开始查询
	printf("该成员的堂兄弟有：");
	while (p != NULL) {
		if (p == p0) {		//若p为该成员父亲，则跳过
			p = p->nextSibling;
			continue;
		}
		cousin = p->firstChild;
		while (cousin != NULL) {
			printf("%s  ", cousin->data.name);
			cousin = cousin->nextSibling;
		}
		p = p->nextSibling;
	}
}

/*
* 层次遍历（思想：从队列取出结点并打印，如果该结点的子节点不为空，则存入队列）重复直至队列为空
*/
void LevelOrderTraverse(CSTree T) {
	if (NULL == T) {
		printf("此树为空！\n");
		return;
	}
	//sum为子代有多少人，i为第几代，newLine用来判断是否需要换行（隔代换行）
	int i = 1, sum = 0;
	int newLine = 0;
	CSTree p = T;
	LQueue Q;
	InitQueue(Q);
	printf("第1代：%s", p->data.name);
	p = p->firstChild;
	while (p != NULL) {
		sum++;
		EnQueue(Q, p);
		p = p->nextSibling;
	}
	while (OK==DeQueue(Q, p)) {		//直到队列为空
		if (newLine == 0) {			//当遍历完一代人则换行
			printf("\n第%d代：",++i);
			newLine = sum - 1;
			sum = 0;
		}
		else {
			newLine--;
		}
		printf("%s  ",p->data.name);
		p = p->firstChild;
		while (p != NULL) {
			sum++;					//统计一代人的人数
			EnQueue(Q, p);
			p = p->nextSibling;
		}
	}
}

/*
* 凹入表形式打印家谱的树形结构
*/
void PrintTree(CSTree T) {
	int i;
	CSTree p;
	if (NULL == T) {
		return;
	}
	for (i = 0; i < T->data.generation * 5; i++) {
		printf(" ");	//缩进代表不同深度，即不同辈分
	}
	printf("|%s|\n", T->data.name);
	for (p = T->firstChild; p != NULL; p = p->nextSibling) {
		PrintTree(p);
	}
}

/*
* 输出该成员信息
*/
Status visit(CSTree node) {
	if (NULL == node) {
		printf("无该成员信息！\n");
		return ERROR;
	}
	printf("姓名:%s\t", node->data.name);
	printf("出生日期:%s\t", node->data.birth);
	printf("父亲姓名:%s\t", node->data.parent);
	printf("辈分:第%d代\n", node->data.generation);
	return OK;
}

/**
 * 构造一个空队列Q
 */
void InitQueue(LQueue& Q) {
	Q.front = Q.rear = NULL;
}

/**
 * 入队：在队列Q的队尾插入元素key
 */
Status EnQueue(LQueue& Q, CSTNode* node) {
	if (NULL == node) {
		return ERROR;
	}
	LQNode* p;
	p = (LQNode*)malloc(sizeof(LQNode));
	if (NULL == p) {
		return ERROR;
	}
	p->data = node;
	p->next = NULL;
	if (NULL == Q.front) {
		Q.front = p;
	}
	else {
		Q.rear->next = p;
	}
	Q.rear = p;
	return OK;
}

/**
 * 出队：若队列Q非空，则删除队头元素，用key返回其值，否则返回ERROR
 */
Status DeQueue(LQueue& Q, CSTree& node) {
	LQNode* p;
	if (NULL == Q.front) {
		return ERROR;
	}
	p = Q.front;
	node = p->data;
	Q.front = p->next;
	if (Q.rear == p) {
		Q.rear = NULL;
	}
	free(p);
	return OK;
}