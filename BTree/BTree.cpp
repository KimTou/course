#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTree.h"

/**
 * 随机生成一颗关键字个数为num的B树
 */
BTree InitBTree(int num) {
	BTree t = NULL;
	int i, data;
	result r;
	for (i = 0; i < num; i++) {
		data = rand() % 99;		//随机生成99以内的关键字
		SearchBTree(t, data, r);
		if (r.tag == 0) {		//插入不重复关键字
			InsertBTree(t, data, r.pt, r.i);
		}
		else {
			i--;
		}
	}
	return t;
}

/**
 * 若在B树中查找到对应关键字，则返回关键字所在结点指针以及所在结点关键字数组的下标，并用tag来标记是否查找到；
 * 若未查找到，则返回其应该插入的结点指针及其位置。
 */
void SearchBTree(BTree t, int k, result& r) {
	int i = 0, found = 0;
	BTree p = t, q = NULL;		//初始化，p指向待查结点，q指向待查结点的双亲
	while (p != NULL && found == 0) {
		i = Search(p, k);
		if (i <= p->keynum && p->key[i] == k) {
			found = 1;			//若查找到关键字
		}
		else {
			q = p;				
			p = p->ptr[i - 1];	//没查找到则指针下移
		}
	}
	if (found == 1) {			//查找到，则返回k所在的结点以及位置i
		r.pt = p;
		r.i = i;
		r.tag = 1;
	}
	else {						//没查找到，则返回k应该插入的q结点以及插入位置i
		r.pt = q;
		r.i = i;
		r.tag = 0;
	}
}

/**
 * 返回关键字位置
 */
int Search(BTree p, int k) {
	int i = 1;
	while (i <= p->keynum && p->key[i] < k) {
		i++;
	}
	return i;
}

/**
 * 将关键字k插入到B树t中q结点的key[i-1]和key[i]之间
 */
void InsertBTree(BTree& t, int k, BTree q, int i) {
	int x, s, finished = 0, needNewRoot = 0;
	BTree ap;
	if (NULL == q) {		//当q为空，则t也为空树，此时应创建根结点
		newRoot(t, NULL, k, NULL);
	}
	else {
		x = k;
		ap = NULL;		//插入最底层，所以ap一开始为NULL
		while (finished == 0 && needNewRoot == 0) {
			Insert(q, i, x, ap);
			if (q->keynum < m) {
				finished = 1;		//插入成功
			}
			else {			//结点q关键字数大于等于m，需进行分裂
				s = (m + 1) / 2;
				split(q, s, ap);
				x = q->key[s];
				if (q->parent != NULL) {
					q = q->parent;
					i = Search(q, x);		//在双亲结点中查找x插入的位置
				}
				else {
					needNewRoot = 1;
				}
			}
		}
		if (needNewRoot == 1) {			//若分裂到根结点，则需生成新根结点
			newRoot(t, q, x, ap);
		}
	}
}

/**
 * 关键字x和新结点指针ap分别插入到q->key[i]和q->ptr[i]
 */
void Insert(BTree& q, int i, int x, BTree ap) {
	int j;
	//当插入位置大于当前关键字数，不执行此循环，否则插入位置及以后的关键字后移一位
	for (j = q->keynum; j >= i; j--) {		
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i] = x;
	q->ptr[i] = ap;
	if (ap != NULL) {
		ap->parent = q;
	}
	q->keynum++;
}

/**
 * 生成新的根结点
 */
void newRoot(BTree& t, BTree p, int x, BTree ap) {
	t = (BTree)malloc(sizeof(BTNode));
	if (NULL == t) {
		return;
	}
	t->keynum = 1;	t->key[1] = x;
	t->ptr[0] = p;	t->ptr[1] = ap;
	if (p != NULL) {
		p->parent = t;
	}
	if (ap != NULL) {
		ap->parent = t;
	}
	t->parent = NULL;	//根结点的双亲为空指针
}

/**
 * 将q结点分裂成两个结点，前一半保留在原结点中，后一半移入ap所指新结点
 */
void split(BTree& q, int s, BTree& ap) {
	int i, j;
	ap = (BTree)malloc(sizeof(BTNode));		//生成新结点
	if (NULL == ap) {
		return;
	}
	ap->ptr[0] = q->ptr[s];
	for (i = s+1,j = 1; i <= q->keynum; i++ ,j++) {	//后一半移入ap结点
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
	}
	for (i = 0; i < j; i++) {		//修改新结点的子结点的parent域
		if (ap->ptr[i]!=NULL) {
			ap->ptr[i]->parent = ap;
		}
	}
	ap->keynum = q->keynum - s;
	ap->parent = q->parent;
	q->keynum = s - 1;
}

/**
 * 删除B树上p结点的第i个关键字
 */
