/*  Complexitati { O(...)}
							Average Case   Best Case   Worst Case
Bubble sort comparatii         n^2             n           n^2
Bubble sort atribuiri          n^2             0           n^2
Bubble sort total              n^2             n           n^2

Insertion sort comparatii       n              n            n
Insertion sort atribuiri       n^2             n           n^2
Insertion sort total           n^2             n           n^2

Selection sort comparatii      n^2            n^2          n^2
Selection sort atribuiri        n              0            n
Selection sort total           n^2            n^2          n^2

Dupa analiza graficelor se poate observa ca in average case bubble sort este de aproximativ
5 ori mai lemt decat insertion sort, iar selection sort este de aproximativ 1.8 ori mai
lent decat insertion sort


*/#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define DEBUG 0
Profiler p("numer");


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


void bubble_sort(int* sir, int n) {
	int ok;
	int n_pass = 0;
	Operation op_comp = p.createOperation("bubble_comp", n);
	Operation op_attr = p.createOperation("bubble_attr", n);
	do {
		ok = 1;
		for (int i = 0; i < n - 1 - n_pass; i++)
		{
			op_comp.count();
			if (sir[i] > sir[i + 1]) {
				swap(&sir[i], &sir[i + 1]);
				ok = 0;
				op_attr.count(3);
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
			op_comp.count();
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
		bubble_sort(sir_aux, i);

		memcpy(sir_aux, sir, sizeof(int) * i);
		selection_sort(sir_aux, i);

		memcpy(sir_aux, sir, sizeof(int) * i);
		insertion_sort(sir_aux, i);

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
		bubble_sort(sir_aux, i);

		memcpy(sir_aux, sir, sizeof(int) * i);
		selection_sort(sir_aux, i);

		memcpy(sir_aux, sir, sizeof(int) * i);
		insertion_sort(sir_aux, i);

		free(sir);
		free(sir_aux);
	}
}

int main() {
	corectitudine_bubble_sort();
	corectitudine_sel_sort();
	corectitudine_ins_sort();

	average_case();

	p.divideValues("bubble_comp", 5);
	p.divideValues("bubble_attr", 5);
	p.addSeries("bubble_total", "bubble_comp", "bubble_attr");//

	p.divideValues("sel_comp", 5);
	p.divideValues("sel_attr", 5);
	p.addSeries("sel_total", "sel_comp", "sel_attr");//

	p.divideValues("ins_comp", 5);
	p.divideValues("ins_attr", 5);
	p.addSeries("ins_total", "ins_comp", "ins_attr");//

	//p.createGroup("comp", "bubble_comp", "sel_comp", "ins_comp");
	//p.createGroup("attr", "bubble_attr", "sel_attr", "ins_attr");
	//p.createGroup("total", "bubble_total", "sel_total", "ins_total");
	//p.createGroup("total bubble", "bubble_comp", "bubble_attr", "bubble_total");

	p.reset("best");

	//p.showReport();

	best_case();
	p.addSeries("bubble_total", "bubble_comp", "bubble_attr");
	p.addSeries("sel_total", "sel_comp", "sel_attr");
	p.addSeries("ins_total", "ins_comp", "ins_attr");
	//p.createGroup("comp", "bubble_comp", "sel_comp", "ins_comp");
	//p.createGroup("attr", "bubble_attr", "sel_attr", "ins_attr");
	//p.createGroup("total", "bubble_total", "sel_total", "ins_total");

	p.reset("worst");

	worst_case();
	p.addSeries("bubble_total", "bubble_comp", "bubble_attr");
	p.addSeries("sel_total", "sel_comp", "sel_attr");
	p.addSeries("ins_total", "ins_comp", "ins_attr");
	p.showReport();

	return 0;

}