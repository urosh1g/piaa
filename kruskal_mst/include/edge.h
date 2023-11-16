#ifndef _GRAPH_EDGE_H
#define _GRAPH_EDGE_H

typedef struct node node_t;

typedef struct edge {
	int weight;
	node_t* src, *dest;
	struct edge* next;
	struct edge* _knext;
} edge_t;

#endif
