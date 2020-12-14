

#include "functions.h"

#include <stdint.h>
#include <stdio.h>

void cparr(uint8_t* src, int src_offset, uint8_t* dst, int dst_offset, int len) {
	for(int i = 0; i < len; i++)
		*(dst + dst_offset + i) = *(src + src_offset + i);
}

void print_arr(char* prefix, uint8_t* arr, int len) {
	printf("%s ", prefix);
	for(int i = 0; i < len; i++) {
		printf("%02X ", arr[i]);
	}
	puts("");
}
