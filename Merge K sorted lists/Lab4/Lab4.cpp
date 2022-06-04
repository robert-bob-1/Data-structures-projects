#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define DEBUG 1
Profiler p("profiler");

typedef struct Node {
	int key;
	Node* next;
}Node;

typedef struct Heap {
	int key;
	int lista;
}Heap;

void afisare(int* sir, int n_elem) {
	for (int i = 0; i < n_elem; i++) {
		printf("%d ", sir[i]);
	}
	puts("");
}

void swap(Heap** i, Heap** j) {
	Heap* aux = *i;
	*i = *j;
	*j = aux;
}

Node** createList(int n, int k) {
	Node** List = (Node**)malloc(k * sizeof(Node*));
	int aux = n;
	for (int i = 0; i < k; i++)
	{
		aux -= n / k;
		int lungime;
		int* sir;
		if (i != k - 1) {
			sir = (int*)malloc(n / k * sizeof(int));
			FillRandomArray(sir, n / k, 10, 50000, false, 1);
			lungime = n / k;
		}
		else {
			lungime = (aux + n / k);
			sir = (int*)malloc(lungime * sizeof(int));
			FillRandomArray(sir, lungime, 10, 50000, false, 1);

		}
		//afisare(sir, n / k);
		Node* listhead = (Node*)malloc(sizeof(Node));
		listhead->key = sir[lungime - 1];
		listhead->next = NULL;
		//printf("sir %d %d ",i, listhead->key);

		for (int j = lungime - 2; j >= 0; j--) {
			Node* node = (Node*)calloc(1, sizeof(Node));
			node->key = sir[j];
			node->next = listhead;
			listhead = node;
		//	printf("%d ", node->key);
		}
		//puts("");
		List[i] = listhead;
	}
	return List;
}
void testCreareListe(int n, int k) {
	Node** List = createList(n, k);
	for (int i = 0; i < k; i++) {
		Node* curr = List[i];
		printf("\nsirul %d   ", i);
		while (curr != NULL) {
			printf("%d ", curr->key);
			curr = curr->next;
		}
	}
}
void minHeapify(Heap** heap, int i, int hsize, Operation op_comp, Operation op_attr) {
	int l = i * 2;
	int r = i * 2 + 1;
	int smallest = i;

	op_comp.count();
	if (l < hsize && heap[l]->key < heap[i]->key)
		smallest = l;
	op_comp.count();
	if (r < hsize && heap[r]->key < heap[smallest]->key)
		smallest = r;

	if (smallest != i)
	{
		op_attr.count(3);
		swap(&heap[i], &heap[smallest]);
		minHeapify(heap, smallest, hsize, op_comp, op_attr);
	}
}

Heap* extractMinHeap(Heap** heap, int* hsize, Operation op_comp, Operation op_attr) {
	/*if (*hsize == 0) {
		Heap *empty = (Heap*)malloc(sizeof(Heap));
		empty->key = -1;
		empty->lista = -1;
		return *empty;
	}*/
	op_attr.count();
	Heap* min = heap[0];
	op_attr.count();
	heap[0] = heap[-- * hsize];
	minHeapify(heap, 0, *hsize, op_comp, op_attr);
	return min;
}

void insertMinHeap(Heap** heap, int* hsize, int key, int lista, Operation op_comp, Operation op_attr) {
	(*hsize)++;
	Heap* new1 = (Heap*)malloc(sizeof(Heap));
	new1->key = key;
	new1->lista = lista;
	int i = (*hsize) - 1;
	op_attr.count();
	heap[i] = new1;
	op_comp.count();
	while (i != 0 && heap[i / 2]->key > heap[i]->key)
	{
		op_attr.count(3);
		swap(&heap[i], &heap[i / 2]);
		i = i / 2;
		op_comp.count();
	}
}

/// MERGELISTS MERGELISTS MERGELISTS MERGELISTS MERGELISTS MERGELISTS MERGELISTS MERGELISTS 

