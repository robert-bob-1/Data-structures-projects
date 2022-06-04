/*
    In cazul mediu statistic, quick sort este cu aproximativ 50% mai eficient decat heapsort, 
	amandoua avand complexitate O(n log n).

*/
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define DEBUG 1
Profiler p("p");







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

int partition(int* a, int p, int r, Operation op_comp, Operation op_attr) {
	op_attr.count();
	int x = a[r];
	int i = p - 1;
	for (int j = p; j < r; j++) {
		op_comp.count();
		if (a[j] <= x) {
			i++;
			op_attr.count(3);
			swap(&a[i], &a[j]);
		}
	}
	op_attr.count(3);
	swap(&a[i + 1], &a[r]);
	return i + 1;
}

void quickSort(int* a, int p, int r, Operation op_comp, Operation op_attr) {
	if (p < r) {
		int q = partition(a, p, r, op_comp, op_attr);
		quickSort(a, p, q - 1, op_comp, op_attr);
		quickSort(a, q + 1, r, op_comp, op_attr);
	}
}

void quickSortTest() {
	int sir[] = { 10,2,7,9,11,29,1 };
	int n = sizeof(sir) / sizeof(sir[0]);
	afisare(sir, n);
	int i = 1;
	Operation op_comp = p.createOperation("quickSortComp", i);
	Operation op_attr = p.createOperation("quickSortComp", i);
	quickSort(sir, 0, n - 1, op_comp, op_attr);
	puts("quickSort");
	afisare(sir, n);
}



int randomizedPartition(int* a, int p, int r, Operation comp, Operation attr) {
	int i = rand()%(r-p+1)+p;
	attr.count(3);
	swap(&a[r], &a[i]);
	return partition(a, p, r, comp, attr);
}
void randomizedQuickSort(int* a, int p, int r, Operation comp, Operation attr) {
	if (p < r) {
		int q = randomizedPartition(a, p, r, comp, attr);
		randomizedQuickSort(a, p, q - 1,comp,attr);
		randomizedQuickSort(a, q+1,r,comp,attr);
	}
}
void randomizedQuickSortTest() {
	int sir[] = { 10,2,7,9,11,29,-1 };
	int n = sizeof(sir) / sizeof(sir[0]);
	afisare(sir, n);
	int i = 1;
	Operation op_comp = p.createOperation("quickSortComp", i);
	randomizedQuickSort(sir, 0, n - 1, op_comp, op_comp);
	puts("randomizedQuickSort");
	afisare(sir, n);
}



void averageCase() {
	for (int pass = 0; pass < 5; pass++) {
		for (int i = 500; i <= 10000; i += 500) {
			printf("%d/%d\r", i, 10000);
			int* sir = (int*)malloc(sizeof(int) * 10000);
			int* sir_aux = (int*)malloc(sizeof(int) * 10000);

			FillRandomArray(sir, i);

			Operation op_comp = p.createOperation("quickSortComp", i);
			Operation op_attr = p.createOperation("quickSortAttr", i);
			memcpy(sir_aux, sir, sizeof(int) * i);
			quickSort(sir_aux, 0, i - 1, op_comp, op_attr);

			memcpy(sir_aux, sir, sizeof(int) * i);
			HeapSort(sir_aux, i);
			Operation op_comp1 = p.createOperation("randomizedQuickSortComp", i);
			Operation op_attr1 = p.createOperation("randomizedQuickSortAttr", i);
			memcpy(sir_aux, sir, sizeof(int) * i);
			randomizedQuickSort(sir_aux, 0, i - 1, op_comp1, op_attr1);

			free(sir);
			free(sir_aux);
		}
	}
}

void worstCase() {
	for (int pass = 0; pass < 5; pass++) {
		for (int i = 500; i <= 10000; i += 500) {
			printf("%d/%d\r", i, 10000);
			int* sir = (int*)malloc(sizeof(int) * 10000);
			int* sir_aux = (int*)malloc(sizeof(int) * 10000);

			for (int j = 0; j < i; j++)
				sir[j] = 1;

			Operation op_comp1 = p.createOperation("quickSortCompWorst", i);
			Operation op_attr1 = p.createOperation("quickSortAttrWorst", i);
			quickSort(sir, 0, i - 1, op_comp1, op_attr1);

			/*FillRandomArray(sir, i, 10, 50000, false, 2);

			Operation op_comp2 = p.createOperation("quickSortCompBest", i);
			Operation op_attr2 = p.createOperation("quickSortAttrBest", i);
			quickSort(sir, 0, i - 1, op_comp2, op_attr2);
			*/

			free(sir);
			free(sir_aux);
		}
	}
}

int main() {
	randomizedQuickSortTest();
	quickSortTest();
	averageCase();
	worstCase();

	
	
	p.divideValues("randomizedQuickSortComp", 5);
	p.divideValues("randomizedQuickSortAttr", 5);
	p.addSeries("randomizedQuickSortTotal", "randomizedQuickSortComp", "randomizedQuickSortAttr");

	p.divideValues("quickSortCompWorst", 5);
	p.divideValues("quickSortAttrWorst", 5);
	p.addSeries("quickSortTotalWorst", "quickSortCompWorst", "quickSortAttrWorst");

	p.divideValues("quickSortComp", 5);
	p.divideValues("quickSortAttr", 5);
	p.addSeries("quickSortTotal", "quickSortComp", "quickSortAttr");

	p.divideValues("heapSortComp", 5);
	p.divideValues("heapSortAttr", 5);
	p.addSeries("heapSortTotal", "heapSortComp", "heapSortAttr");

	p.createGroup("quickSort", "quickSortComp", "quickSortAttr", "quickSortTotal");
	p.createGroup("randomizedQuickSort", "randomizedQuickSortComp", "randomizedQuickSortAttr", "randomizedQuickSortTotal");
	p.createGroup("heapSort", "heapSortComp", "heapSortAttr", "heapSortTotal");
	p.createGroup("quickSortWorst", "quickSortCompWorst", "quickSortAttrWorst", "quickSortTotalWorst");
	p.createGroup("quickSortBest", "quickSortCompBest", "quickSortAttrBest", "quickSortTotalBest");
	p.createGroup("Comparatie QS HS", "heapSortTotal", "quickSortTotal","randomizedQuickSortTotal");

	//p.showReport();
	
	return 0;

}