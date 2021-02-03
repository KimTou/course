#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FamilyTree.h"

/*
* ��ȡ�ļ���ʼ��������
*/
CSTree InitTree() {
	int i,n;
	CSTree T = NULL;
	char name[MAX];
	char birth[MAX];
	char parentName[MAX];
	FILE* fp;	//ָ���ļ������ݵ�ָ�����
	if ((fp = fopen("family.txt", "r")) == NULL) {	//�Զ��뷽ʽ��family�ı��ļ�
		printf("open file error!\n");
		exit(0);
	}
	fscanf(fp, "%d", &n);
	for (i = 0; i < n; i++) {	//��˳������Ա��Ϣ��������������
		fscanf(fp, "%s%s%s", name, birth, parentName);
		InsertNode_auto(T, name, birth, parentName);
	}
	fclose(fp);
	printf("��ȡ��ɣ�\n");
	return T;
}

/*
* �½������ļ�
*/
void NewFile() {
	int i, n;
	char name[MAX];
	char birth[MAX];
	char parentName[MAX];
	FILE* fp;	//ָ���ļ������ݵ�ָ�����
	CSTree p = (CSTree)malloc(sizeof(CSTNode));
	if ((fp = fopen("family.txt", "w")) == NULL){	//��д��ķ�ʽ��family�ı��ļ�
		printf("open file error!\n");
		exit(0);
	}
	printf("����������������");
	scanf("%d", &n);
	while (n < 0 || n > 99) {
		printf("������������ֵ��0-99��\n");
		printf("����������������");
		scanf("%d", &n);
	}
	fprintf(fp, "%d ", n);
	printf("���ڿ�ʼ������׳�Ա��Ϣ����1λ��Ա������������ȣ��丸������Ϊ���ޡ���\n\n");
	for (i = 1; i <= n; i++) {
		printf("�������%d����Ա���������������ڡ�����������", i);
		scanf("%s%s%s", name, birth, parentName);
		fprintf(fp, "%s %s %s ", name, birth, parentName);
	}
	fclose(fp);
	printf("�½���ɣ�\n");
}

/*
* ������������ļ�
*/
void SaveFile(CSTree T) {
	if (NULL == T) {
		printf("����Ϊ�գ�\n");
		return;
	}
	int i = 1, sum = 0;
	CSTree p = T;
	LQueue Q;
	InitQueue(Q);
	FILE* fp;	//ָ���ļ������ݵ�ָ�����
	if ((fp = fopen("family.txt", "w")) == NULL)	//��д��ķ�ʽ��family�ı��ļ�
	{
		printf("open file error!\n");
		exit(0);
	}
	fprintf(fp, "    ");	//Ϊ����������Ԥ��λ��
	while (p != NULL) {
		sum++;
		EnQueue(Q, p);
		p = p->nextSibling;
	}
	while (OK == DeQueue(Q, p)) {		//ֱ������Ϊ��
		fprintf(fp, "%s %s %s ", p->data.name, p->data.birth, p->data.parent);
		p = p->firstChild;
		while (p != NULL) {
			sum++;					//ͳ��һ���˵�����
			EnQueue(Q, p);
			p = p->nextSibling;
		}
	}
	rewind(fp);		//�ļ�ָ���ƻص��ļ�ͷ
	fprintf(fp, "%d ", sum);	//д���Ա����
	fclose(fp);
	printf("����ɹ���\n");
}

/*
* ��ȡ�ļ����Զ�������׳�Ա
*/
Status InsertNode_auto(CSTree& T, char name[], char birth[], char parentName[]) {
	CSTree parent = NULL;
	CSTree child = NULL;
	//�������Ĳ��ǵ�һ�����Ȼ��޷����ҵ����ף�����ʧ��
	if (strcmp(parentName, "��") != 0 && (parent = Search(T, parentName)) == NULL) {
		return ERROR;
	}
	if ((child = CreatNode(name, birth, parentName)) == NULL) {	//�������
		return ERROR;
	}
	if (parent == NULL && strcmp(parentName, "��") == 0) {	//�������Ƚ��
		child->data.generation = 1;
		T = child;
		return OK;
	}
	child->data.generation = parent->data.generation + 1;	//�����Ա����
	child->nextSibling = parent->firstChild;	//ʹ��ͷ�巨��ʱ�临�Ӷ�O(1)
	parent->firstChild = child;
	//if (parent->firstChild == NULL) {
	//	parent->firstChild = child;
	//}
	//else {
	//	CSTree node = parent->firstChild;
	//	while (node->nextSibling != NULL) {
	//		node = node->nextSibling;			//β�巨��ʱ�临�Ӷ�O(n)
	//	}
	//	node->nextSibling = child;
	//}
	return OK;
}

