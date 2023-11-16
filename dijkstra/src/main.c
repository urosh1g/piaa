#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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

static void test(FILE* sink, edge_generator_t generator);
static void generate_edges(graph_t* graph, int num_nodes, edge_generator_t generator);
static char* generator_name(edge_generator_t generator);


int main(int argc, char** argv) {
	FILE* time_fd = stdout;
	srand(time(0));
	if(argc == 2) {
		if(!(time_fd = fopen(argv[1], "w"))) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	test(time_fd, DEFAULT);
	test(time_fd, DAISY_CHAIN);
	if(time_fd != stdout) fclose(time_fd);
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

static void test(FILE* sink, edge_generator_t generator) {
	fprintf(sink, "Using %s\n", generator_name(generator));
	for(size_t test_num = 0; test_num < num_tests; test_num++) {
		if(test_num == num_tests - 1 && generator == DEFAULT) {
			fprintf(sink, "Za kombinacije N: %lu i K ne vredi cekati (traje duze od minut)\n", test_size[test_num]);
			return;
		}
		graph = create_graph(UNDIRECTED);
		for(size_t i = 0; i < test_size[test_num]; i++)
			insert_node(graph, i);
		generate_edges(graph, graph->num_nodes, generator);
		for(size_t i = 0; i < ksize; i++) {
			if(test_num == num_tests - 1 && generator == DAISY_CHAIN && k[i] == 33) {
				fprintf(sink, "Za kombinacije N: %lu i K >= 33 ne vredi cekati (traje duze od minut)\n", test_size[test_num]);
				return;
			}
			fprintf(sink, "N: %lu K: %lu\n", test_size[test_num], k[i]);
			int num_edges = k[i];
			while(num_edges) {
				int src = rand() % test_size[test_num];
				int dst = rand() % test_size[test_num];
				if(find_edge(graph, src, dst)) continue;
				insert_edge(graph, src, dst, rand() % MAX_WEIGHT);
				--num_edges;
			}
			clock_t t_start = clock();
			for(size_t j = 0; j < 10; j++) {
				int src = rand() % graph->num_nodes;
				int dst = rand() % graph->num_nodes;
				node_t* dest = dijkstra(
							graph,
							src, 
							dst
						);
				printf("Shortest distance from %d to %d => %d\n", src, dst, dest->dist);

			}
			clock_t t_end = clock();
			double elapsed = 1000.f * (t_end - t_start) / CLOCKS_PER_SEC;
			fprintf(sink, "10 pairs of shortest path: %.3fms\n", elapsed);
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