void DeleteBTree(BTree &t, BTree& p, int i) {
	//若不是最下层非终端结点
	if (p->ptr[i] != NULL) {
		Successor(p, i);	//在Ai子树中找出最下层非终端结点的最小关键字Ki
		DeleteBTree(t, p, 1);	//转换为删除最下层非终端结点中的最小关键字
	}
	else {		//若是最下层非终端结点
		Remove(p, i);	//从结点p中删除key[i]
		if (p->keynum < (m - 1) / 2) {	//删除后关键字个数小于(m-1)/2
			if (p == t) {		//若该B树为根结点
				if (p->keynum <= 0) {	//若根结点没有关键字，则该树已不满足B树定义，应当销毁
					printf("此树已不符合定义，正在进行销毁\n");
					free(p);
					t = NULL;
				}
			}
			else {
				Restore(t, p, i);	//调整B树
			}
		}
	}
}

/**
 * 在Ai子树中找出最下层非终端结点的最小关键字Ki
 */
void Successor(BTree& p, int i) {
	BTree pl = p->ptr[i];
	//找到Ai子树最下层非终端结点的最小关键字
	while (pl->ptr[0] != NULL) {
		pl = pl->ptr[0];
	}
	//替换结点关键字
	p->key[i] = pl->key[1];
	p = pl;
}

/**
 * 从结点p中删除key[i]
 */
void Remove(BTree& p, int i) {
	while (i < p->keynum) {		//关键字依次左移，添补被删除关键字空缺
		p->key[i] = p->key[i + 1];
		i++;
	}
	p->keynum--;
}

/**
 * 调整B树
 */
void Restore(BTree& t, BTree& p, int i) {
	//j为被删结点在双亲结点中的位置
	int j = 0, k;
	BTree parent = p->parent;
	BTree bro;
	//找到被删结点在双亲结点中的位置
	while (parent->ptr[j] != p) {
		j++;
	}
	if (j < parent->keynum && parent->ptr[j + 1]->keynum > (m - 1) / 2) {		//当右兄弟结点有富余的关键字
		bro = parent->ptr[j + 1];
		//双亲结点中关键字key[j+1]下移至被删关键字结点中
		p->keynum++;
		p->key[p->keynum] = parent->key[j+1];
		//双亲结点下移关键字用被删结点右兄弟的最小关键字代替
		parent->key[j+1] = bro->key[1];
		//删除被删结点右兄弟上移关键字
		Remove(bro, 1);
	}
	else if (j > 0 && parent->ptr[j - 1]->keynum > (m - 1) / 2) {	//当左兄弟结点有富余的关键字
		bro = parent->ptr[j - 1];
		//双亲结点中关键字key[j]下移至被删关键字结点中
		p->keynum++;
		for (k = p->keynum; k > 1; k--) {
			p->key[k] = p->key[k - 1];
		}
		p->key[1] = parent->key[j];
		//双亲结点下移关键字用被删结点左兄弟的最大关键字代替
		parent->key[j] = bro->key[bro->keynum];
		//删除被删结点左兄弟上移关键字
		Remove(bro, bro->keynum);
	}
	else {		//当左右兄弟结点都没有富余的关键字
		if (j > 0) {	//将被删结点剩余关键字以及双亲中分隔p与左兄弟bro的关键字转移至左兄弟
			bro = parent->ptr[j - 1];
			Insert(bro, bro->keynum + 1, parent->key[j], p->ptr[0]);
			for (k = 1; k <= p->keynum; k++) {
				Insert(bro, bro->keynum + 1, p->key[k], p->ptr[k]);
			}
			for (k = j; k < parent->keynum; k++) {
				parent->key[k] = parent->key[k + 1];
				parent->ptr[k] = parent->ptr[k + 1];
			}
			parent->ptr[k] = NULL;
			parent->keynum--;
			free(p);
			p = NULL;
		}
		else {		//将被删结点剩余关键字以及双亲中分隔p与右兄弟bro的关键字转移至右兄弟
			bro = parent->ptr[j + 1];
			Insert(bro, 1, parent->key[j + 1], p->ptr[0]);
			bro->ptr[1] = bro->ptr[0];
			bro->ptr[0] = p->ptr[0];
			for (k = p->keynum; k >=1; k--) {
				Insert(bro, 1, p->key[k], p->ptr[k]);
			}
			parent->ptr[j] = bro;
			for (k = j + 1; k < parent->keynum; k++) {
				parent->key[k] = parent->key[k + 1];
				parent->ptr[k] = parent->ptr[k + 1];
			}
			parent->ptr[k] = NULL;
			parent->keynum--;
			free(p);
			p = NULL;
		}
		if (parent->keynum < (m - 1) / 2) {	//若双亲结点因关键字下移后关键字不足
			if (parent->parent != NULL) {	//如果双亲不是最顶层结点，则继续调整
				Restore(t, parent, i);
			}
			else {			//若双亲是最顶层结点
				if (parent->keynum <= 0) {	//由于根结点最少有2颗子树，即最少1个关键字
					free(parent);
					bro->parent = NULL;		//不为空的子树成为根结点,树的高度减1
					t = bro;				//重新设置根结点
				}
				//根结点有关键字则依旧满足定义
			}
		}
	}
}

/**
 * 销毁B树
 */
void DestroyBTree(BTree& t) {
	int i;
	if (t != NULL) {
		for (i = 0; i <= t->keynum; i++) {	//从最底层开始销毁
			DestroyBTree(t->ptr[i]);
		}
		free(t);
		t = NULL;
	}
}

