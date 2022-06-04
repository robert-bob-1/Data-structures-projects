#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"
Profiler p("profiler");

typedef struct Node {
	Node* p;
	int rank;
	int key;
};

typedef struct Edge {
	int u, v;
	int weight;
};

typedef struct Graph {
	int* nodes;
	int nodesNo;
	Edge* edges;
	int edgeNo;
};

Node* makeSet(int x, Operation comp, Operation attr) {
	Node* node = (Node*)malloc(sizeof(Node));
	attr.count(3);
	node->p = node;
	node->rank = 0;
	node->key = x;
	return node;
}

Node* findSet(Node* x, Operation comp, Operation attr) {
	comp.count();
	if (x != x->p) {
		attr.count();
		x->p = findSet(x->p, comp, attr);
	}
	return x->p;
}

void link(Node* x, Node* y, Operation comp, Operation attr) {
	comp.count();
	if (x->rank > y->rank) {
		attr.count();
		y->p = x;
	}
	else {
		attr.count();
		x->p = y;
		comp.count();
		if (x->rank == y->rank) {
			attr.count();
			y->rank++;
		}
	}
}

void union_(Node* x, Node* y, Operation comp, Operation attr) {
	link(findSet(x, comp, attr), findSet(y, comp, attr), comp, attr);
}

void sort(Edge* edges, int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		Edge key = edges[i];
		j = i - 1;
		while (j >= 0 && edges[j].weight > key.weight)
		{
			edges[j + 1] = edges[j];
			j = j - 1;
		}
		edges[j + 1] = key;
	}
}

void afisareParinti(Node* node) {
	if (node->p == node)
		printf("%d", node->key);
	else
	{
		printf("%d ->", node->key);
		afisareParinti(node->p);
	}
}
void afisareSet(Node** sets, int size) {
	for (int i = 0; i < size; i++) {
		printf("\n%d ", sets[i]->key);
		if (sets[i]->p != sets[i]) {
			printf("-> ");
			afisareParinti(sets[i]->p);
		}
	}
	puts("");
}


int afisare = 1;
Edge* kruskal(Graph g, Operation comp, Operation attr) {
	Node** sets = (Node**)malloc(g.nodesNo * sizeof(Node*));
	Edge* A = (Edge*)malloc(g.edgeNo * sizeof(Edge));
	int ACount = -1;
	for (int i = 0; i < g.nodesNo; i++) {
		sets[i] = makeSet(g.nodes[i], comp, attr);
	}

	//insertion sort
	sort(g.edges, g.edgeNo);//merge

	if (afisare == 1)
		for (int i = 0; i < g.edgeNo; i++) {
			printf("\n%d %d %d", g.edges[i].u, g.edges[i].v, g.edges[i].weight);
		}
	///////////////////////////////
	for (int i = 0; i < g.edgeNo; i++)
		if (findSet(sets[g.edges[i].u],comp,attr) != findSet(sets[g.edges[i].v], comp, attr))
		{
			A[++ACount] = g.edges[i];
			union_(sets[g.edges[i].u], sets[g.edges[i].v], comp, attr);
		}

	if (afisare == 1) {
		afisareSet(sets, g.nodesNo);
		afisare = 0;
	}
	return A;
}


