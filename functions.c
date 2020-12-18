

#include "functions.h"

#include <stdint.h>
#include <stdio.h>
#include <avr/pgmspace.h>

void print_arr(const char* prefix, uint8_t* arr, int len) {
	printf_P(PSTR("%s "), prefix);
	for(int i = 0; i < len; i++) {
		printf_P(PSTR("%02X "), arr[i]);
	}
	puts_P(PSTR(""));
}

int freeMem() {
	int size = 8 * 1024;
	uint8_t *b;
	while(size > 0 && (b = malloc(--size)) == NULL);
	free(b);
	return size;
}
