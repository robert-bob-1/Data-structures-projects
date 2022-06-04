#include <stdlib.h>
#include <string.h>
#include "bfs.h"

void insert_point(Point neighb[], int count, int row, int col) {
	Point* newPoint = (Point*)malloc(sizeof(Point));
	newPoint->col = col;
	newPoint->row = row;
	neighb[count] = *newPoint;
}

int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
	// TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
	// the point p will have at most 4 neighbors (up, down, left, right)
	// avoid the neighbors that are outside the grid limits or fall into a wall
	// note: the size of the array neighb is guaranteed to be at least 4
	int col = p.col;
	int row = p.row;
	int count = -1;

	if (grid->mat[row][col - 1] == 0)
		insert_point(neighb, ++count, row, col - 1);
	if (grid->mat[row][col + 1] == 0)
		insert_point(neighb, ++count, row, col + 1);
	if (grid->mat[row - 1][col] == 0)
		insert_point(neighb, ++count, row - 1, col);
	if (grid->mat[row + 1][col] == 0)
		insert_point(neighb, ++count, row + 1, col);

	return count + 1;
}

void grid_to_graph(const Grid* grid, Graph* graph)
{
	//we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
	Node* nodes[MAX_ROWS][MAX_COLS];
	int i, j, k;
	Point neighb[4];

	//compute how many nodes we have and allocate each node
	graph->nrNodes = 0;
	for (i = 0; i < grid->rows; ++i) {
		for (j = 0; j < grid->cols; ++j) {
			if (grid->mat[i][j] == 0) {
				nodes[i][j] = (Node*)malloc(sizeof(Node));
				memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
				nodes[i][j]->position.row = i;
				nodes[i][j]->position.col = j;
				++graph->nrNodes;
			}
			else {
				nodes[i][j] = NULL;
			}
		}
	}
	graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
	k = 0;
	for (i = 0; i < grid->rows; ++i) {
		for (j = 0; j < grid->cols; ++j) {
			if (nodes[i][j] != NULL) {
				graph->v[k++] = nodes[i][j];
			}
		}
	}

	//compute the adjacency list for each node
	for (i = 0; i < graph->nrNodes; ++i) {
		graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
		if (graph->v[i]->adjSize != 0) {
			graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
			k = 0;
			for (j = 0; j < graph->v[i]->adjSize; ++j) {
				if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
					neighb[j].col >= 0 && neighb[j].col < grid->cols &&
					grid->mat[neighb[j].row][neighb[j].col] == 0) {
					graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
				}
			}
			if (k < graph->v[i]->adjSize) {
				//get_neighbors returned some invalid neighbors
				graph->v[i]->adjSize = k;
				graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
			}
		}
	}
}

void free_graph(Graph* graph)
{
	if (graph->v != NULL) {
		for (int i = 0; i < graph->nrNodes; ++i) {
			if (graph->v[i] != NULL) {
				if (graph->v[i]->adj != NULL) {
					free(graph->v[i]->adj);
					graph->v[i]->adj = NULL;
				}
				graph->v[i]->adjSize = 0;
				free(graph->v[i]);
				graph->v[i] = NULL;
			}
		}
		free(graph->v);
		graph->v = NULL;
	}
	graph->nrNodes = 0;
}

typedef struct QNode {
	QNode* next;
	Node* node;
};

typedef struct Queue {
	QNode* first, * last;
};

//3 operatii pe node
QNode* createQNode(Node* node, int dist, Node* parent) {
	QNode* newNode = (QNode*)malloc(sizeof(QNode));
	newNode->next = NULL;
	newNode->node = node;
	node->dist = dist;
	node->parent = parent;
	return newNode;
}
void enqueue(Queue* q, Node* node, int dist, Node* parent) {
	if (q->first == NULL) {
		q->first = q->last = createQNode(node, dist, parent);
		return;
	}
	QNode* newNode = createQNode(node, dist, parent);
	q->last->next = newNode;
	q->last = newNode;
	return;
}

