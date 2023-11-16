#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define DICT_CAPACITY 256
#define BUFF_SIZE 4096

#ifdef DEBUG
#define DBG(fmt, ...)\
	do { fprintf(stdout, fmt, __VA_ARGS__); } while(0)
#else
#define DBG(fmt, ...)
#endif

#include "priority_queue.h"
#include "util.h"

static size_t frequencies[DICT_CAPACITY] = {0};
static node_t* dict[DICT_CAPACITY];
static node_t* root = NULL;
static prioq_t* pq = NULL;

static void free_node(node_t* node) {
	free(node->code);
	node->code = NULL;
	free(node);
	node = NULL;
}

static void clean_tree(node_t* root) {
	if(!root) return;
	clean_tree(root->left);
	root->left = NULL;
	clean_tree(root->right);
	root->right = NULL;
	free_node(root);
}

static void get_freq(const char* msg) {
	size_t idx = 0;
	while(msg[idx]){
		frequencies[(size_t)msg[idx++]]++;
	}
} 

static void generate_tree() {
	pq = prioq_create(DICT_CAPACITY);
	for(size_t i = 0; i < DICT_CAPACITY; i++){
		if(frequencies[i]){
			node_t* n = node((uint8_t)i, frequencies[i], NULL, NULL);
			enqueue(pq, n);
		}
	}
	size_t tmp = 1;
	while(pq->idx != 2){
		node_t* left = dequeue(pq);
		node_t* right = dequeue(pq);
		node_t* parent = node('0' + tmp++, 
				left->freq + right->freq,
				left,
				right);
		enqueue(pq, parent);
	}
	root = dequeue(pq);
}

static void assign_codes(node_t* root, char* code, size_t len) {
	if(!root) return;
	if(leaf(root)){
		root->code = (char*)calloc(sizeof(char), len);
		strncpy(root->code, code, len);
		root->code_len = strlen(root->code);
		dict[(uint8_t)root->ch] = root;
		return;
	}

	//TODO fix [static buffer] :(
	char* left_code = (char*)calloc(sizeof(char), len + 2);
	strncpy(left_code, code, len);
	strcat(left_code, "0");
	assign_codes(root->left, left_code, len + 2);

	char* right_code = (char*)calloc(sizeof(char), len + 2);
	strncpy(right_code, code, len);
	strcat(right_code, "1");
	assign_codes(root->right, right_code, len + 2);

	free(left_code);
	free(right_code);
}

static char* encode_msg(const char* msg) {
	size_t sz = 1024;
	char* encoded = (char*)calloc(sizeof(char), sz);
	size_t len = 0;
	size_t idx = 0;
	while(msg[idx]){
		if(len + dict[(size_t)msg[idx]]->code_len >= sz){
			sz *= 2;
			encoded = (char*)realloc(encoded, sz);
		}
		strncat(encoded, dict[(size_t)msg[idx]]->code, dict[(size_t)msg[idx]]->code_len);
		len += dict[(size_t)msg[idx]]->code_len;
		idx++;
	}
	return encoded;
}

