#include <stdio.h>
#include <stdlib.h>

#include "../include/graph.h"
#include "../include/fib_heap.h"
#include "../include/disjoint_set.h"

static edge_t* create_edge(node_t* first, node_t* second, int weight);
static int edge_cmp(const void*, const void*);
static edge_t* sort_edges(const graph_t* graph, size_t* count);
static void rm_edge(edge_t* edge);
static void rm_node(node_t* node);

graph_t* create_graph(graph_type_t type) {
	graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
	graph->nodes = NULL;
	graph->type = type;
	graph->num_nodes = 0;
	return graph;
}

node_t* insert_node(graph_t* graph, int value) {
	node_t* node = create_node(value, graph->nodes);
	graph->nodes = node;
	graph->num_nodes++;
	return node;
}

edge_t* insert_edge(graph_t* graph, int first, int second, int weight) {
	node_t* first_node = find_node(graph, first);
	node_t* second_node = find_node(graph, second);
	if(!first || !second) return NULL;
	edge_t* edge = create_edge(first_node, second_node, weight);
	if(graph->type == UNDIRECTED)
		edge = create_edge(second_node, first_node, weight);
	return edge;
}

void print(graph_t* graph) {
	node_t* node = graph->nodes;
	while(node) {
		printf("[%d](%d)", node->value, node->dist);
		edge_t* edge = node->adj;
		while(edge) {
			printf(" -> (%d, w:%d)", edge->dest->value, edge->weight);
			edge = edge->next;
		}
		printf("\n");
		node = node->next;
	}
}

node_t* find_node(graph_t* graph, int value) {
	node_t* node = graph->nodes;

	while(node) {
		if(node->value == value)
			return node;
		node = node->next;
	}

	return NULL;
}

edge_t* find_edge(graph_t* graph, int source, int destination) {
	node_t* src = find_node(graph, source);
	node_t* dst = find_node(graph, destination);

	if(!src || !dst) return NULL;

	edge_t* edge = src->adj;
	while(edge && edge->dest != dst)
		edge = edge->next;

	return edge;
}

static edge_t* create_edge(node_t* first, node_t* second, int weight) {
	edge_t* edge = (edge_t*)malloc(sizeof(edge_t));
	edge->weight = weight;
	edge->next = first->adj;
	edge->src = first;
	edge->dest = second;
	first->adj = edge;
	return edge;
}

edge_t* kruskal(const graph_t* graph) {
	edge_t* mst_edges = NULL;
	node_t* node_iter = NULL;
	disjoint_set_t* set; 
	size_t edges = 0;

	set = create_set(graph->num_nodes);
	edge_t* sorted = sort_edges(graph, &edges);
	node_iter = graph->nodes;
	while(node_iter) {
		make_set(set, node_iter);
		node_iter = node_iter->next;
	}

	for(size_t i = 0; i < edges; i++) {
		if(find_set(set, sorted[i].src) !=
		   find_set(set, sorted[i].dest))
		{
			edge_t* edge = create_edge(
					sorted[i].src,
					sorted[i].dest,
					sorted[i].weight
					);

			if(!mst_edges) {
				mst_edges = edge;
			}
			else {
				edge->_knext = mst_edges;
				mst_edges = edge;
			}
			unify(set, sorted[i].src, sorted[i].dest);
		}
	}

	destroy_set(set);
	free(sorted);
	return mst_edges;
}

static int edge_cmp(const void* a, const void* b) {
	return ((edge_t*)a)->weight - ((edge_t*)b)->weight;
}

static edge_t* sort_edges(const graph_t* graph, size_t* count) {
	node_t* node = graph->nodes;
	size_t edges_len = ((graph->num_nodes * (graph->num_nodes - 1)) / 2) + 1;
	size_t edges_cnt = 0;
	edge_t* edges = (edge_t*)malloc(edges_len * sizeof(edge_t));
	while(node) {
		edge_t* edge = node->adj;
		while(edge) {
			edges[edges_cnt++] = *edge;
			edge = edge->next;
		}
		node = node->next;
	}
	*count = edges_cnt;
	qsort(edges, edges_cnt, sizeof(edges[0]), edge_cmp);
	return edges;
}

static void rm_node(node_t* node) {
	if(!node) return;
	edge_t* edge = node->adj;
	rm_edge(edge);
	free(node);
}

static void rm_edge(edge_t* edge) {
	if(!edge) return;
	rm_edge(edge->next);
	free(edge);
}

void destroy_graph(graph_t* graph) {
	node_t* node = graph->nodes;
	node_t* next = NULL;
	while(node) {
		next = node->next;
		rm_node(node);
		node = next;
	}
	free(graph);
}

node_t* dijkstra(graph_t* graph, int start_node, int end_node) {
	fib_heap_t* heap = create_fib_heap();
	node_t* node_iter = graph->nodes;
	edge_t* edge_iter = NULL;

	node_t* start = find_node(graph, start_node);
	node_t* end   = find_node(graph, end_node);
	if(!start || !end) goto cleanup;

	start->dist = 0;
	while(node_iter) {
		if(node_iter != start) {
			node_iter->dist = INT32_MAX;
			node_iter->_dprev = NULL;
		}
		fib_heap_insert(heap, node_iter);
		node_iter = node_iter->next;
	}

	while(heap->min) {
		node_t* current = fib_heap_extract_min(heap);
		if(current == end) break;
		edge_iter = current->adj;
		while(edge_iter) {
			int32_t alt_dist = current->dist + edge_iter->weight;
			if(alt_dist < edge_iter->dest->dist) {
				edge_iter->dest->dist = alt_dist;
				edge_iter->dest->_dprev = current;
				fib_heap_decrease_key(heap, edge_iter->dest, alt_dist);
			}
			edge_iter = edge_iter->next;
		}
	}

cleanup:
	fib_heap_destroy(heap);
	return end;
}
