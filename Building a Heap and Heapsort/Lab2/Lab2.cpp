/*
Implementarea BuildMaxHeap-ului in maniera Bottom up este cu aproximativ 40% mai eficienta pe
siruri de lungime 100-10 000. Totusi, ambii algoritmi au la prima vedere complexitate O(n logn)
, cu implementarea bottom up fiind mai eficienta. Cu toate acestea, exista demonstratii care
arata ca abordarea Bottom Up are de fapt complexitate O(n) deoarece costul Max Heapify-ului
nu este O(log n) pentru toate elementele (este O(logn) doar pentru ultimul element)

*/
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define DEBUG 1
Profiler p("num");



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

void MaxHeapify(int* a, int i, int length, Operation op_comp, Operation op_attr) {
	int largest = i;
	int l = i * 2 + 1;
	int r = 2 * i + 2;
	op_comp.count();
	if (l < length && a[l] > a[i])
		largest = l;

	op_comp.count();
	if (r < length && a[r] > a[largest])
		largest = r;
	if (largest != i)
	{
		op_attr.count(3);
		swap(&a[i], &a[largest]);
		MaxHeapify(a, largest, length, op_comp, op_attr);
	}
}


void BuildMaxHeap(int* a, int length, Operation op_comp, Operation op_attr) {

	for (int i = length / 2; i >= 0; i--) {
		MaxHeapify(a, i, length, op_comp, op_attr);
	}
}

void BuildMaxHeapTest() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n = sizeof(sir) / sizeof(sir[0]);
	afisare(sir, n);
	Operation op_comp = p.createOperation("h", n);
	Operation op_attr = p.createOperation("h1", n);
	BuildMaxHeap(sir, n, op_comp, op_attr);
	puts("build max heap");
	afisare(sir, n);
}

void HeapSort(int* a, int n) {
	Operation op_comp = p.createOperation("heapSortComp", n);
	Operation op_attr = p.createOperation("heapSortAttr", n);
	BuildMaxHeap(a, n, op_comp, op_attr);

	for (int i = n - 1; i >= 0; i--) {
		op_attr.count(3);
		swap(&a[0], &a[i]);
		MaxHeapify(a, 0, i, op_comp, op_attr);
	}
}

void HeapSortTest() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n = sizeof(sir) / sizeof(sir[0]);
	afisare(sir, n);
	HeapSort(sir, n);
	puts("heapsort");
	afisare(sir, n);
}
/////////////////////////////////////////////////////////////////////////////////////////

void HeapIncreaseKey(int* a, int i, int key, Operation op_comp, Operation op_attr) {
	op_comp.count();
	if (key < a[i]) {
		return;
	}
	op_attr.count();
	a[i] = key;
	op_comp.count();
	while (i > 0 && a[i / 2] < a[i]) {
		op_attr.count(3);
		swap(&a[i], &a[i / 2]);
		i = i / 2;
	}
}

void MaxHeapInsert(int* a, int key, int* n, Operation op_comp, Operation op_attr) {
	(*n)++;
	op_attr.count();
	a[*n] = INT_MIN;
	HeapIncreaseKey(a, *n, key, op_comp, op_attr);
}

void BuildMaxHeap2(int* a, int n, Operation op_comp, Operation op_attr) {
	int heapSize = -1;

	for (int i = 0; i < n; i++) {
		MaxHeapInsert(a, a[i], &heapSize, op_comp, op_attr);
	}
}

void BuildMaxHeap2Test() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n = sizeof(sir) / sizeof(sir[0]);
	afisare(sir, n);
	Operation op_comp = p.createOperation("h2", n);
	Operation op_attr = p.createOperation("h3", n);
	BuildMaxHeap2(sir, n, op_comp, op_attr);
	puts("max heap 2");
	afisare(sir, n);
}

