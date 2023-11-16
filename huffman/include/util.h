#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

uint8_t convert_to_byte(char* bits) {
	uint8_t byte = 0;
	for(size_t i = 0; i < 8; i++){
		if(bits[i] == '1')
			byte |= (0x1 << (7 - i));
	}
	return byte;
}

void convert_to_bits(char* bits, uint8_t byte){
	for(int i = 7; i >= 0; i--){
		bits[7 - i] = '0' + ((0x1 & (byte >> i)));
	}
	return;
}

#endif