char* huffman_encode(const char* msg) {
	clock_t start_time, end_time;
	start_time = clock();
	get_freq(msg);
	generate_tree();
	char* start = (char*)"\0";
	assign_codes(root, start, 0);
	end_time = clock();
	printf("Time elapsed: %.3fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	char* enc = encode_msg(msg);
	clean_tree(root);
	return enc;
}

void encode_file(const char* input, const char* output) {
	FILE* in_fd, *out_fd, *stats_fd;
	clock_t start_time, end_time;
	size_t start_size, encoded_size;

	bzero(frequencies, DICT_CAPACITY);
	bzero(dict, DICT_CAPACITY);
	start_time = clock();

	stats_fd = fopen("stats.txt", "a+");

	in_fd = fopen(input, "r");
	long start_pos = ftell(in_fd);
	if(!in_fd) {
		fprintf(stderr, "ERROR: Error opening input file!\n");
		return;
	}

	out_fd = fopen(output, "wb");
	if(!out_fd) {
		fprintf(stderr, "ERROR: Error opening output file!\n");
		return;
	}
	
	while(true) {
		uint8_t ch = (uint8_t)fgetc(in_fd);
		start_size++;
		if(feof(in_fd)) break;
		frequencies[ch]++;
	}

	generate_tree();
	assign_codes(root, (char*)"\0", 0);

	uint16_t num = 0;
	for(size_t i = 0; i < DICT_CAPACITY; i++)
		if(frequencies[i] != 0)
			num++;
	DBG("%u entries for dictionary\n", num);
	size_t total_sz = 2;

	fwrite(&num, sizeof(uint16_t), 1, out_fd);
	for(size_t i = 0; i < DICT_CAPACITY; i++) {
		if(frequencies[i] == 0) continue;
		fwrite(&i, sizeof(uint8_t), 1, out_fd);
		total_sz += 1;
		fwrite(&frequencies[i], sizeof(size_t), 1, out_fd);
		total_sz += sizeof(size_t);
	}

	DBG("%lu total bytes for dictionary\n", total_sz);
	size_t bytes_written = 0;
	fseek(in_fd, start_pos, SEEK_SET);

	uint8_t byte = 0;
	uint8_t byte_sz = 0;
	while(!feof(in_fd)) {
		uint8_t ch = (uint8_t)fgetc(in_fd);
		if(feof(in_fd)) break;
		for(size_t i = 0; i < dict[ch]->code_len; i++) {
			if(byte_sz == 8) {
				fwrite(&byte, sizeof(uint8_t), 1, out_fd);
				bytes_written++;
				byte = byte_sz = 0;
			}
			byte <<= 1;
			byte |= dict[ch]->code[i] - '0';
			byte_sz++;
		}
	}
	uint8_t pad = 0;
	if(byte_sz) {
		pad = 8 - byte_sz;
		for(uint8_t i = 0; i < pad; i++)
			byte <<= 1;
		fwrite(&byte, sizeof(uint8_t), 1, out_fd);
		bytes_written++;
	}


	end_time = clock();
	fwrite(&bytes_written, sizeof(size_t), 1, out_fd);
	bytes_written += sizeof(size_t);
	fwrite(&pad, sizeof(uint8_t), 1, out_fd);
	bytes_written += sizeof(uint8_t);
	total_sz += bytes_written;

	double compression = 100 * (1 - total_sz / (double)start_size);

	fprintf(stats_fd, "File %s (%luB) compressed to %luB (%.2f %)\n", input, start_size, total_sz, compression);

	DBG("%lu bytes written for data\n", total_sz);
	DBG("encoding done in %.2fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	fclose(stats_fd);
	fclose(in_fd);
	fclose(out_fd);
	clean_tree(root);
}

void decode_file(const char* input, const char* output) {
	FILE* in_fd, *out_fd;
	clock_t start_time, end_time;
	size_t bytes_to_read;

	bzero(frequencies, DICT_CAPACITY);
	bzero(dict, DICT_CAPACITY);

	start_time = clock();
	in_fd = fopen(input, "rb");
	if(!in_fd) {
		fprintf(stderr, "ERROR: Error opening input file!\n");
		return;
	}

	long pos = ftell(in_fd);
	uint8_t pad = 0;
	fseek(in_fd, -(sizeof(bytes_to_read) + sizeof(pad)), SEEK_END);
	fread(&bytes_to_read, sizeof(size_t), 1, in_fd);
	fread(&pad, sizeof(uint8_t), 1, in_fd);
	pad = 8 - pad;
	fseek(in_fd, pos, SEEK_SET);

	out_fd = fopen(output, "w");
	if(!out_fd) {
		fprintf(stderr, "ERROR: Error opening output file!\n");
		return;
	}

	uint16_t num;
	fread(&num, sizeof(num), 1, in_fd);
	char ch;
	size_t len;
	for(size_t i = 0; i < num; i++) {
		fread(&ch, sizeof(char), 1, in_fd);
		fread(&len, sizeof(size_t), 1, in_fd);
		frequencies[(uint8_t)ch] = len;
	}

	generate_tree();
	assign_codes(root, (char*)"\0", 0);

	node_t* iter = root;
	uint8_t bits_read = 0, byte = 0;
	while(true) {
		if(bits_read == 0){
			fread(&byte, sizeof(uint8_t), 1, in_fd);
			--bytes_to_read;
			if(feof(in_fd) || !bytes_to_read) break;
		}
		if(byte & 0x80)
			iter = iter->right;
		else 
			iter = iter->left;
		byte <<= 1;
		if(leaf(iter)) {
			fprintf(out_fd, "%c", iter->ch);
			iter = root;
		}
		if(++bits_read == 8)
			bits_read = 0;
	}
	if(!bytes_to_read) {
		while(pad--) {
			if(byte & 0x80)
				iter = iter->right;
			else 
				iter = iter->left;
			byte <<= 1;
			if(leaf(iter)) {
				fprintf(out_fd, "%c", iter->ch);
				iter = root;
			}
		}
	}

	end_time = clock();
	DBG("%.2fs for decoding\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	fclose(in_fd);
	fclose(out_fd);
	clean_tree(root);
}

#endif
