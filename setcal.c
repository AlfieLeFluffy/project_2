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
#include <stdbool.h>

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

//vytiskne memory error
void memory_err()
{
    fprintf(stderr, "Memory error\n");
}

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
            memory_err();
            return 0;
        }
        u->elem_arr = p;
        u->cap++;
    }

    //alokace mista pro dany prvek
    u->elem_arr[u->length] = malloc(str_len * sizeof(char));                          /**!!! str_len +1 !!!  podle nacitani ze souboru...**/
    if( u->elem_arr[u->length] == NULL) {
        memory_err();
        return 0;
    }

    //ulozim novy prvek
    strcpy(u->elem_arr[u->length], elem);

    u->length++;

    return 1;
}

/* funkce pro rozsireni mnoziny  o novy prvek*/
int set_append(set_t *s, int elem)      /** unsigned int ???, vraceni pointeru ??? **/
{
    //pokud je potreba, zvetsim mnozinu
    if (s->cap <= s->length) {
        int *p;
        p = realloc(s->elem_arr, sizeof(int)*(s->cap + 1));
        if(p == NULL) {
            memory_err();
            return 0;
        }
        s->elem_arr = p;
        s->cap++;
    }

    //priradim na nove misto dany prvek
    s->elem_arr[s->length] = elem;
    s->length++;

    return 1;
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

/* funkce pro kontrolu vlozeneho parametru*/

int check_param(int argc, char **argv)
{
    if (argc != 2){
        fprintf(stderr, "Invalid number of args\n");
        return 0;
    }
    if (fopen(argv[1],"r") == NULL){
        fprintf(stderr, "Unable to open file\n");
        return 0;
    }
    return 1;
}

/* funkce pro nacteni souboru a zpracovani dat*/

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


//tiskne true nebo false podle toho, jestli je množina definovaná na řádku A prázdná nebo neprázdná
void set_empty(set_t* s, int line)
{
    for (int l = 0; l < 3; l++)         //l < 3 ???, velikost [sets]... zbytek funguje
    {
        //find set on line [line]
        if (s[l].line == line)
        {
            if (s[l].length == 0)
            {
                fprintf(stdout, "Set on line %d is empty: true\n", line);
                return;
            }

            fprintf(stdout, "Set on line %d is empty: false\n", line);
            return;
        }
    }

    fprintf(stderr, "No set defined on line %d.\n", line);
    return;
}

//tiskne počet prvků v množině A (definované na řádku A)
void set_card(set_t* s, int line)
{
    for (int l = 0; l < 3; l++)         //l < 3 ???, velikost [sets]... zbytek funguje
    {
        //find set on line [line]
        if (s[l].line == line)
        {
            fprintf(stdout, "Set on line %d contains %d elements.\n", line, s[l].length);
            return;
        }
    }

    fprintf(stderr, "No set defined on line %d.\n", line);
    return;
}

//tiskne doplněk množiny A
void set_complement(set_t* s, uni_t* u, int line)
{
    bool temp[u->length];
    for (int i = 0; i < u->length; i++)
    {
        temp[i] = true;
    }
    
    for (int l = 0; l < 3; l++)         //l < 3 ???, velikost [sets]... zbytek funguje
    {
        //find set on line [line]
        if (s[l].line == line)
        {
            for (int i = 0; i < s[l].length; i++)
            {
                temp[s[l].elem_arr[i]] = false;
            }


            fprintf(stdout, "S ");

            for (int i = 0; i < u->length; i++)
            {
                if (temp[i])
                {
                    fprintf(stdout, "%s ", u->elem_arr[i]);
                }
            }

            fprintf(stdout, "\n");

            return;
        }
    }

    fprintf(stderr, "No set defined on line %d.\n", line);
    return;
}

//tiskne sjednocení množin A a B
void set_union();

//tiskne průnik množin A a B
void set_intersect();

//tiskne rozdíl množin A \ B
void set_minus();

//tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B
void set_subseteq();

//tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B
void set_subset();

//tiskne true nebo false, jestli jsou množiny rovny
void equals();


int main(int argc, char **argv)
{
    uni_t uni;
    set_t set;

    set_t sets[3];

    uni_create(&uni);

    uni_append(&uni, "Ahoj", 5);
    uni_append(&uni, "1234", 5);

    set_create(&set, 1); //set on line 1, "Ahoj" "1234"
    set_append(&set, 0);
    set_append(&set, 1);
    sets[0] = set;

    set_print(&set, &uni);


    set_create(&set, 3); //set on line 3, "1234" "1234"
    set_append(&set, 1);
    set_append(&set, 1);
    sets[1] = set;

    set_print(&set, &uni);

    set_create(&set, 4); //set on line 4, empty*/
    sets[2] = set;

    set_print(&set, &uni);

    /*TODO if (append == 1) {
        return EXIT_FAILURE
    }*/

    
    set_complement(sets, &uni, 1);
    set_complement(sets, &uni, 3);
    set_complement(sets, &uni, 4);

    /*
    set_empty(sets, 2);
    set_empty(sets, 1);
    set_empty(sets, 4);

    set_card(sets, 2);
    set_card(sets, 1);
    set_card(sets, 4);
    */

    set_destroy(&set);
    uni_destroy(&uni);

    return EXIT_SUCCESS;
}

// set_t { [ab], [cd] }

// set_t { [ab, cd], [ef, gh] }
