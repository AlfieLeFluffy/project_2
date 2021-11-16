/********************************
 *
 * setcal.c
 *
 * ver 0.1
 * =========================
 *
 * 16.11.2021
 *
 *******************************/

/*TODO chybova hlaseni*/

/** hlavickove soubory **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** definice konstant **/
#define ELEM_LEN 30         //maximalni povolena delka retezce

/** definice novych datovych typu **/
typedef struct{
	int length;
	int cap;
	char **elem_arr;
} uni_t;

typedef struct{
	int line;
	int length;
	int cap;
	int *elem_arr;
} set_t;

typedef struct{
    int e_1;                                    //maly L vypada stejne jak 1 tak jsem to upravila
    int e_2;
} elpair_t;

typedef struct{
	int line;
	int length;         //pocet dvojic
	int cap;
	elpair_t *elem_arr;
} rel_t;


/** definice globalnich funkci **/
/* funkce pro vytvoreni univerza */
void uni_create(uni_t *u)
{
    u->length = 0;
    u->cap = 0;
    u->elem_arr = NULL;
}

/* funkce pro vytvoreni mnoziny */
void set_create(set_t *s, int line)
{
    s->line = line;
    s->length = 0;
    s->cap = 0;
    s->elem_arr = NULL;
}

/* funkce pro vytvoreni relace */
void rel_create(rel_t *r, int line)
{
    r->line = line;
    r->length = 0;
    r->cap = 0;
    r->elem_arr = NULL;
}

/* funkce pro rozsireni univerza  o novy prvek*/
int uni_append(uni_t *u, char *elem, int str_len)
{
    //pokud je potreba, zvetsim pole ukazatelu na prvky univerza
    if (u->cap <= u->length) {
        char **p;
        p = realloc(u->elem_arr, sizeof(char *)*(u->cap + 1));
        if(p == NULL) {
            fprintf(stderr, "Memory error\n");
            return 1;
        }
        u->elem_arr = p;
        u->cap++;
    }

    //alokace mista pro dany prvek
    u->elem_arr[u->length] = malloc(str_len * sizeof(char));                          /**!!! str_len +1 !!!  podle nacitani ze souboru...**/
    if( u->elem_arr[u->length] == NULL) {
        fprintf(stderr, "Memory error\n");
        return 1;
    }

    //ulozim novy prvek
    strcpy(u->elem_arr[u->length], elem);

    u->length++;

    return 0;
}

/* funkce pro rozsireni mnoziny  o novy prvek*/
int set_append(set_t *s, int elem)      /** unsigned int ???, vraceni pointeru ??? **/
{
    //pokud je potreba, zvetsim mnozinu
    if (s->cap <= s->length) {
        int *p;
        p = realloc(s->elem_arr, sizeof(int)*(s->cap + 1));
        if(p == NULL) {
            fprintf(stderr, "Memory error\n");
            return 1;
        }
        s->elem_arr = p;
        s->cap++;
    }

    //priradim na nove misto dany prvek
    s->elem_arr[s->length] = elem;
    s->length++;

    return 0;
}

/* funkce pro uvolneni pameti alokovane pro univerzum */
void uni_destroy(uni_t *u)
{
    for (int i = 0; i < u->length ; i++) {
        if (u->elem_arr[i] != NULL) {
            free(u->elem_arr[i]);
        }
    }

    if (u->elem_arr != NULL) {
        free(u->elem_arr);
    }
    u->length = 0;
    u->cap = 0;
}

/* funkce pro uvolneni pameti alokovane pro mnozinu */
void set_destroy(set_t *s)
{
    if (s->elem_arr != NULL) {
        free(s->elem_arr);
    }
    s->length = 0;
    s->cap = 0;
}

/* funkce pro vraceni mnoziny */
void set_print(set_t *s, uni_t *uni)
{
	fprintf(stdout, "Set on line %d contains %d elements which are: ", s->line, s->length);
	for(int i = 0; i < s->length; i++){
		fprintf(stdout, "%s ", uni->elem_arr[s->elem_arr[i]]);
	}
	fprintf(stdout, "\n");

	/*fprintf(stdout, "Set on line %d contains %d elements which are: ", s->line, s->length);
	for(int i = 0; i < s->length; i++){
		for(int j = 0; s->elem_arr[i][j] != '\0'; j++){
			fprintf(stdout, "%c", s->elem_arr[i][j]);
		}
		fprintf(stdout, " ");
	}
	fprintf(stdout, "\n");*/
}

int main()
{
    uni_t uni;
    set_t set;

    uni_create(&uni);

    uni_append(&uni, "Ahoj", 5);
    uni_append(&uni, "1234", 5);

    set_create(&set, 1);

    set_append(&set, 0);
    set_append(&set, 1);

    /*TODO if (append == 1) {
        return EXIT_FAILURE
    }*/

    set_print(&set, &uni);

    set_destroy(&set);
    uni_destroy(&uni);

    return EXIT_SUCCESS;
}

// set_t { [ab], [cd] }

// set_t { [ab, cd], [ef, gh] }