void test() {

	Operation comp = p.createOperation("comp", 100);
	Operation attr = p.createOperation("attr", 100);
	puts("testam union");
	int a[] = { 1,2,3,4,5,6,7,8,9,10 };
	int size = 10;
	Node** sets = (Node**)malloc(size * sizeof(Node*));
	for (int i = 0; i < size; i++)
		sets[i] = makeSet(a[i], comp, attr);
	afisareSet(sets, size);
	union_(sets[0], sets[1], comp, attr);
	afisareSet(sets, size);
	union_(sets[0], sets[2], comp, attr);
	afisareSet(sets, size);
	union_(sets[3], sets[0], comp, attr);
	afisareSet(sets, size);

	union_(sets[5], sets[9], comp, attr);
	afisareSet(sets, size);
	union_(sets[9], sets[8], comp, attr);
	afisareSet(sets, size);
	union_(sets[7], sets[8], comp, attr);
	afisareSet(sets, size);
	union_(sets[6], sets[7], comp, attr);
	afisareSet(sets, size);

	union_(sets[0], sets[8], comp, attr);
	afisareSet(sets, size);
	union_(sets[4], sets[8], comp, attr);
	afisareSet(sets, size);

	
	Graph* g = (Graph*)malloc(sizeof(Graph));
	g->nodesNo = 9;
	g->nodes = (int*)malloc(g->nodesNo * sizeof(int));
	for (int i = 0; i < g->nodesNo; i++) {
		g->nodes[i] = i;
		//	printf("%d ", g->nodes[i]);
	}

	g->edgeNo = 14;
	g->edges = (Edge*)malloc(g->edgeNo * sizeof(Edge));
	g->edges[0].u = 0; g->edges[0].v = 1; g->edges[0].weight = 4;
	g->edges[1].u = 0; g->edges[1].v = 7; g->edges[1].weight = 8;

	g->edges[2].u = 1; g->edges[2].v = 7; g->edges[2].weight = 11;
	g->edges[3].u = 1; g->edges[3].v = 2; g->edges[3].weight = 8;

	g->edges[4].u = 2; g->edges[4].v = 8; g->edges[4].weight = 2;
	g->edges[5].u = 2; g->edges[5].v = 5; g->edges[5].weight = 4;
	g->edges[6].u = 2; g->edges[6].v = 3; g->edges[6].weight = 7;

	g->edges[7].u = 3; g->edges[7].v = 4; g->edges[7].weight = 9;
	g->edges[8].u = 3; g->edges[8].v = 5; g->edges[8].weight = 14;

	g->edges[9].u = 4; g->edges[9].v = 5; g->edges[9].weight = 10;

	g->edges[10].u = 5; g->edges[10].v = 6; g->edges[10].weight = 2;

	g->edges[11].u = 6; g->edges[11].v = 7; g->edges[11].weight = 1;
	g->edges[12].u = 6; g->edges[12].v = 8; g->edges[12].weight = 6;

	g->edges[13].u = 7; g->edges[13].v = 8; g->edges[13].weight = 7;

	Edge* A = kruskal(*g, comp, attr);
	puts("\n\nmuchiile din arbore");
	for (int i = 0; i < 8; i++) {
		printf("%d %d weight %d\n", A[i].u, A[i].v, A[i].weight);
	}
}


void averageCase() {
	for (int n = 100; n <= 10000; n += 100) {
		Operation comp = p.createOperation("comp", n);
		Operation attr = p.createOperation("attr", n);

		printf("%d/%d\r", n, 10000);

		Graph* g = (Graph*)malloc(sizeof(Graph));
		g->nodesNo = n;
		g->nodes = (int*)malloc(g->nodesNo * sizeof(int));
		for (int i = 0; i < g->nodesNo; i++) {
			g->nodes[i] = i;
		}

		g->edgeNo = n * 4;
		g->edges = (Edge*)malloc(g->edgeNo * sizeof(Edge));

		// generam un lant din noduri random
		int* sirNoduri = (int*)malloc(sizeof(int) * n * 4);
		FillRandomArray(sirNoduri, n, 0, n - 1, true);

		// generam un sir pentru weighturi random
		int* sirWeights = (int*)malloc(sizeof(int) * n * 4);
		FillRandomArray(sirWeights, n * 4, 0, n, false);


		for (int i = 0; i < n - 1; i++) {
			g->edges[i].u = sirNoduri[i];
			g->edges[i].v = sirNoduri[i + 1];
			g->edges[i].weight = sirWeights[i];
		}

		FillRandomArray(sirNoduri, n * 3 + 2, 0, n - 1, false);
		/*for (int i = 0; i < n * 4; i++) {
			printf("%d ", sirNoduri[i]);
		}
		puts("edges");*/
		//umplem restul n*3 pozitii de edgeuri
		for (int i = n - 1; i < n * 4; i++) {
			g->edges[i].u = sirNoduri[i - n + 1];
			g->edges[i].v = sirNoduri[i + 2 - n];
			g->edges[i].weight = sirWeights[i];
			//printf("\n%d %d %d", g->edges[i].u, g->edges[i].v, g->edges[i].weight);
		}

		kruskal(*g, comp, attr);



		free(sirNoduri);
		free(sirWeights);
	}
}




int main() {
	test();
	averageCase();

	p.addSeries("Total", "comp", "attr");

	p.createGroup("Multimi disjuncte", "comp", "attr", "Total");

	p.showReport();

}


