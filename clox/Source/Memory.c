#include <stdlib.h>

#include "Memory.h"

void* Reallocate(void* pointer, size_t oldSize, size_t newSize)
{	
	if (newSize == 0)
	{
		free(pointer);

		int a = 0;

		return NULL;
	}

	void* result = realloc(pointer, newSize);
	if (result == NULL)
	{
		exit(1);
	}

	return result;
}