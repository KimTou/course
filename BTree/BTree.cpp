#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTree.h"

/**
 * �������һ�Źؼ��ָ���Ϊnum��B��
 */
BTree InitBTree(int num) {
	BTree t = NULL;
	int i, data;
	result r;
	for (i = 0; i < num; i++) {
		data = rand() % 99;		//�������99���ڵĹؼ���
		SearchBTree(t, data, r);
		if (r.tag == 0) {		//���벻�ظ��ؼ���
			InsertBTree(t, data, r.pt, r.i);
		}
		else {
			i--;
		}
	}
	return t;
}

/**
 * ����B���в��ҵ���Ӧ�ؼ��֣��򷵻عؼ������ڽ��ָ���Լ����ڽ��ؼ���������±꣬����tag������Ƿ���ҵ���
 * ��δ���ҵ����򷵻���Ӧ�ò���Ľ��ָ�뼰��λ�á�
 */
void SearchBTree(BTree t, int k, result& r) {
	int i = 0, found = 0;
	BTree p = t, q = NULL;		//��ʼ����pָ������㣬qָ��������˫��
	while (p != NULL && found == 0) {
		i = Search(p, k);
		if (i <= p->keynum && p->key[i] == k) {
			found = 1;			//�����ҵ��ؼ���
		}
		else {
			q = p;				
			p = p->ptr[i - 1];	//û���ҵ���ָ������
		}
	}
	if (found == 1) {			//���ҵ����򷵻�k���ڵĽ���Լ�λ��i
		r.pt = p;
		r.i = i;
		r.tag = 1;
	}
	else {						//û���ҵ����򷵻�kӦ�ò����q����Լ�����λ��i
		r.pt = q;
		r.i = i;
		r.tag = 0;
	}
}

/**
 * ���عؼ���λ��
 */
int Search(BTree p, int k) {
	int i = 1;
	while (i <= p->keynum && p->key[i] < k) {
		i++;
	}
	return i;
}

/**
 * ���ؼ���k���뵽B��t��q����key[i-1]��key[i]֮��
 */
void InsertBTree(BTree& t, int k, BTree q, int i) {
	int x, s, finished = 0, needNewRoot = 0;
	BTree ap;
	if (NULL == q) {		//��qΪ�գ���tҲΪ��������ʱӦ���������
		newRoot(t, NULL, k, NULL);
	}
	else {
		x = k;
		ap = NULL;		//������ײ㣬����apһ��ʼΪNULL
		while (finished == 0 && needNewRoot == 0) {
			Insert(q, i, x, ap);
			if (q->keynum < m) {
				finished = 1;		//����ɹ�
			}
			else {			//���q�ؼ��������ڵ���m������з���
				s = (m + 1) / 2;
				split(q, s, ap);
				x = q->key[s];
				if (q->parent != NULL) {
					q = q->parent;
					i = Search(q, x);		//��˫�׽���в���x�����λ��
				}
				else {
					needNewRoot = 1;
				}
			}
		}
		if (needNewRoot == 1) {			//�����ѵ�����㣬���������¸����
			newRoot(t, q, x, ap);
		}
	}
}

/**
 * �ؼ���x���½��ָ��ap�ֱ���뵽q->key[i]��q->ptr[i]
 */
void Insert(BTree& q, int i, int x, BTree ap) {
	int j;
	//������λ�ô��ڵ�ǰ�ؼ���������ִ�д�ѭ�����������λ�ü��Ժ�Ĺؼ��ֺ���һλ
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
 * �����µĸ����
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
	t->parent = NULL;	//������˫��Ϊ��ָ��
}

/**
 * ��q�����ѳ�������㣬ǰһ�뱣����ԭ����У���һ������ap��ָ�½��
 */
void split(BTree& q, int s, BTree& ap) {
	int i, j;
	ap = (BTree)malloc(sizeof(BTNode));		//�����½��
	if (NULL == ap) {
		return;
	}
	ap->ptr[0] = q->ptr[s];
	for (i = s+1,j = 1; i <= q->keynum; i++ ,j++) {	//��һ������ap���
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
	}
	for (i = 0; i < j; i++) {		//�޸��½����ӽ���parent��
		if (ap->ptr[i]!=NULL) {
			ap->ptr[i]->parent = ap;
		}
	}
	ap->keynum = q->keynum - s;
	ap->parent = q->parent;
	q->keynum = s - 1;
}

/**
 * ɾ��B����p���ĵ�i���ؼ���
 */
