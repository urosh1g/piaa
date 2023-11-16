#ifndef _GRAPH_NODE_H
#define _GRAPH_NODE_H

typedef struct edge edge_t;

typedef enum status {
	NOTVISITED = 0,
	VISITING = 1,
	VISITED  = 2
} node_status_t;

typedef struct node {
	int value;
	int dist;
	node_status_t status;
	edge_t* adj;
	struct node* next;
	struct node* _dprev;
} node_t;

node_t* create_node(int value, node_t* next);

#endif
