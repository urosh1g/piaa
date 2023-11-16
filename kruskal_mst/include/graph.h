#ifndef _GRAPH_H
#define _GRAPH_H
#include <stdio.h>

#include "node.h"
#include "edge.h"

typedef enum graph_type {
	DIRECTED = 0,
	UNDIRECTED
} graph_type_t;

typedef struct graph {
	graph_type_t type;
	size_t num_nodes;
	node_t* nodes;
} graph_t;

graph_t* create_graph(graph_type_t type);
node_t* insert_node(graph_t* graph, int value);
void remove_node(graph_t* graph, int value);
edge_t* insert_edge(graph_t* graph, int source, int destination, int weight);
node_t* find_node(graph_t* graph, int value);
edge_t* find_edge(graph_t* graph, int source, int destination);
edge_t* kruskal(const graph_t* graph);
void dijkstra(graph_t* graph, int start);
void remove_edge(graph_t* graph, int source, int destination);
void print(graph_t* graph);
void dot_print_graph(graph_t* graph, FILE* sink);
void destroy_graph(graph_t* graph);

#endif
