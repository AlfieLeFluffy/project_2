/********************************
 *
 * setcal.c
 *
 * ver 0.1
 * =========================
 *
 * 14.11.2021
 *
 *******************************/

/** hlavickove soubory **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** definice konstant **/
#define ELEM_LEN 30

/** definice globalnich funkci **/
typedef struct{
	int line;
	char **elem_arr;
	int length;
} set_t;

typedef struct{
    char *el1;
    char *el2;
} elpair_t;

typedef struct{
	int line;
	elpair_t **elem_arr;
	int length;
} rel_t;

/* funkce pro alokaci mnoziny */        /**TODO chybove hlaseni?**/
int set_create(set_t *s)
{
    //alokace cele mnoziny
    s->elem_arr = malloc(s->length * sizeof(char *));
    if (s->elem_arr == NULL) {
        return 1;
    }

    //alokace jednotlivych prvku mnoziny
    for (int i = 0; i < s->length; i++) {
        s->elem_arr[i] = malloc(ELEM_LEN * sizeof(char));
        if (s->elem_arr == NULL) {
            return 1;
        }
    }

    return 0;
}

/* funkce pro uvolneni pameti alokovane pro mnozinu */ /**snad funguje..?**/
void set_destroy(set_t *s)
{
    free(s->elem_arr);
}

/* funkce pro vraceni mnoziny */
void set_print(set_t *s)
{
	fprintf(stdout, "Set on line %d contains %d elements which are: ", s->line, s->length);
	for(int i = 0; i < s->length; i++){
		for(int j = 0; s->elem_arr[i][j] != '\0'; j++){
			fprintf(stdout, "%c", s->elem_arr[i][j]);
		}
		fprintf(stdout, " ");
	}
	fprintf(stdout, "\n");
}

int main()
{
    set_t set;
    if(set_create(&set) == 1)
        return EXIT_FAILURE;

    set.length=3;
    set.elem_arr[0] = "sdds";
    set.elem_arr[1] = "lfyi";
    set.elem_arr[2] = "5564";

    set_print(&set);

    set_destroy(&set);

    return EXIT_SUCCESS;
}

// set_t { [ab], [cd] }

// set_t { [ab, cd], [ef, gh] }
