#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define    OK 1
#define ERROR 0

typedef int Status;
typedef int KeyType;

#define m 3                    //B���Ľף��˴���Ϊ3
//=============B���洢�ṹ=================
typedef struct {
    KeyType key;
    char data;
}Record;

typedef struct BTNode {
    int keynum;                    //��㵱ǰ�Ĺؼ��ָ���
    KeyType key[m + 1];            //�ؼ������飬key[0]δ��
    struct BTNode* parent;        //˫�׽��ָ��
    struct BTNode* ptr[m + 1];    //���ӽ��ָ������
    Record* recptr[m + 1];        //��¼ָ��������0�ŵ�Ԫδ��
} BTNode, * BTree;                //B���Ľ�㼰ָ������

typedef struct {
    BTree pt;        //ָ���ҵ��Ľ��
    int i;            //1 <= i <= m ���ڽ���еĹؼ���λ��
    int tag;        //1:���ҳɹ���0:����ʧ��
}result;            //B���Ĳ��ҽ������

//=============�����д洢�ṹ===========
typedef struct LQNode {    
    BTNode* data;
    struct LQNode* next;
}LQNode,*QueuePtr;
typedef struct {
    QueuePtr front;        //��ͷָ��
    QueuePtr rear;        //��βָ��
}LQueue;

//=================B�����������ĺ�������==============
void SearchBTree(BTree t, int k, result& r);    //���ҹؼ���
int Search(BTree p, int k);                    //���عؼ���λ��
void InsertBTree(BTree& t, int k, BTree q, int i);    //����ؼ���
void split(BTree& q, int s, BTree& ap);            //��q�����ѳ�������㣬ǰһ�뱣����ԭ����У���һ������ap��ָ�½��
void newRoot(BTree& t, BTree p, int x, BTree ap);    //�����µĸ����
void Insert(BTree& q, int i, int x, BTree ap);    //�ؼ���x���½��ָ��ap�ֱ���뵽q->key[i]��q->ptr[i]
void DeleteBTree(BTree& t, BTree& p, int i);    //ɾ���ؼ���
void Successor(BTree& p, int i);            //��Ai�������ҳ����²���ն˽�����С�ؼ���Ki
void Remove(BTree& p, int i);                //�ӽ��p��ɾ��key[i]
void Restore(BTree& t, BTree& p, int i);    //����B��
void DestroyBTree(BTree& t);            //����B��
int CountKeyNums(BTree t);                //����ؼ��ָ���
void PrintBTree(BTree t, int tab);        //�������ʽ��ӡB��
void PreOrderTraverse(BTree t);            //�������B��
void InOrderTraverse(BTree t);            //�������B��
void LevelOrderTraverse(BTree t);            //��α���B��
BTree InitBTree(int num);                    //�������һ�Źؼ��ָ���Ϊnum��B��


//===============�����л��������ĺ�������==========
void InitQueue(LQueue& Q);        //����һ���ն���Q
Status EnQueue(LQueue& Q, BTNode* ptr);        //�ڶ���Q�Ķ�β����Ԫ��key
Status DeQueue(LQueue& Q, BTree& ptr);    //������Q�ǿգ���ɾ����ͷԪ�أ���key������ֵ�����򷵻�ERROR
void DestroyQueue(LQueue& Q);        //���ٶ���


#endif
