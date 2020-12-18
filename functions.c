

#include "functions.h"

#include <stdint.h>
#include <stdio.h>

void print_arr(char* prefix, uint8_t* arr, int len) {
	printf("%s ", prefix);
	for(int i = 0; i < len; i++) {
		printf("%02X ", arr[i]);
	}
	puts("");
}

int freeMem() {
	int size = 8 * 1024;
	uint8_t *b;
	while(size > 0 && (b = malloc(--size)) == NULL);
	free(b);
	return size;
}
