#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ELEM_LEN 30

char *element;

typedef struct
{
	int line;
	char **elem_arr;
	int length;
} set_t;

typedef struct
{
  char *el1;
  char *el2;
} elpair_t;

typedef struct
{
	int line;
	elpair_t **elem_arr;
	int length;
} rel_t;

int set_create(set_t *s)
{
  //alokace cele mnoziny
  s->elem_arr = malloc(s->length * sizeof(char *));
  if (s->elem_arr == NULL)
      return 1;
  //alokace jednotlivych prvku
  for (int i = 0; i < s->length; i++) {
    s->elem_arr[i] = malloc(30 * sizeof(char));
    if (s->elem_arr == NULL)
      return 1;
  }
  return 0;
}

void set_destroy(set_t *s)
{
  for (int i = 0; i < s->length; i++) {
    free(s->elem_arr[i]);
  }
  free(s->elem_arr);
}

/*void set_init(set_t *s)
{
  for (int i = 0; i < s->length; i++) {
    for (int j = 0; j < 30; j++) {

    }
  }
}*/

void set_print(set_t *s)
{
	fprintf(stdout,"Set on line %d contains %d elements which are:", s->length, sizeof(s->elem_arr));
	for(int i = 0; i>s->length; i++){
		for(int j = 0; j<sizeof(s->elem_arr[i]);j++){
			fprintf(stdout," ,%c", s->elem_arr[i][j]);
		}
	}
	fprintf(stdout,"\n");
}

int main()
{
  set_t set;
  set_create(&set);


  set_destroy(&set);


  return EXIT_SUCCESS;
}

// set_t { [ab], [cd] }

// set_t { [ab, cd], [ef, gh] }