/*
* ���ҳ�Ա
*/
CSTree Search(CSTree T, char name[]) {
	//resultΪĿ����
	CSTNode* result = NULL;
	if (NULL == T) {
		return NULL;
	}
	//ͨ�����ֲ��ң���������Ӧ���򷵻ظý��
	if (strcmp(T->data.name, name) == 0) {
		return T;
	}		//������ȱ���
	if ((result = Search(T->firstChild, name)) != NULL) {
		return result;
	}
	return Search(T->nextSibling, name);
}

/*
* �������
*/
CSTree CreatNode(char name[], char birth[], char parent[]) {
	CSTree p = (CSTree)malloc(sizeof(CSTNode));
	if (NULL == p) {
		return NULL;
	}
	strcpy(p->data.name, name);		//��ʼ����Ϣ
	strcpy(p->data.birth, birth);
	strcpy(p->data.parent, parent);
	p->firstChild = NULL;
	p->nextSibling = NULL;
	return p;
}

/*
* ������׳�Ա
*/
Status InsertNode(CSTree &T,char parentName[]) {
	CSTree parent = NULL;
	CSTree child = NULL;
	char name[MAX];
	char birth[MAX];
	//�������Ĳ��ǵ�һ�����Ȼ��޷����ҵ�����
	if (strcmp(parentName, "��") != 0 && (parent = Search(T, parentName)) == NULL) {
		printf("�������޸ó�Ա\n");
		return ERROR;
	}
	printf("������������Ա������ :");
	scanf("%s", name);
	while ((child = Search(T, name)) != NULL) {
		printf("�ó�Ա�Ѵ��ڣ��������������� :");
		scanf("%s", name);
	}
	printf("������������Ա�ĳ������� :");
	scanf("%s", birth);
	if ((child = CreatNode(name, birth, parentName)) == NULL) {
		printf("����ʧ��\n");
		return ERROR;
	}
	if (parent == NULL && strcmp(parentName, "��") == 0) {	//�������Ƚ��
		child->data.generation =  1;
		T = child;
		printf("����ɹ���\n");
		return OK;
	}
	child->data.generation = parent->data.generation + 1;	//���㱲��
	child->nextSibling = parent->firstChild;
	parent->firstChild = child;		//ͷ�巨��ʱ�临�Ӷ�ΪO(1)
	printf("����ɹ���\n");
	return OK;
}

/*
* �޸ĳ�Ա��Ϣ
*/
Status ModifyNode(CSTree T, char name[]) {
	CSTree node = Search(T, name);
	if (node == NULL) {		//��δ���ҵ��ó�Ա
		printf("�������޸ó�Ա\n");
		return ERROR;
	}
	printf("ԭ����:%s\t", node->data.name);
	printf("������:");
	scanf("%s", node->data.name);
	printf("ԭ��������:%s\t", node->data.birth);
	printf("�ֳ�������:");
	scanf("%s", node->data.birth);
	printf("�޸���ɣ�\n");
	return OK;
}

/*
* ɾ����Ա
*/
Status DeleteNode(CSTree &T, char name[]) {
	CSTree node = Search(T, name);
	if (NULL == node) {		//��δ���ҵ��ó�Ա
		printf("�������޸ó�Ա\n");
		return ERROR;
	}
	//�����˫�׽��ĺ������е��ֵ�����ɾ��
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
	//ɾ����Ա��������������ɾ��
	DestoryTree(node);
	printf("ɾ���ɹ���\n");
	return OK;
}