void DeleteBTree(BTree &t, BTree& p, int i) {
	//���������²���ն˽��
	if (p->ptr[i] != NULL) {
		Successor(p, i);	//��Ai�������ҳ����²���ն˽�����С�ؼ���Ki
		DeleteBTree(t, p, 1);	//ת��Ϊɾ�����²���ն˽���е���С�ؼ���
	}
	else {		//�������²���ն˽��
		Remove(p, i);	//�ӽ��p��ɾ��key[i]
		if (p->keynum < (m - 1) / 2) {	//ɾ����ؼ��ָ���С��(m-1)/2
			if (p == t) {		//����B��Ϊ�����
				if (p->keynum <= 0) {	//�������û�йؼ��֣�������Ѳ�����B�����壬Ӧ������
					printf("�����Ѳ����϶��壬���ڽ�������\n");
					free(p);
					t = NULL;
				}
			}
			else {
				Restore(t, p, i);	//����B��
			}
		}
	}
}

/**
 * ��Ai�������ҳ����²���ն˽�����С�ؼ���Ki
 */
void Successor(BTree& p, int i) {
	BTree pl = p->ptr[i];
	//�ҵ�Ai�������²���ն˽�����С�ؼ���
	while (pl->ptr[0] != NULL) {
		pl = pl->ptr[0];
	}
	//�滻���ؼ���
	p->key[i] = pl->key[1];
	p = pl;
}

/**
 * �ӽ��p��ɾ��key[i]
 */
void Remove(BTree& p, int i) {
	while (i < p->keynum) {		//�ؼ����������ƣ�����ɾ���ؼ��ֿ�ȱ
		p->key[i] = p->key[i + 1];
		i++;
	}
	p->keynum--;
}

/**
 * ����B��
 */