void averageCase() {
	for (int pass = 0; pass < 5; pass++) {
		for (int i = 500; i <= 10000; i += 500) {
			printf("%d/%d\r", i, 10000);
			int* sir = (int*)malloc(sizeof(int) * 10000);
			int* sir_aux = (int*)malloc(sizeof(int) * 10000);

			FillRandomArray(sir, i);

			Operation op_comp = p.createOperation("heapbotComp", i);
			Operation op_attr = p.createOperation("heapbotAttr", i);
			memcpy(sir_aux, sir, sizeof(int) * i);
			BuildMaxHeap(sir_aux, i, op_comp, op_attr);
			Operation op_comp1 = p.createOperation("heaptopComp", i);
			Operation op_attr1 = p.createOperation("heaptopAttr", i);
			memcpy(sir_aux, sir, sizeof(int) * i);
			BuildMaxHeap2(sir_aux, i, op_comp1, op_attr1);
			memcpy(sir_aux, sir, sizeof(int) * i);
			HeapSort(sir_aux, i);

			free(sir);
			free(sir_aux);
		}
	}
}
/*
void bubble_sort(int* sir, int n) {
	int ok;
	int n_pass = 0;
	//Operation op_comp = p.createOperation("bubble_comp", n);
	//Operation op_attr = p.createOperation("bubble_attr", n);
	do {
		ok = 1;
		for (int i = 0; i < n - 1 - n_pass; i++)
		{
			//op_comp.count();
			if (sir[i] > sir[i + 1]) {
				swap(&sir[i], &sir[i + 1]);
				ok = 0;
				//op_attr.count(3);
			}
		}
		n_pass++;
		if (DEBUG == 1) afisare(sir, n);
	} while (ok == 0);
}
void corectitudine_bubble_sort() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n_elem = sizeof(sir) / sizeof(int);
	afisare(sir, n_elem);
	bubble_sort(sir, n_elem);
	puts("BUBBLE SORT");
	afisare(sir, n_elem);
}

/// <summary>
/// /////////////////////////////////////////////////////////////////////////
/// </summary>
/// <param name="a"></param>
/// <param name="n"></param>

void selection_sort(int* a, int n) {
	Operation op_comp = p.createOperation("sel_comp", n);
	Operation op_attr = p.createOperation("sel_attr", n);

	int min, i, j;
	for (i = 0; i < n - 1; i++) {
		min = i;
		for (j = i + 1; j < n; j++) {
			op_comp.count();
			if (a[j] < a[min])
				min = j;
		}
		if (i != min) {
			op_attr.count(3);
			swap(&a[i], &a[min]);
		}

		if (DEBUG == 1) afisare(a, n);
	}
}

void corectitudine_sel_sort() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n_elem = sizeof(sir) / sizeof(int);
	puts("\n\nSELECTION SORT");
	afisare(sir, n_elem);
	puts("");
	selection_sort(sir, n_elem);
	puts("rezultat");
	afisare(sir, n_elem);
}

////////////////////////////////////////////////////////////////////////////////////////////////


void insertion_sort(int* a, int n) {
	Operation op_comp = p.createOperation("ins_comp", n);
	Operation op_attr = p.createOperation("ins_attr", n);
	int k, j;
	for (int i = 0; i < n; i++) {
		op_attr.count();
		k = a[i];
		j = i - 1;
		op_comp.count();
		while (j >= 0 && a[j] > k) {
			op_attr.count();
			a[j + 1] = a[j];
			j = j - 1;
		}

		a[j + 1] = k;
	}
	if (DEBUG == 1) afisare(a, n);
}


void corectitudine_ins_sort() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n_elem = sizeof(sir) / sizeof(int);
	puts("\n\nINSERTION SORT");
	afisare(sir, n_elem);
	puts("");
	insertion_sort(sir, n_elem);
	puts("rezultat");
	afisare(sir, n_elem);
}

////////////////////////////////////////////////////////////////////////////////////////////////


void average_case() {

	for (int pass = 0; pass < 5; pass++) {
		for (int i = 500; i <= 10000; i += 500) {
			printf("%d/%d\r", i, 10000);
			int* sir = (int*)malloc(sizeof(int) * 10000);
			int* sir_aux = (int*)malloc(sizeof(int) * 10000);

			FillRandomArray(sir, i);

			memcpy(sir_aux, sir, sizeof(int) * i);
			bubble_sort(sir_aux, i);
			memcpy(sir_aux, sir, sizeof(int) * i);
			selection_sort(sir_aux, i);
			memcpy(sir_aux, sir, sizeof(int) * i);
			insertion_sort(sir_aux, i);

			free(sir);
			free(sir_aux);
		}
	}
}


void best_case() {

	for (int i = 500; i <= 10000; i += 500) {
		printf("%d/%d\r", i, 10000);
		int* sir = (int*)malloc(sizeof(int) * 10000);
		int* sir_aux = (int*)malloc(sizeof(int) * 10000);

		FillRandomArray(sir, i, 10, 50000, false, 1);
		memcpy(sir_aux, sir, sizeof(int) * i);

		memcpy(sir_aux, sir, sizeof(int) * i);

		memcpy(sir_aux, sir, sizeof(int) * i);

		free(sir);
		free(sir_aux);
	}
}

void worst_case() {
	for (int i = 500; i <= 10000; i += 500) {
		printf("%d/%d\r", i, 10000);
		int* sir = (int*)malloc(sizeof(int) * 10000);
		int* sir_aux = (int*)malloc(sizeof(int) * 10000);

		FillRandomArray(sir, i, 10, 50000, false, 2);
		memcpy(sir_aux, sir, sizeof(int) * i);


		memcpy(sir_aux, sir, sizeof(int) * i);


		memcpy(sir_aux, sir, sizeof(int) * i);


		free(sir);
		free(sir_aux);
	}
}*/

int main() {
	BuildMaxHeapTest();
	HeapSortTest();
	BuildMaxHeap2Test();
	averageCase();

	p.divideValues("heapbotComp", 5);
	p.divideValues("heapbotAttr", 5);
	p.addSeries("heapbotTotal", "heapbotComp", "heapbotAttr");

	p.divideValues("heaptopComp", 5);
	p.divideValues("heaptopAttr", 5);
	p.addSeries("heaptopTotal", "heaptopComp", "heaptopAttr");

	p.divideValues("heapSortComp", 5);
	p.divideValues("heapSortAttr", 5);
	p.addSeries("heapSortTotal", "heapSortComp", "heapSortAttr");

	p.createGroup("compBuild", "heapbotComp", "heaptopComp");
	p.createGroup("attrBuild", "heapbotAttr", "heaptopAttr");
	p.createGroup("totalBuild", "heapbotTotal", "heaptopTotal");
	p.createGroup("Sort", "heapSortComp", "heapSortAttr", "heapSortTotal");


	p.showReport();

	return 0;

}