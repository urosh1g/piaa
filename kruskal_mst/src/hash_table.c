#include <stdint.h>
#include "../include/hash_table.h"

hash_table_t* create_hash_table(size_t size) {
	hash_table_t* table = (hash_table_t*)malloc(sizeof(hash_table_t));
	table->size = size;
	table->nentries = 0;
	table->entries = (table_entry_t**)
		malloc(table->size * sizeof(table_entry_t));
	for(size_t i = 0; i < table->size; i++)
		table->entries[i] = NULL;
	return table;
}

//Robert Jenkins' 32 bit integer hash function
static size_t hash(node_t* key1) {
	uint32_t key = (uint32_t)((uintptr_t)key1);
	key = (key+0x7ed55d16) + (key<<12);
	key = (key^0xc761c23c) ^ (key>>19);
	key = (key+0x165667b1) + (key<<5);
	key = (key+0xd3a2646c) ^ (key<<9);
	key = (key+0xfd7046c5) + (key<<3);
	key = (key^0xb55a4f09) ^ (key>>16);
	return (size_t)key;
}

table_entry_t* new_entry(node_t* key, int32_t value, table_entry_t* next) {
	table_entry_t* entry = (table_entry_t*)
		malloc(sizeof(table_entry_t));
	entry->key = key;
	entry->value = value;
	entry->next = next;
	return entry;
}

void table_insert(hash_table_t* table, node_t* key, int32_t value) {
	size_t idx = hash(key) % table->size;
	if(!table->entries[idx])
		table->entries[idx] = new_entry(key, value, NULL);
	else
		table->entries[idx] = new_entry(key, value, table->entries[idx]);
	table->nentries++;
}

int32_t table_at(hash_table_t* table, node_t* key) {
	size_t idx = hash(key) % table->size;
	table_entry_t* entry = table->entries[idx];
	while(entry && entry->key != key)
		entry = entry->next;
	if(entry)
		return entry->value;
	return -1;
}

void destroy_table_entry(table_entry_t* entry) {
	if(entry->next)
		destroy_table_entry(entry->next);
	free(entry);
}

void destroy_hash_table(hash_table_t* table) {
	for(size_t i = 0; i < table->size; i++)
		if(table->entries[i])
			destroy_table_entry(table->entries[i]);
	free(table->entries);
	free(table);
}