void Restore(BTree& t, BTree& p, int i) {
	//jΪ��ɾ�����˫�׽���е�λ��
	int j = 0, k;
	BTree parent = p->parent;
	BTree bro;
	//�ҵ���ɾ�����˫�׽���е�λ��
	while (parent->ptr[j] != p) {
		j++;
	}
	if (j < parent->keynum && parent->ptr[j + 1]->keynum > (m - 1) / 2) {		//�����ֵܽ���и���Ĺؼ���
		bro = parent->ptr[j + 1];
		//˫�׽���йؼ���key[j+1]��������ɾ�ؼ��ֽ����
		p->keynum++;
		p->key[p->keynum] = parent->key[j+1];
		//˫�׽�����ƹؼ����ñ�ɾ������ֵܵ���С�ؼ��ִ���
		parent->key[j+1] = bro->key[1];
		//ɾ����ɾ������ֵ����ƹؼ���
		Remove(bro, 1);
	}
	else if (j > 0 && parent->ptr[j - 1]->keynum > (m - 1) / 2) {	//�����ֵܽ���и���Ĺؼ���
		bro = parent->ptr[j - 1];
		//˫�׽���йؼ���key[j]��������ɾ�ؼ��ֽ����
		p->keynum++;
		for (k = p->keynum; k > 1; k--) {
			p->key[k] = p->key[k - 1];
		}
		p->key[1] = parent->key[j];
		//˫�׽�����ƹؼ����ñ�ɾ������ֵܵ����ؼ��ִ���
		parent->key[j] = bro->key[bro->keynum];
		//ɾ����ɾ������ֵ����ƹؼ���
		Remove(bro, bro->keynum);
	}
	else {		//�������ֵܽ�㶼û�и���Ĺؼ���
		if (j > 0) {	//����ɾ���ʣ��ؼ����Լ�˫���зָ�p�����ֵ�bro�Ĺؼ���ת�������ֵ�
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
		else {		//����ɾ���ʣ��ؼ����Լ�˫���зָ�p�����ֵ�bro�Ĺؼ���ת�������ֵ�
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
		if (parent->keynum < (m - 1) / 2) {	//��˫�׽����ؼ������ƺ�ؼ��ֲ���
			if (parent->parent != NULL) {	//���˫�ײ�������㣬���������
				Restore(t, parent, i);
			}
			else {			//��˫���������
				if (parent->keynum <= 0) {	//���ڸ����������2��������������1���ؼ���
					free(parent);
					bro->parent = NULL;		//��Ϊ�յ�������Ϊ�����,���ĸ߶ȼ�1
					t = bro;				//�������ø����
				}
				//������йؼ������������㶨��
			}
		}
	}
}

/**
 * ����B��
 */
void DestroyBTree(BTree& t) {
	int i;
	if (t != NULL) {
		for (i = 0; i <= t->keynum; i++) {	//����ײ㿪ʼ����
			DestroyBTree(t->ptr[i]);
		}
		free(t);
		t = NULL;
	}
}

/**
 * ����ؼ��ָ���
 */
int CountKeyNums(BTree t) {
	int i, result = 0;
	if (t!=NULL) {
		result += t->keynum;
		if (t->ptr[0] == NULL) {	//��������ײ��㣬��ֱ�ӷ���
			return result;
		}							//������������һ��
		for (i = 0; i <= t->keynum; i++) {
			result += CountKeyNums(t->ptr[i]);
		}
	}
	return result;
}

/**
 * �������ʽ��ӡB��
 */
void PrintBTree(BTree t,int tab) {
	if (NULL == t) {
		return;
	}
	int i;
	printf("\n");
	for (i = 0; i < tab; i++) {		//��������ʾ���
		printf(" ");
	}
	for (i = 1; i <= t->keynum; i++) {	//�ȴ�ӡ���ý������йؼ���
		printf("|%d", t->key[i]);
	}
	printf("|\n");				//��ӡ��һ�����ͻ���
	for (i = 0; i <= t->keynum; i++) {	//�����δ�ӡ����
		PrintBTree(t->ptr[i], tab + 5);
	}
}

/**
 * �������B��
 */
void PreOrderTraverse(BTree t) {
	if (NULL == t) {
		return;
	}
	int i;
	for (i = 1; i <= t->keynum; i++) {
		printf("%d ", t->key[i]);
	}
	if (t->ptr[0] == NULL) { //��Ϊ��ײ��㣬�����ٷ�����һ���㣬���ٵݹ���õ�����
		return;
	}
	for (i = 0; i <= t->keynum; i++) {
		PreOrderTraverse(t->ptr[i]);
	}
}

/**
 * �������B��������ֵ��С�����ӡ�ؼ���
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
 * ��α���B��
 */
void LevelOrderTraverse(BTree t) {
	if (NULL == t) {
		return;
	}
	int i, sum = 0;			//sum����������һ����Ҫ�����Ľ�����
	int newLine = t->keynum + 1;	//newLine������־�Ƿ���Ҫ����
	BTNode* ptr = NULL;
	LQueue Q;
	InitQueue(Q);		//����������˳���Ž��
	EnQueue(Q, t->ptr[0]);
	for (i = 1; i <= t->keynum; i++) {
		printf(" %d ", t->key[i]);	//�����ý���ϵ����йؼ���
		EnQueue(Q, t->ptr[i]);		//�����ν����ӽ�������
	}
	printf("\n");
	while (OK == DeQueue(Q, ptr)) {
		if (newLine == 0) {		//��������ͬһ��������йؼ��ֺ�Ż���
			printf("\n");
			newLine = sum - 1;			//���¸�ֵ
			sum = ptr->keynum + 1;		//���¼���
		}
		else {
			newLine--;			//ÿ������ͬһ���һ����㣬newLine��һ
			sum += ptr->keynum + 1;		//sum������һ����Ҫ�����Ľ�����
		}
		EnQueue(Q, ptr->ptr[0]);
		for (i = 1; i <= ptr->keynum; i++) {
			printf(" %d ", ptr->key[i]);	//�����ý���ϵ����йؼ���
			EnQueue(Q, ptr->ptr[i]);		//�����ν����ӽ�������
		}
	}
	DestroyQueue(Q);
}

/**
 * ����һ���ն���Q
 */
void InitQueue(LQueue& Q) {
	Q.front = Q.rear = NULL;	//��ʼ��
}

/**
 * ��ӣ��ڶ���Q�Ķ�β����Ԫ��key
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
	if (NULL == Q.front) {	//������Ϊ�գ����ͷ��βͬʱָ��p
		Q.front = p;
	}
	else {
		Q.rear->next = p;	//�ڶ�β�����½��
	}
	Q.rear = p;
	return OK;
}

/**
 * ���ӣ�������Q�ǿգ���ɾ����ͷԪ�أ���key������ֵ�����򷵻�ERROR
 */
Status DeQueue(LQueue& Q, BTree& ptr) {
	LQNode* p;
	if (NULL == Q.front) {	//������Ϊ��
		return ERROR;
	}
	p = Q.front;
	ptr = p->data;
	Q.front = p->next;
	if (Q.rear == p) {	//��p�պ�ΪΨһ��㣬���βָ��NULL
		Q.rear = NULL;
	}
	free(p);			//�ͷ��ڴ�
	return OK;
}

/**
 * ���ٶ���
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