/*
* ���ټ�����
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
* �ж������˵ļ����ϵ
*/
void Relationship(CSTree T, char name1[], char name2[]) {
	CSTree p1 = Search(T, name1);
	CSTree p2 = Search(T, name2);
	//����Ϊ���Ҳ�����ͬһ����
	if (p1 == NULL) printf("�������� %s �˳�Ա\n", name1);
	else if (p2 == NULL) printf("�������� %s �˳�Ա\n", name2);
	else if (p1 == p2) printf("ͬһ����\n");
	else {
		printf("���ߵĹ�ϵΪ��");
		if (p1->data.generation == p2->data.generation) {	//�����߱�����ͬ
			if (strcmp(p1->data.parent, p2->data.parent)==0) {
				printf("%s �� %s �����ֵ�\n", name1, name2);
			}
			else {
				printf("%s �� %s �����ֵ�\n", name1, name2);
			}
		}
		else {		//�����ֲ�ͬ
			if (p1->data.generation < p2->data.generation) {	//��p1���ִ�
				if (p2->data.generation - p1->data.generation == 1) {	//��һ��
					if (strcmp(name1, p2->data.parent) == 0) {
						printf("%s �� %s �ĸ���\n", name1, name2);
					}
					else {
						printf("%s �� %s �����壨������\n", name1, name2);
					}
				}
				else {
					printf("%s �� %s �� %d ���ĳ���\n", name1, name2, p2->data.generation - p1->data.generation);
				}
			}
			else {		//��p2���ִ�
				if (p1->data.generation - p2->data.generation == 1) {	//��һ��
					if (strcmp(p1->data.parent, name2) == 0) {
						printf("%s �� %s �ĸ���\n", name2, name1);
					}
					else {
						printf("%s �� %s �����壨������\n", name2, name1);
					}
				}
				else {
					printf("%s �� %s �� %d ���ĳ���\n", name2, name1, p1->data.generation - p2->data.generation);
				}
			}
		}
	}
}

/*
* �ж�������Ա�Ƿ�����ֱϵ����ϵ������ϵ
*/
void GenerationRelationship(CSTree T, char name1[], char name2[]) {
	CSTree p1 = Search(T, name1);
	CSTree p2 = Search(T, name2);
	//����Ϊ���Ҳ�����ͬһ����
	if (p1 == NULL) printf("�������� %s �˳�Ա\n", name1);
	else if (p2 == NULL) printf("�������� %s �˳�Ա\n", name2);
	else if (p1 == p2) printf("ͬһ����\n");
	else {		//��������ͬ����ض�����ֱϵ�ұض�����ϵ������ϵ
		if (p1->data.generation == p2->data.generation) {	
			printf("�ǣ�����Ϊ��ϵ������ϵ��\n");
		}
		else {
			//�ӱ���С�ĳ�Ա���ϲ���
			if (p1->data.generation > p2->data.generation) {
				CSTree ancestor = Search(T, p1->data.parent);
				//�����ҵ���һ��Ա����Ϊֱϵ������Ϊ�ж��Ƿ�Ϊ��ϵ������ϵ
				while (ancestor != NULL) {
					if (ancestor == p2) {
						printf("�ǣ�����Ϊֱϵ������\n");
						return;
					}
					ancestor = Search(T, ancestor->data.parent);
				}
				if (p1->data.generation - p2->data.generation == 1) {
					printf("�ǣ�����Ϊ��ϵ������ϵ��\n");
				}
				else {
					printf("����\n");
				}
			}
			else {
				CSTree ancestor = Search(T, p2->data.parent);
				//�����ҵ���һ��Ա����Ϊֱϵ������Ϊ�ж��Ƿ�Ϊ��ϵ������ϵ
				while (ancestor != NULL) {
					if (ancestor == p1) {
						printf("�ǣ�����Ϊֱϵ������\n");
						return;
					}
					ancestor = Search(T, ancestor->data.parent);
				}
				if (p2->data.generation - p1->data.generation == 1) {
					printf("�ǣ�����Ϊ��ϵ������ϵ��\n");
				}
				else {
					printf("����\n");
				}
			}
		}
	}
}

/*
* ���ҳ�Ա������
*/
void Descendant(CSTree T, char name[]) {
	CSTree p = Search(T, name);
	if (NULL == p) {
		printf("�������޸ó�Ա\n");
		return;
	}
	printf("����Ϊ%s���������������ʾ��\n", name);
	LevelOrderTraverse(p);		//��α����ý��
}

