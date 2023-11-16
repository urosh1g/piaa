#ifndef _DISJOIN_SET_H
#define _DISJOIN_SET_H

#define size_t unsigned long

#include "node.h"
#include "hash_table.h"

typedef struct {
	size_t key, rank;
	node_t* value;
} set_elem_t;

typedef struct {
	set_elem_t** elements;
	hash_table_t* _table;
	size_t nelems, size, ncomponents;
} disjoint_set_t;

set_elem_t* create_elem(node_t* value);
disjoint_set_t* create_set(size_t size);
void make_set(disjoint_set_t* set, node_t* node);
set_elem_t* find_set(disjoint_set_t* set, node_t* elem);
set_elem_t* unify(disjoint_set_t* set, node_t* elem1, node_t* elem2);
void destroy_set(disjoint_set_t* set);

#endif
