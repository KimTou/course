#ifndef FAMILYTREE_H_INCLUDED
#define FAMILYTREE_H_INCLUDED

#define MAX 20
#define OK 1
#define ERROR 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef  int  Status;

//===============树存储结构================
typedef struct DataType {	//存放个人信息
	char name[MAX];		//姓名
	char birth[MAX];	//出生日期
	char parent[MAX];	//父亲
	int generation;		//代
}DataType;

typedef struct CSTNode {
	DataType data;		//数据域
	struct CSTNode* firstChild, * nextSibling;	//最左孩子指针、右兄弟指针
}CSTNode, * CSTree;		//孩子兄弟表示法

//=============链队列存储结构===========
typedef struct LQNode {
	CSTNode* data;
	struct LQNode* next;
}LQNode, * QueuePtr;

typedef struct {
	QueuePtr front;		//队头指针
	QueuePtr rear;		//队尾指针
}LQueue;

//============ 家谱函数接口 ===============
void NewFile();		//新建家谱文件
CSTree InitTree();	//读取文件初始化家谱树
void SaveFile(CSTree T);	//保存家谱树到文件
Status InsertNode_auto(CSTree& T, char name[], char birth[], char parentName[]);//读取文件后自动插入家谱成员
CSTree Search(CSTree T, char name[]);	//查找成员
CSTree CreatNode(char name[],char birth[],char parent[]);	//创建结点
Status InsertNode(CSTree &T,char parentName[]);//插入家谱成员
Status ModifyNode(CSTree T, char name[]);	//修改成员信息
void DestoryTree(CSTree &T);		//销毁家谱树
Status DeleteNode(CSTree &T, char name[]);	//删除成员
void Relationship(CSTree T, char name1[], char name2[]);	//判断两个人的家族关系
void GenerationRelationship(CSTree T, char name1[], char name2[]);	//判断两个成员是否属于直系或旁系三代关系
void Descendant(CSTree T, char name[]);	//查询子孙关系
void Ancestor(CSTree T, char name[]);	//查询祖先关系
void Cousin(CSTree T, char name[]);		//查询堂兄弟关系
void LevelOrderTraverse(CSTree T);//层次遍历（思想：从队列取出结点并打印，如果该结点的子节点不为空，则存入队列）重复直至队列为空
void PrintTree(CSTree T);	//打印家谱的树形结构
Status visit(CSTree node);	//输出该成员信息

//===============链队列基本操作的函数声明==========

void InitQueue(LQueue& Q);		//构造一个空队列Q
Status EnQueue(LQueue& Q, CSTNode* node);		//在队列Q的队尾插入元素node
Status DeQueue(LQueue& Q, CSTree& node);	//若队列Q非空，则删除队头元素，用node返回其值，否则返回ERROR


#endif	