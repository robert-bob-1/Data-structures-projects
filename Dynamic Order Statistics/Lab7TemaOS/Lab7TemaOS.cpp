#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define DEBUG 1
Profiler p("profiler");


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

typedef struct Node {
	Node* left, * right;
	int key;
	int size;
};
void spaces(int depth, Node* node) {
	for (int d = 0; d < depth; d++) {
		printf("   ");
	}
	printf("(%d %d)\n", node->key, node->size);

}

Node* OSBuild(int a, int b) {
	if (a > b)
		return NULL;
	int c = (a + b) / 2;
	Node* root = (Node*)malloc(sizeof(Node));
	root->key = c;
	root->left = OSBuild(a, c - 1);
	root->right = OSBuild(c + 1, b);
	if (root->left == NULL && root->right == NULL) {
		root->size = 1;
		return root;
	}
	if (root->left == NULL) {
		root->size = 1 + root->right->size;
		return root;
	}
	if (root->right == NULL) {
		root->size = 1 + root->left->size;
		return root;
	}
	root->size = 1 + root->left->size + root->right->size;
	return root;
}

Node* buildTree(int n) {
	if (n == 0)
		return NULL;
	return OSBuild(1, n);
}

void prettyPrint(Node* root, int depth) {
	if (root == NULL)
		return;
	spaces(depth, root);
	prettyPrint(root->left, depth + 1);
	prettyPrint(root->right, depth + 1);
}

/// <summary>
/// //////////////////////////////////////////////////////////////////////
/// </summary>

Node* OSSelect(Node* x, int i) {
	int r = 1;
	if (x->left != NULL)
		r = x->left->size + r;
	if (i == r)
		return x;
	if (i < r)
		return OSSelect(x->left, i);
	if (x->right != NULL)
		return OSSelect(x->right, i - r);
}


Node* OSSelect2(Node* x, int i, Node** last) {
	int r = 1;
	if (x->left != NULL)
		r = x->left->size + r;
	if (i == r)
		return x;
	if (i < r) {
		//x->size--;
		*last = x;
		return OSSelect2(x->left, i, last);
	}
	if (x->right != NULL) {
		*last = x;
		//x->size--;
		return OSSelect2(x->right, i - r, last);
	}
}

Node* findPredecessor(Node* replace) {//,Node **last) {
	while (replace->right != NULL) {
		//(*last) = replace;
		replace = replace->right;
	}
	return replace;
}

Node* deleteNode(Node* root, int key) {
	if (root == NULL)
		return root;
	//daca rangu e mai mic
	if (key < root->key) {
		root->size--;
		root->left = deleteNode(root->left, key);
	}
	//daca rangu e mai mare
	else if (key > root->key) {
		root->size--;
		root->right = deleteNode(root->right, key);
	}
	//daca am gasit rangu
	else {
		root->size--;
		int size = root->size;
		if (root->left == NULL) {
			Node* aux = root->right;
			if (root->right != NULL)
				aux->size = size;
			free(root);
			return aux;
		}
		else if (root->right == NULL) {
			Node* aux = root->left;
			free(root);
			aux->size = size;
			return aux;
		}
		Node* aux = findPredecessor(root->left);
		root->key = aux->key;
		root->left = deleteNode(root->left, aux->key);
	}
	return root;
}


void OSDelete(Node** x, int i) {
	Node* last = NULL;
	Node* toDelete = OSSelect2(*x, i, &last);

	if (last != NULL) {
		//last = deleteNode(last, toDelete->key);
		*x = deleteNode(*x, toDelete->key);
	}
	else
	{
		*x = deleteNode(*x, (*x)->key);
	}
}

void test() {
	int n = 11;
	Node* root = buildTree(n);
	prettyPrint(root, 0);
	printf("select 3 %d   select 5 %d   select 10 %d\n", OSSelect(root, 3)->key, OSSelect(root, 5)->key, OSSelect(root, 10)->key);

	//prettyPrintSize(root, 0);
	puts("");
	OSDelete(&root, 5);
	printf("delete %d---------------\n", 5);
	prettyPrint(root, 0);
	OSDelete(&root, 5);
	printf("delete %d---------------\n", 5);
	prettyPrint(root, 0);

	/*for (int i = n; i >= 1; i--) {
		OSDelete(&root, i);
		printf("delete %d---------------\n", i);
		prettyPrint(root, 0);
	}*/
}

void averageCase() {
	for (int pass = 0; pass < 5; pass++) {
		for (int n = 100; n <= 10000; n += 100) {

			Node* root = buildTree(n);

			int naux = n;
			for (int i = rand() % naux; naux >= 1; naux--, i = rand() % naux) {
				OSSelect(root, i);
				OSDelete(&root, i);
				prettyPrint(root, 0);
			}
		}
	}
}




int main() {
	test();
	averageCase();



	/*p.divideValues("randomizedQuickSortComp", 5);
	p.divideValues("randomizedQuickSortAttr", 5);
	p.addSeries("randomizedQuickSortTotal", "randomizedQuickSortComp", "randomizedQuickSortAttr");

	p.createGroup("quickSort", "quickSortComp", "quickSortAttr", "quickSortTotal");


	p.showReport();*/


	return 0;
}