/*
* ���ҳ�Ա������
*/
void Ancestor(CSTree T, char name[]) {
	CSTree p = Search(T, name);
	if (NULL == p) {
		printf("�������޸ó�Ա\n");
		return;
	}
	if (strcmp(p->data.parent, "��") == 0) {
		printf("�������޸ó�Ա������\n");
		return;
	}
	printf("����·��Ϊ�� %s", name);
	p = Search(T, p->data.parent);
	//�ǵݹ飺ѭ�����ϲ������	||	Ҳ��ʹ�õݹ飺�Ȳ��ҵ���������
	while (p != NULL) {
		printf("<- %s ", p->data.name);
		p = Search(T, p->data.parent);
	}
}

/*
* ��ѯ��Ա�����ֵ�
*/
void Cousin(CSTree T, char name[]) {
	CSTree p0 = Search(T, name);
	CSTree cousin, p;
	if (NULL == p0) {
		printf("�������޸ó�Ա\n");
		return;
	}
	if (strcmp(p0->data.parent, "��") == 0) {
		printf("�������޸ó�Ա�����ֵ�\n");
		return;
	}
	p0 = Search(T, p0->data.parent);//p0Ϊ�ó�Ա����
	if (strcmp(p0->data.parent, "��") == 0) {
		printf("�������޸ó�Ա�����ֵ�\n");
		return;
	}
	p = Search(T, p0->data.parent);	//��ѯ�ó�Ա���׵ĸ���
	p = p->firstChild;				//�ӵ�һ�����ӿ�ʼ��ѯ
	printf("�ó�Ա�����ֵ��У�");
	while (p != NULL) {
		if (p == p0) {		//��pΪ�ó�Ա���ף�������
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
* ��α�����˼�룺�Ӷ���ȡ����㲢��ӡ������ý����ӽڵ㲻Ϊ�գ��������У��ظ�ֱ������Ϊ��
*/
void LevelOrderTraverse(CSTree T) {
	if (NULL == T) {
		printf("����Ϊ�գ�\n");
		return;
	}
	//sumΪ�Ӵ��ж����ˣ�iΪ�ڼ�����newLine�����ж��Ƿ���Ҫ���У��������У�
	int i = 1, sum = 0;
	int newLine = 0;
	CSTree p = T;
	LQueue Q;
	InitQueue(Q);
	printf("��1����%s", p->data.name);
	p = p->firstChild;
	while (p != NULL) {
		sum++;
		EnQueue(Q, p);
		p = p->nextSibling;
	}
	while (OK==DeQueue(Q, p)) {		//ֱ������Ϊ��
		if (newLine == 0) {			//��������һ��������
			printf("\n��%d����",++i);
			newLine = sum - 1;
			sum = 0;
		}
		else {
			newLine--;
		}
		printf("%s  ",p->data.name);
		p = p->firstChild;
		while (p != NULL) {
			sum++;					//ͳ��һ���˵�����
			EnQueue(Q, p);
			p = p->nextSibling;
		}
	}
}

/*
* �������ʽ��ӡ���׵����νṹ
*/
void PrintTree(CSTree T) {
	int i;
	CSTree p;
	if (NULL == T) {
		return;
	}
	for (i = 0; i < T->data.generation * 5; i++) {
		printf(" ");	//��������ͬ��ȣ�����ͬ����
	}
	printf("|%s|\n", T->data.name);
	for (p = T->firstChild; p != NULL; p = p->nextSibling) {
		PrintTree(p);
	}
}

/*
* ����ó�Ա��Ϣ
*/
Status visit(CSTree node) {
	if (NULL == node) {
		printf("�޸ó�Ա��Ϣ��\n");
		return ERROR;
	}
	printf("����:%s\t", node->data.name);
	printf("��������:%s\t", node->data.birth);
	printf("��������:%s\t", node->data.parent);
	printf("����:��%d��\n", node->data.generation);
	return OK;
}

/**
 * ����һ���ն���Q
 */
void InitQueue(LQueue& Q) {
	Q.front = Q.rear = NULL;
}

/**
 * ��ӣ��ڶ���Q�Ķ�β����Ԫ��key
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
 * ���ӣ�������Q�ǿգ���ɾ����ͷԪ�أ���key������ֵ�����򷵻�ERROR
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