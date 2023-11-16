#include <stdio.h>
#include <stdlib.h>

#include "../include/disjoint_set.h"

set_elem_t* create_elem(node_t* value) {
	set_elem_t* elem = (set_elem_t*)malloc(sizeof(set_elem_t));
	elem->rank = 0;
	elem->value = value;
	return elem;
}

disjoint_set_t* create_set(size_t size) {
	disjoint_set_t* set = (disjoint_set_t*)malloc(sizeof(disjoint_set_t));
	set->elements = (set_elem_t**)malloc(size * sizeof(set_elem_t*));
	for(size_t i = 0; i < size; i++)
		set->elements[i] = NULL;
	set->size = size;
	set->nelems = set->ncomponents = 0;
	set->_table = create_hash_table(size);
	return set;
}

void make_set(disjoint_set_t* set, node_t* node) {
	if(set->nelems == set->size)
		return;
	set_elem_t* elem = create_elem(node);
	elem->key = set->nelems;
	set->elements[set->nelems++] = elem;
	set->ncomponents++;
	table_insert(set->_table, node, elem->key);
	return;
}

set_elem_t* find_set(disjoint_set_t* set, node_t* elem) {
	int32_t idx = table_at(set->_table, elem);
	if(idx == -1) return NULL;
	if(set->elements[idx]->key == (size_t)idx)
		return set->elements[idx];
	set->elements[idx] = find_set(set, set->elements[idx]->value);
	return set->elements[idx];
}

set_elem_t* unify(disjoint_set_t* set, node_t* elem1, node_t* elem2) {
	set_elem_t* root1 = find_set(set, elem1);
	set_elem_t* root2 = find_set(set, elem2);
	if(root1 == root2) return NULL;
	set->ncomponents--;
	if(root1->rank > root2->rank)
		set->elements[root2->key] = root1;
	else {
		set->elements[root1->key] = root2;
		if(root1->rank == root2->rank)
			root2->rank++;
	}
	return root2;
}

void destroy_set(disjoint_set_t* set) {
	free(set->elements);
	destroy_hash_table(set->_table);
	free(set);
}