Node* dequeue(Queue* q) {
	if (q->first == NULL) {
		puts("queue empty");
		return NULL;
	}
	if (q->first == q->last) {
		QNode* newNode = q->first;
		q->first = q->last = NULL;
		Node* node = newNode->node;
		free(newNode);
		return node;
	}
	QNode* newNode = q->first;
	q->first = q->first->next;
	Node* node = newNode->node;
	free(newNode);

	return node;
}

void bfs(Graph* graph, Node* s, Operation* op)
{
	// TOOD: implement the BFS algorithm on the graph, starting from the node s
	// at the end of the algorithm, every node reachable from s should have the color BLACK
	// for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
	// for counting the number of operations, the optional op parameter is received
	// since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
	// if(op != NULL) op->count();
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->first = NULL;
	q->last = NULL;

	//3 operatii pe node
	if (op != NULL) op->count(3);
	enqueue(q, s, 0, NULL);
	s->color = COLOR_GRAY;

	while (q->first != NULL) {

		//1 operatii pe node +color deci 2 operatii
		if (op != NULL) op->count(2);
		Node* node = dequeue(q);
		//node->color = COLOR_BLACK;

		for (int i = 0; i < node->adjSize; i++) {
			if (op != NULL) op->count(1);//1 accesari la node
			if (node->adj[i]->color == COLOR_WHITE)
			{
				if (op != NULL) op->count(2 + 3);//2 in apelarea enqueue si 3 in enqueue
				node->adj[i]->color = COLOR_GRAY;

				enqueue(q, node->adj[i], node->dist + 1, node);
			}
		}
		node->color = COLOR_BLACK;
	}
}

void spaces(int depth, Point repr) {
	for (int d = 0; d < depth; d++) {
		printf("   ");
	}
	printf("(%d, %d)\n", repr.row, repr.col);
}
void ppR1(int* p, int n, int last, int depth, Point* repr) {

	for (int j = 0; j < n; j++) {
		if (p[j] == last) {
			spaces(depth, repr[j]);
			ppR1(p, n, j, depth + 1, repr);
		}
	}
}