/**
 * 计算关键字个数
 */
int CountKeyNums(BTree t) {
	int i, result = 0;
	if (t!=NULL) {
		result += t->keynum;
		if (t->ptr[0] == NULL) {	//若已是最底层结点，则直接返回
			return result;
		}							//否则计算叠加下一层
		for (i = 0; i <= t->keynum; i++) {
			result += CountKeyNums(t->ptr[i]);
		}
	}
	return result;
}

/**
 * 凹入表形式打印B树
 */
void PrintBTree(BTree t,int tab) {
	if (NULL == t) {
		return;
	}
	int i;
	printf("\n");
	for (i = 0; i < tab; i++) {		//用缩进表示深度
		printf(" ");
	}
	for (i = 1; i <= t->keynum; i++) {	//先打印出该结点的所有关键字
		printf("|%d", t->key[i]);
	}
	printf("|\n");				//打印完一个结点就换行
	for (i = 0; i <= t->keynum; i++) {	//再依次打印子树
		PrintBTree(t->ptr[i], tab + 5);
	}
}

/**
 * 先序遍历B树
 */
void PreOrderTraverse(BTree t) {
	if (NULL == t) {
		return;
	}
	int i;
	for (i = 1; i <= t->keynum; i++) {
		printf("%d ", t->key[i]);
	}
	if (t->ptr[0] == NULL) { //若为最底层结点，则不用再访问下一层结点，减少递归调用的消耗
		return;
	}
	for (i = 0; i <= t->keynum; i++) {
		PreOrderTraverse(t->ptr[i]);
	}
}

/**
 * 中序遍历B树：按数值大小升序打印关键字
 */
void InOrderTraverse(BTree t) {
	int i;
	if (NULL == t) {
		return;
	}
	InOrderTraverse(t->ptr[0]);
	for (i = 1; i <= t->keynum; i++) {
		printf("%d ", t->key[i]);
		InOrderTraverse(t->ptr[i]);
	}
}

/**
 * 层次遍历B树
 */
void LevelOrderTraverse(BTree t) {
	if (NULL == t) {
		return;
	}
	int i, sum = 0;			//sum用来叠加下一层需要遍历的结点个数
	int newLine = t->keynum + 1;	//newLine用来标志是否需要换行
	BTNode* ptr = NULL;
	LQueue Q;
	InitQueue(Q);		//队列用来按顺序存放结点
	EnQueue(Q, t->ptr[0]);
	for (i = 1; i <= t->keynum; i++) {
		printf(" %d ", t->key[i]);	//遍历该结点上的所有关键字
		EnQueue(Q, t->ptr[i]);		//并依次将孩子结点入队列
	}
	printf("\n");
	while (OK == DeQueue(Q, ptr)) {
		if (newLine == 0) {		//当遍历完同一层结点的所有关键字后才换行
			printf("\n");
			newLine = sum - 1;			//重新赋值
			sum = ptr->keynum + 1;		//重新计算
		}
		else {
			newLine--;			//每访问完同一层的一个结点，newLine减一
			sum += ptr->keynum + 1;		//sum叠加下一层需要遍历的结点个数
		}
		EnQueue(Q, ptr->ptr[0]);
		for (i = 1; i <= ptr->keynum; i++) {
			printf(" %d ", ptr->key[i]);	//遍历该结点上的所有关键字
			EnQueue(Q, ptr->ptr[i]);		//并依次将孩子结点入队列
		}
	}
	DestroyQueue(Q);
}

/**
 * 构造一个空队列Q
 */
void InitQueue(LQueue& Q) {
	Q.front = Q.rear = NULL;	//初始化
}

/**
 * 入队：在队列Q的队尾插入元素key
 */
Status EnQueue(LQueue& Q, BTNode* ptr) {
	if (NULL == ptr) {
		return ERROR;
	}
	LQNode* p;
	p = (LQNode*)malloc(sizeof(LQNode));
	if (NULL == p) {
		return ERROR;
	}
	p->data = ptr;
	p->next = NULL;
	if (NULL == Q.front) {	//若队列为空，则队头队尾同时指向p
		Q.front = p;
	}
	else {
		Q.rear->next = p;	//在队尾插入新结点
	}
	Q.rear = p;
	return OK;
}

/**
 * 出队：若队列Q非空，则删除队头元素，用key返回其值，否则返回ERROR
 */
Status DeQueue(LQueue& Q, BTree& ptr) {
	LQNode* p;
	if (NULL == Q.front) {	//若队列为空
		return ERROR;
	}
	p = Q.front;
	ptr = p->data;
	Q.front = p->next;
	if (Q.rear == p) {	//若p刚好为唯一结点，则队尾指向NULL
		Q.rear = NULL;
	}
	free(p);			//释放内存
	return OK;
}

/**
 * 销毁队列
 */
void DestroyQueue(LQueue& Q) {
	LQNode* p;
	while (Q.front != Q.rear) {
		p = Q.front;
		Q.front = p->next;
		free(p);
	}
	free(Q.rear);
	Q.front = Q.rear = NULL;
}