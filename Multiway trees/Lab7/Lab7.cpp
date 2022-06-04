
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define DEBUG 1
Profiler p("profiler");

typedef struct Node1 {
	Node1** child;
	int key;
	int count = 0;
};

typedef struct Node2 {
	Node2* leftChild,*rightBrother;
	int key;
};

typedef struct stack {
	int key;
	stack* next;
}stack;

void afisare(int* sir, int n_elem) {
	for (int i = 0; i < n_elem; i++) {
		printf("%d ", sir[i]);
	}
	puts("");
}

void swap(int* i, int* j) {
	int aux = *i;
	*i = *j;
	*j = aux;
}

void spaces(int depth,int i) {
	for (int d = 0; d < depth; d++) {
		printf("   ");
	}
	printf("%d\n", i);

}

void ppR1(int* p, int n, int last, int depth) {
	//int n = (int)(sizeof(p) / sizeof(p[0]));

	for (int j = 0; j < n; j++) {
		if (p[j] == last) {
			spaces(depth,j+1);
			ppR1(p, n,j, depth+1);
		}
	}
}

Node1* t1(int* p,int size) {
	int nodesLen = size;
	Node1** nodes = (Node1**)calloc(nodesLen, sizeof(Node1*));
	int* count = (int*)calloc(nodesLen, sizeof(int));
	Node1* root=NULL;
	for (int i = 0; i < nodesLen; i++) {
		nodes[i] = (Node1*)calloc(1,sizeof(Node1)); 
		nodes[i]->key = i;
	} 
	for (int i = 0; i < nodesLen; i++) {
		
		if (p[i] != -1)
			nodes[p[i]]->count++;
		else
			root= nodes[i];
	}

	for (int i = 0; i < nodesLen; i++)
		if (nodes[i]->count != 0)
			nodes[i]->child = (Node1**)calloc(nodes[i]->count,sizeof(Node1*));


	for (int i = 0; i < nodesLen; i++) {
		if (p[i] != -1) {
			nodes[p[i]]->child[count[p[i]]] = nodes[i];
			count[p[i]]++;
		}
	}
	return root;
}

void ppR2(Node1* root,int depth) {
	if (depth == 0) {
		spaces(depth, root->key+1);
		depth = depth + 1;
	}
	for (int i = 0; i < root->count; i++) {
		spaces(depth,root->child[i]->key+1);
		ppR2(root->child[i], depth + 1);
	}
}

Node2* t2(Node1* root) {
	Node2* p;
	Node2* rootB = NULL;

	if (root != NULL) {
		rootB = (Node2*)malloc(sizeof(Node2));
		rootB->key = root->key+1;
		rootB->rightBrother = NULL;
		rootB->leftChild = NULL;
		if (root->count > 0) {
			rootB->leftChild = t2(root->child[0]);
			p=rootB->leftChild;

			for (int i = 1; i < root->count; i++) {
				p->rightBrother = t2(root->child[i]);
				p = p->rightBrother;
			}
		}
	}
	return rootB;
}

void ppR3(Node2* root, int depth) {
	if (root != NULL) {
		spaces(depth, root->key);
		ppR3(root->leftChild, depth + 1);
		ppR3(root->rightBrother, depth);
	}
}

void test() {
	int p[] = { 2,7,5,2,7,7,-1,5,2 };
	int n = (int)(sizeof(p) / sizeof(p[0]));
	for (int i = 0; i < n; i++) {
		if (p[i] != -1)
			p[i]--;
	}
	ppR1(p, n, -1, 0);
	Node1* root= t1(p, n);
	ppR2(root, 0);
	Node2* rootB = t2(root);
	ppR3(rootB, 0);
}

int main() {
	test();
	return 0;

}