void print_bfs_tree(Graph* graph)
{
	//first, we will represent the BFS tree as a parent array
	int n = 0; //the number of nodes
	int* p = NULL; //the parent array
	Point* repr = NULL; //the representation for each element in p

	//some of the nodes in graph->v may not have been reached by BFS
	//p and repr will contain only the reachable nodes
	int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
	for (int i = 0; i < graph->nrNodes; ++i) {
		if (graph->v[i]->color == COLOR_BLACK) {
			transf[i] = n;
			++n;
		}
		else {
			transf[i] = -1;
		}
	}
	if (n == 0) {
		//no BFS tree
		free(transf);
		return;
	}

	int err = 0;
	p = (int*)malloc(n * sizeof(int));
	repr = (Point*)malloc(n * sizeof(Node));
	for (int i = 0; i < graph->nrNodes && !err; ++i) {
		if (graph->v[i]->color == COLOR_BLACK) {
			if (transf[i] < 0 || transf[i] >= n) {
				err = 1;
			}
			else {
				repr[transf[i]] = graph->v[i]->position;
				if (graph->v[i]->parent == NULL) {
					p[transf[i]] = -1;
				}
				else {
					err = 1;
					for (int j = 0; j < graph->nrNodes; ++j) {
						if (graph->v[i]->parent == graph->v[j]) {
							if (transf[j] >= 0 && transf[j] < n) {
								p[transf[i]] = transf[j];
								err = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	free(transf);
	transf = NULL;

	if (!err) {
		// TODO: pretty print the BFS tree
		// the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
		// when printing the node k, print repr[k] (it contains the row and column for that point)
		// you can adapt the code for transforming and printing multi-way trees from the previous labs
		ppR1(p, n, -1, 0, repr);
	}

	if (p != NULL) {
		free(p);
		p = NULL;
	}
	if (repr != NULL) {
		free(repr);
		repr = NULL;
	}
}

int shortest_path(Graph* graph, Node* start, Node* end, Node* path[])
{
	// TODO: compute the shortest path between the nodes start and end in the given graph
	// the nodes from the path, should be filled, in order, in the array path
	// the number of nodes filled in the path array should be returned
	// if end is not reachable from start, return -1
	// note: the size of the array path is guaranteed to be at least 1000
	bfs(graph, end, NULL);
	Node* node = start;
	int idx = -1;
	while (node != end && node != NULL) {
		path[idx++] = node;
		node = node->parent;
	}
	if (node == end)
		return idx;
	return -1;
}


void performance()
{
	int n, i;
	Profiler p("bfs");

	// vary the number of edges
	for (n = 1000; n <= 4500; n += 100) {
		Operation op = p.createOperation("bfs-edges", n);
		Graph graph;
		graph.nrNodes = 100;
		//initialize the nodes of the graph
		graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
		for (i = 0; i < graph.nrNodes; ++i) {
			graph.v[i] = (Node*)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		// TODO: generate n random edges
		// make sure the generated graph is connected

		// generam un lant random
		int sirNoduri[100];
		FillRandomArray(sirNoduri, graph.nrNodes, 0, graph.nrNodes - 1, true);

		for (int i = 0; i < graph.nrNodes; i++) {
			graph.v[sirNoduri[i]]->adj = (Node**)malloc(sizeof(Node*));
		}

		for (i = 0; i < graph.nrNodes - 1; i++) {
			graph.v[sirNoduri[i]]->adjSize++;
			graph.v[sirNoduri[i]]->adj[0] = graph.v[sirNoduri[i + 1]];
		}

		//generam muchii random in continuare pana la n
		int sirNoduri1[4500];
		FillRandomArray(sirNoduri1, n - 100, 0, graph.nrNodes - 1, false);
		for (int j = 0; j < n - 101; j++) {
			graph.v[sirNoduri1[j]]->adjSize++;
			if (graph.v[sirNoduri1[j]]->adjSize > sizeof(graph.v[sirNoduri1[j]]->adj) / sizeof(Node*)) {
				graph.v[sirNoduri1[j]]->adj = (Node**)realloc(graph.v[sirNoduri1[j]]->adj,
					2 * graph.v[sirNoduri1[j]]->adjSize * sizeof(Node*));
			}
			graph.v[sirNoduri1[j]]->adj[graph.v[sirNoduri1[j]]->adjSize - 1]
				= graph.v[sirNoduri1[j + 1]];
		}


		bfs(&graph, graph.v[0], &op);
		free_graph(&graph);
	}

	// vary the number of vertices
	for (n = 100; n <= 200; n += 10) {
		Operation op = p.createOperation("bfs-vertices", n);
		Graph graph;
		graph.nrNodes = n;
		//initialize the nodes of the graph
		graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
		for (i = 0; i < graph.nrNodes; ++i) {
			graph.v[i] = (Node*)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		// TODO: generate 4500 random edges
		// make sure the generated graph is connected

		int sirNoduri[200];
		FillRandomArray(sirNoduri, graph.nrNodes, 0, graph.nrNodes - 1, true);

		for (int i = 0; i < graph.nrNodes; i++) {
			graph.v[sirNoduri[i]]->adj = (Node**)malloc(sizeof(Node*));
		} 

		for (i = 0; i < graph.nrNodes - 1; i++) {
			graph.v[sirNoduri[i]]->adjSize++;
			graph.v[sirNoduri[i]]->adj[0] = graph.v[sirNoduri[i + 1]];
		}

		//generam muchii random in continuare pana la n
		int sirNoduri1[4500];
		FillRandomArray(sirNoduri1, 4500-n, 0, graph.nrNodes - 1, false);
		for (int j = 0; j < 4500-n-1; j++) {
			graph.v[sirNoduri1[j]]->adjSize++;
			if (graph.v[sirNoduri1[j]]->adjSize > sizeof(graph.v[sirNoduri1[j]]->adj) / sizeof(Node*)) {
				graph.v[sirNoduri1[j]]->adj = (Node**)realloc(graph.v[sirNoduri1[j]]->adj,
					2 * graph.v[sirNoduri1[j]]->adjSize * sizeof(Node*));
			}
			graph.v[sirNoduri1[j]]->adj[graph.v[sirNoduri1[j]]->adjSize - 1]
				= graph.v[sirNoduri1[j + 1]];
		}



		bfs(&graph, graph.v[0], &op);
		free_graph(&graph);
	}

	p.showReport();
}
