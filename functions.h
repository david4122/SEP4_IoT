/*
 * IncFile1.h
 *
 * Created: 12/14/2020 2:46:30 AM
 *  Author: mmate
 */ 

#pragma once

#include <stdint.h>

void cparr(uint8_t* src, int src_offset, uint8_t* dst, int dst_offset, int len);

void print_arr(char* prefix, uint8_t* arr, int len);
