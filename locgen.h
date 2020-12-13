#pragma once

#include <stdint.h>

typedef struct location* loc;

loc loc_create(void);
void generateLocation(loc self);
void destroyLoc(loc self);
char* GetLocation(loc self);
void getLocationInInt(loc self);
uint16_t * getIntArray(loc self);