int* mergeLists(Node** List, int n, int k, Operation op_comp, Operation op_attr) {

	Heap** heap = (Heap**)malloc(k * sizeof(Heap*));

	int hsize = 0;
	for (int i = 0; i < k; i++)
		if (List[i] != NULL) {
			insertMinHeap(heap, &hsize, List[i]->key, i, op_comp, op_attr);
			/*printf("\nheap");
			for(int j=0;j<i+1;j++)
				printf("%d ", heap[j]->key);*/
		}
	int* sir = (int*)malloc(n * sizeof(int));
	int pozSir = -1;
	for (int i = 0; i < n; i++) {
		Heap* min = extractMinHeap(heap, &hsize, op_comp, op_attr);

		sir[++pozSir] = min->key;
		Node* del = List[min->lista];
		List[min->lista] = List[min->lista]->next;

		if (List[min->lista] != NULL)
			insertMinHeap(heap, &hsize, List[min->lista]->key, min->lista, op_comp, op_attr);
	}
	return sir;
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////
/// </summary>
void mergeListsTest()
{
	int n = 20;
	int k = 4;
	Node** List = createList(20, 4);
	//testCreareListe(20, 4);
	Operation op_comp = p.createOperation("MergeComp2", 30);
	Operation op_attr = p.createOperation("MergeAttr2", 30);

	int* sir = mergeLists(List, 20, 4, op_comp, op_comp);
	puts("mergeLists");
	afisare(sir, n);
}

void averageCase() {
	
	for (int i = 400; i <= 10000; i += 400) {
		printf("%d/%d\r", i, 10000);

		Operation op_comp = p.createOperation("mergeCompk3", i);
		Operation op_attr = p.createOperation("mergeAttrk3", i);
		Node** List = createList(i, 100);
		mergeLists(List, i, 100, op_comp, op_attr);

	}
	for (int i = 400; i <= 10000; i += 400) {
		printf("%d/%d\r", i, 10000);

		Operation op_comp = p.createOperation("mergeCompk1", i);
		Operation op_attr = p.createOperation("mergeAttrk1", i);
		Node** List = createList(i, 5);
		mergeLists(List, i, 5, op_comp, op_attr);

	}
	for (int i = 400; i <= 10000; i += 400) {
		printf("%d/%d\r", i, 10000);

		Operation op_comp = p.createOperation("mergeCompk2", i);
		Operation op_attr = p.createOperation("mergeAttrk2", i);
		Node** List = createList(i, 10);
		mergeLists(List, i, 10, op_comp, op_attr);

	}

	
	for (int j = 0; j < 5; j++)
		for (int i = 10; i < 510; i += 10) {
			printf("%d/%d\r", i, 10000);
			Operation op_comp = p.createOperation("mergeComp2", i);
			Operation op_attr = p.createOperation("mergeAttr2", i);
			Node** List = createList(10000, i);
			mergeLists(List, 10000, i, op_comp, op_attr);
		}


}

int main() {
	int n = 20, k = 4;
	//testCreareListe(n, k);
	mergeListsTest();
	averageCase();

	p.divideValues("mergeComp2", 5);
	p.divideValues("mergeAttr2", 5);

	p.addSeries("mergeTotalk2", "mergeCompk2", "mergeAttrk2");
	p.addSeries("mergeTotalk3", "mergeCompk3", "mergeAttrk3");
	p.addSeries("mergeTotalk1", "mergeCompk1", "mergeAttrk1");
	p.addSeries("mergeTotal2", "mergeComp2", "mergeAttr2");

	p.createGroup("ex1", "mergeTotalk1", "mergeTotalk2", "mergeTotalk3");
	p.createGroup("ex2", "mergeTotal2", "mergeComp2", "mergeAttr2");

	p.showReport();
	
	p.showReport();

	return 0;

}dd