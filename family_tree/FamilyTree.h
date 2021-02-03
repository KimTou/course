#ifndef FAMILYTREE_H_INCLUDED
#define FAMILYTREE_H_INCLUDED

#define MAX 20
#define OK 1
#define ERROR 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef  int  Status;

//===============���洢�ṹ================
typedef struct DataType {	//��Ÿ�����Ϣ
	char name[MAX];		//����
	char birth[MAX];	//��������
	char parent[MAX];	//����
	int generation;		//��
}DataType;

typedef struct CSTNode {
	DataType data;		//������
	struct CSTNode* firstChild, * nextSibling;	//������ָ�롢���ֵ�ָ��
}CSTNode, * CSTree;		//�����ֵܱ�ʾ��

//=============�����д洢�ṹ===========
typedef struct LQNode {
	CSTNode* data;
	struct LQNode* next;
}LQNode, * QueuePtr;

typedef struct {
	QueuePtr front;		//��ͷָ��
	QueuePtr rear;		//��βָ��
}LQueue;

//============ ���׺����ӿ� ===============
void NewFile();		//�½������ļ�
CSTree InitTree();	//��ȡ�ļ���ʼ��������
void SaveFile(CSTree T);	//������������ļ�
Status InsertNode_auto(CSTree& T, char name[], char birth[], char parentName[]);//��ȡ�ļ����Զ�������׳�Ա
CSTree Search(CSTree T, char name[]);	//���ҳ�Ա
CSTree CreatNode(char name[],char birth[],char parent[]);	//�������
Status InsertNode(CSTree &T,char parentName[]);//������׳�Ա
Status ModifyNode(CSTree T, char name[]);	//�޸ĳ�Ա��Ϣ
void DestoryTree(CSTree &T);		//���ټ�����
Status DeleteNode(CSTree &T, char name[]);	//ɾ����Ա
void Relationship(CSTree T, char name1[], char name2[]);	//�ж������˵ļ����ϵ
void GenerationRelationship(CSTree T, char name1[], char name2[]);	//�ж�������Ա�Ƿ�����ֱϵ����ϵ������ϵ
void Descendant(CSTree T, char name[]);	//��ѯ�����ϵ
void Ancestor(CSTree T, char name[]);	//��ѯ���ȹ�ϵ
void Cousin(CSTree T, char name[]);		//��ѯ���ֵܹ�ϵ
void LevelOrderTraverse(CSTree T);//��α�����˼�룺�Ӷ���ȡ����㲢��ӡ������ý����ӽڵ㲻Ϊ�գ��������У��ظ�ֱ������Ϊ��
void PrintTree(CSTree T);	//��ӡ���׵����νṹ
Status visit(CSTree node);	//����ó�Ա��Ϣ

//===============�����л��������ĺ�������==========

void InitQueue(LQueue& Q);		//����һ���ն���Q
Status EnQueue(LQueue& Q, CSTNode* node);		//�ڶ���Q�Ķ�β����Ԫ��node
Status DeQueue(LQueue& Q, CSTree& node);	//������Q�ǿգ���ɾ����ͷԪ�أ���node������ֵ�����򷵻�ERROR


#endif	