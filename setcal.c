#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int line;
	char **elements;
} set_t;

typedef struct
{
	int line;
	char ***elements;
} rel_t;


// set_t { [ab], [cd] }
// set_t { [ab, cd], [ef, gh] }