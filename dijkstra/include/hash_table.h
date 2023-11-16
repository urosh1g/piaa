#ifndef _HTABLE_H
#define _HTABLE_H

#include <stdlib.h>
#include "node.h"

typedef struct entry {
	int32_t value;
	node_t* key;
	struct entry* next;
} table_entry_t;

typedef struct {
	table_entry_t** entries;
	size_t size;
	size_t nentries;
} hash_table_t;

table_entry_t* new_entry(node_t* key, int32_t value, table_entry_t* next);
hash_table_t* create_hash_table(size_t size);
void table_insert(hash_table_t* table, node_t* key, int32_t value);
int32_t table_at(hash_table_t* table, node_t* key);
void destroy_table_entry(table_entry_t* entry);
void destroy_hash_table(hash_table_t* table);

#endif
