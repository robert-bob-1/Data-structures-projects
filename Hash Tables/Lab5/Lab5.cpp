
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"

#define DEBUG 1
Profiler p("profiler");

typedef struct Node {
	int key;
	Node* next;
}Node;



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

int h(int key, int i, int m) {
	return (key % m + i*2  + 3 * i * i) % m;
}
void insert(int* sir, int k, int m) {
	int i = 0;
	do {
		int j = h(k, i, m);
		if (sir[j] == 0) {
			sir[j] = k;
			return;
		}
		i++;
	} while (true);
	puts("overflow");
}

int search(int* sir, int k, int m) {
	int i = 0;
	int j;
	do {
		j = h(k, i, m);
		if (sir[j] == k)
			return j;
		i++;
	} while (sir[j] != 0 && i < m);
	return -1;
}
int searchNo(int* sir, int k, int m) {
	int i = 0;
	int j;
	do {
		j = h(k, i, m);
		if (sir[j] == k)
			return i + 1;
		i++;
	} while (sir[j] != 0 && i < m);
	return i + 1;
}

void hashTest()
{
	int* sir = (int*)malloc(19 * sizeof(int));
	FillRandomArray(sir, 19);
	int* hashTable = (int*)calloc(20, sizeof(int));
	for (int i = 0; i < 20; i++)
		insert(hashTable, sir[i], 20);
	puts("sir generat");
	afisare(sir, 19);
	puts("\nhashTable");
	afisare(hashTable, 20);
	for (int i = 0; i < 19; i++)
		printf("\ncautam %d gasit la pozitia %d", sir[i], search(hashTable, sir[i], 20));
	printf("\ncautam %d gasit la pozitia %d", 155274, search(hashTable, 155274, 20));

	printf("\ncautam %d gasit la pozitia %d", 1, search(hashTable, 1, 20));
	puts("");
}


void Evaluare(float factor, int* aF, int* mFx, int* aNF, int* mNFx, int* hashTable, int* nPrev, int m, int* sir,
	int* sirNotFound, int* mF, int* mNF) {
	int n = (int)((float)m * factor);

	for (int i = *nPrev; i < n; i++)
		insert(hashTable, sir[i], m);
	*nPrev = n;
	int incrementSearch = n / 1500;
	for (int i = 0; i < n; i += incrementSearch) {
		int attempts = searchNo(hashTable, sir[i], m);
		*aF += attempts;
		if (*mF < attempts)
			*mF = attempts;
	}
	for (int i = 0; i < 1500; i++) {
		int attempts = searchNo(hashTable, sirNotFound[i], m);
		*aNF += attempts;
		if (*mNF < attempts)
			*mNF = attempts;
	}
	*mFx += *mF;
	*mNFx += *mNF;
}

void averageCase() {
	int m = 10007;
	printf("Factor de umplere   Efort mediu gasite   Efort maxim gasite   Efort mediu negasite   Efort maxim negasite");
	int avgFound8 = 0, maxFound8 = 0, avgNotFound8 = 0, maxNotFound8 = 0;
	int avgFound85 = 0, maxFound85 = 0, avgNotFound85 = 0, maxNotFound85 = 0;
	int avgFound9 = 0, maxFound9 = 0, avgNotFound9 = 0, maxNotFound9 = 0;
	int avgFound95 = 0, maxFound95 = 0, avgNotFound95 = 0, maxNotFound95 = 0;
	int avgFound99 = 0, maxFound99 = 0, avgNotFound99 = 0, maxNotFound99 = 0;

	for (int pass = 0; pass < 5; pass++) {
		int* hashTable = (int*)calloc(m, sizeof(int));

		int* sir = (int*)malloc(m * sizeof(int));
		FillRandomArray(sir, m);

		int* sirNotFound = (int*)malloc(1500 * sizeof(int));
		FillRandomArray(sirNotFound, 1500, 50001, 60000);
		int nPrev = 0;
		int maxFound = 0, maxNotFound = 0;
		//alpha 0.8 
		Evaluare(0.8, &avgFound8, &maxFound8, &avgNotFound8, &maxNotFound8, hashTable, &nPrev, m, sir, sirNotFound
			, &maxFound, &maxNotFound);
		//alpha 0.85
		Evaluare(0.85, &avgFound85, &maxFound85, &avgNotFound85, &maxNotFound85, hashTable, &nPrev, m, sir, sirNotFound
			, &maxFound, &maxNotFound);
		//alpha 0.9
		Evaluare(0.9, &avgFound9, &maxFound9, &avgNotFound9, &maxNotFound9, hashTable, &nPrev, m, sir, sirNotFound
			, &maxFound, &maxNotFound);
		//alpha 0.95
		Evaluare(0.95, &avgFound95, &maxFound95, &avgNotFound95, &maxNotFound95, hashTable, &nPrev, m, sir, sirNotFound
			, &maxFound, &maxNotFound);
		//alpha 0.99
		Evaluare(0.99, &avgFound99, &maxFound99, &avgNotFound99, &maxNotFound99, hashTable, &nPrev, m, sir, sirNotFound
			, &maxFound, &maxNotFound);
	}


	printf("\n%.2f                  %.2f                  %.2f                   %.2f                   %.2f",
		0.80, (float)avgFound8 / 5 / 1500, (float)maxFound8 / 5, (float)avgNotFound8 / 5 / 1500, (float)maxNotFound8 / 5);

	printf("\n%.2f                  %.2f                  %.2f                   %.2f                  %.2f",
		0.85, (float)avgFound85 / 5 / 1500, (float)maxFound85 / 5, (float)avgNotFound85 / 5 / 1500, (float)maxNotFound85 / 5);

	printf("\n%.2f                  %.2f                  %.2f                   %.2f                  %.2f",
		0.90, (float)avgFound9 / 5 / 1500, (float)maxFound9 / 5, (float)avgNotFound9 / 5 / 1500, (float)maxNotFound9 / 5);

	printf("\n%.2f                  %.2f                  %.2f                   %.2f                  %.2f",
		0.95, (float)avgFound95 / 5 / 1500, (float)maxFound95 / 5, (float)avgNotFound95 / 5 / 1500, (float)maxNotFound95 / 5);

	printf("\n%.2f                  %.2f                  %.2f                  %.2f                 %.2f",
		0.99, (float)avgFound99 / 5 / 1500, (float)maxFound99 / 5, (float)avgNotFound99 / 5 / 1500, (float)maxNotFound99 / 5);
}

int main() {
	hashTest();
	averageCase();

	return 0;

}