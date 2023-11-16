#include <stdio.h>

#define DEBUG
#include "../include/huffman.h"
#include "../include/priority_queue.h"

int main(int argc, char** argv){
	if(argc < 3) return -1;
	if(!strcmp(argv[1], "encode")) {
		encode_file(argv[2], argv[3]);
	}
	if(!strcmp(argv[1], "decode")) {
		decode_file(argv[2], argv[3]);
	}
	return 0;
}
