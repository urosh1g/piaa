#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/graph.h"

#define MAX_WEIGHT 1000

typedef enum {
	DEFAULT = 0,
	DAISY_CHAIN
} edge_generator_t;

static size_t test_size[] = { 100, 1000, 10000, 100000 };
static size_t num_tests = sizeof(test_size) / sizeof(test_size[0]);

static size_t k[] = { 2, 5, 10, 20, 33, 50 };
static size_t ksize = sizeof(k) / sizeof(k[0]);

static graph_t* graph = NULL;

static void test(edge_generator_t generator);
static void generate_edges(graph_t* graph, int num_nodes, edge_generator_t generator);
static char* generator_name(edge_generator_t generator);


int main() {
	srand(time(0));
	test(DEFAULT);
	test(DAISY_CHAIN);
}

static void generate_edges(graph_t* graph, int num_nodes, edge_generator_t generator) {
	if(generator == DAISY_CHAIN) {
		for(int i = 0; i < num_nodes; i++) {
			insert_edge(graph, i, (i + 1) % num_nodes, rand() % MAX_WEIGHT);
		}
	}
	else {
		int node = rand() % num_nodes;
		for(int i = 0; i < num_nodes; i++) {
			if(node == i) continue;
			insert_edge(graph, node, i, rand() % MAX_WEIGHT);
		}
	}
}

static void test(edge_generator_t generator) {
	printf("Using %s\n", generator_name(generator));
	for(size_t test_num = 0; test_num < num_tests; test_num++) {
		if(test_num == num_tests - 1) {
			printf("Za kombinacije N:%lu i k ne vredi cekati\n", test_size[test_num]);
			break;
		}
		graph = create_graph(UNDIRECTED);
		for(size_t i = 0; i < test_size[test_num]; i++)
			insert_node(graph, i);
		generate_edges(graph, graph->num_nodes, generator);
		for(size_t i = 0; i < ksize; i++) {
			int num_edges = k[i];
			while(num_edges) {
				int src = rand() % test_size[test_num];
				int dst = rand() % test_size[test_num];
				if(find_edge(graph, src, dst)) continue;
				insert_edge(graph, src, dst, rand() % MAX_WEIGHT);
				--num_edges;
			}
			printf("Kruskal for N:%lu k:%lu ", test_size[test_num], k[i]);
			clock_t t_start = clock();
			kruskal(graph);
			clock_t t_end = clock();
			printf("%.3fms\n", 1000 * (double)(t_end - t_start) / CLOCKS_PER_SEC);
		}
		destroy_graph(graph);
	}
	return;
}

static char* generator_name(edge_generator_t generator) {
	switch(generator) {
	case DEFAULT:
		return "DEFAULT";
	case DAISY_CHAIN:
		return "DAISY CHAIN";
	default:
		return "UNKNOWN";
	}
}
