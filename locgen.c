//
// Created by semin on 12/13/2020.
//

#include "locgen.h"
#include <stdlib.h>
#include <string.h>

struct location {
	char* generatedLocation;
	uint16_t* locationInInt;
};

loc loc_create(void)
{
	struct location *_newLocation = NULL;
	_newLocation = malloc(sizeof(struct location));

	if (_newLocation == NULL)
	{
		return NULL;
	}
	_newLocation -> generatedLocation = NULL;
	_newLocation ->locationInInt = NULL;
	return _newLocation;
}

void generateLocation(loc self)
{
	if(self == NULL)
	{
		return;
	}
	char *loc = malloc(6);
	if (rand() % 2 != 1) {
		strcpy(loc, "Back");
		} else {
		strcpy(loc, "Front");
	}
	self->generatedLocation = loc;
}

void destroyLoc(loc self) {

	if(self == NULL)
	{
		return;
	}
	free(self);
}

char* GetLocation(loc self)
{
	return self -> generatedLocation;
}

uint16_t * getIntArray(loc self)
{
	return self -> locationInInt;
}

void getLocationInInt(loc self)
{
	int i = 0;
	char* location = self ->generatedLocation;
	uint16_t listOfNumbers[6];

	while ('\0' != *location) {
		if(location[i] == 0)
		{
			location[i+1] = 0;
			break;
		}
		listOfNumbers[i] = (uint16_t) location[i];
		i++;
	}
	self -> locationInInt = (uint16_t *) listOfNumbers;
}