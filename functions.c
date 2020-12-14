

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
