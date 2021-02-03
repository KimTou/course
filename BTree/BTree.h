#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define    OK 1
#define ERROR 0

typedef int Status;
typedef int KeyType;

#define m 3                    //B树的阶，此处设为3
//=============B树存储结构=================
typedef struct {
    KeyType key;
    char data;
}Record;

typedef struct BTNode {
    int keynum;                    //结点当前的关键字个数
    KeyType key[m + 1];            //关键字数组，key[0]未用
    struct BTNode* parent;        //双亲结点指针
    struct BTNode* ptr[m + 1];    //孩子结点指针数组
    Record* recptr[m + 1];        //记录指针向量，0号单元未用
} BTNode, * BTree;                //B树的结点及指针类型

typedef struct {
    BTree pt;        //指向找到的结点
    int i;            //1 <= i <= m ，在结点中的关键字位序
    int tag;        //1:查找成功，0:查找失败
}result;            //B树的查找结果类型

//=============链队列存储结构===========
typedef struct LQNode {    
    BTNode* data;
    struct LQNode* next;
}LQNode,*QueuePtr;
typedef struct {
    QueuePtr front;        //队头指针
    QueuePtr rear;        //队尾指针
}LQueue;

//=================B树基本操作的函数声明==============
void SearchBTree(BTree t, int k, result& r);    //查找关键字
int Search(BTree p, int k);                    //返回关键字位置
void InsertBTree(BTree& t, int k, BTree q, int i);    //插入关键字
void split(BTree& q, int s, BTree& ap);            //将q结点分裂成两个结点，前一半保留在原结点中，后一半移入ap所指新结点
void newRoot(BTree& t, BTree p, int x, BTree ap);    //生成新的根结点
void Insert(BTree& q, int i, int x, BTree ap);    //关键字x和新结点指针ap分别插入到q->key[i]和q->ptr[i]
void DeleteBTree(BTree& t, BTree& p, int i);    //删除关键字
void Successor(BTree& p, int i);            //在Ai子树中找出最下层非终端结点的最小关键字Ki
void Remove(BTree& p, int i);                //从结点p中删除key[i]
void Restore(BTree& t, BTree& p, int i);    //调整B树
void DestroyBTree(BTree& t);            //销毁B树
int CountKeyNums(BTree t);                //计算关键字个数
void PrintBTree(BTree t, int tab);        //凹入表形式打印B树
void PreOrderTraverse(BTree t);            //先序遍历B树
void InOrderTraverse(BTree t);            //中序遍历B树
void LevelOrderTraverse(BTree t);            //层次遍历B树
BTree InitBTree(int num);                    //随机生成一颗关键字个数为num的B树


//===============链队列基本操作的函数声明==========
void InitQueue(LQueue& Q);        //构造一个空队列Q
Status EnQueue(LQueue& Q, BTNode* ptr);        //在队列Q的队尾插入元素key
Status DeQueue(LQueue& Q, BTree& ptr);    //若队列Q非空，则删除队头元素，用key返回其值，否则返回ERROR
void DestroyQueue(LQueue& Q);        //销毁队列


#endif
