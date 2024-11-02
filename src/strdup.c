#include "PROTO.h"
#include "malloc.h"
#include "string.h"

char *
strdup(const char *str)
{
	if (str != NULL) {
		register char *copy = NEW(char, strlen(str) + 1);
		if (copy != NULL) {
			return strcpy(copy, str);
        }
	}
	return NULL;
}
