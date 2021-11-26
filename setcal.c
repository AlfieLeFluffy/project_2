/********************************
 *
 * setcal.c
 *
 * ver 0.2
 * =========================
 *
 * 23.11.2021
 *
 *******************************/

/** hlavickove soubory **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** definice konstant **/
#define ELEM_LEN 31         //maximalni povolena delka retezce + 1
#define LINES_MAX 1000      //max. podporovany pocet radku v souboru

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
    int e_1;
    int e_2;
} elpair_t;

typedef struct{
	int line;
	int length;         //pocet dvojic
	int cap;
	elpair_t *elem_arr;
} rel_t;

typedef struct{
    set_t **arr_s;      //pole ukazatelu na vsechny mnoziny
    int length_s;
    int cap_s;
    rel_t **arr_r;      //pole ukazatelu na vsechny relace
    int length_r;
    int cap_r;
} data_t;

/** definice globalnich funkci **/
//funkce pri uspechu vraci 1, pri neuspechu 0

/* prints memory error */
void memory_err()
{
    fprintf(stderr, "Memory error\n");
}

/* funkce pro tisk univerza */
void uni_print(uni_t *u)
{
    printf("U ");
    for (int i = 0; i < u->length; i++) {
        printf("%s ", u->elem_arr[i]);
    }
    printf("\n");
}

/* function for printing a set */
void set_print(set_t *s, uni_t *u)
{
	fprintf(stdout, "Set on line %d contains %d elements which are: ", s->line, s->length);

	for(int i = 0; i < s->length; i++){
		fprintf(stdout, "%s ", u->elem_arr[s->elem_arr[i]]);
	}
	fprintf(stdout, "\n");
}

/* function for printing a relation */
void rel_print(rel_t *r, uni_t *u)
{
    fprintf(stdout, "Relation on line %d contains %d elements which are: ", r->line, r->length);

	for(int i = 0; i < r->length; i++){
		fprintf(stdout, "(%s %s) ", u->elem_arr[r->elem_arr[i].e_1], u->elem_arr[r->elem_arr[i].e_2] );
	}
	fprintf(stdout, "\n");
}

/* funkce pro inicializaci univerza */
void uni_create(uni_t *u)
{
    u->length = 0;
    u->cap = 0;
    u->elem_arr = NULL;
}

/* funkce pro inicializaci mnoziny */
void set_create(set_t *s, int line)
{
    s->line = line;
    s->length = 0;
    s->cap = 0;
    s->elem_arr = NULL;
}

/* funkce pro inicializaci relace */
void rel_create(rel_t *r, int line)
{
    r->line = line;
    r->length = 0;
    r->cap = 0;
    r->elem_arr = NULL;
}

/* funkce pro inicializaci poli mnozin a relaci */
void data_create(data_t *d)
{
    d->arr_s = NULL;
    d->length_s = 0;
    d->cap_s = 0;
    d->arr_r = NULL;
    d->length_r = 0;
    d->cap_r = 0;
}

/* funkce pro rozsireni univerza  o novy prvek*/
int uni_append(uni_t *u, char *elem, int str_len)
{
    //kontrola, zda neni retezec prazdny - kvuli konci radku
    if (str_len == 0) {
        return 1;
    }

    //pokud je potreba, zvetsim pole ukazatelu na prvky univerza
    if (u->cap <= u->length) {
        char **p = NULL;
        p = realloc(u->elem_arr, sizeof(char *)*(u->cap + 1));
        if(p == NULL) {
            memory_err();
            return 0;
        }
        u->elem_arr = p;
        u->cap++;
    }

    //alokace mista pro dany prvek
    u->elem_arr[u->length] = malloc((str_len+1) * sizeof(char));
    if( u->elem_arr[u->length] == NULL) {
        memory_err();
        return 0;
    }

    //ulozim novy prvek
    strcpy(u->elem_arr[u->length], elem);

    u->length++;

    return 1;
}

/* funkce pro rozsireni mnoziny o novy prvek */
int set_append(set_t *s, int elem)      /** unsigned int ???, vraceni pointeru ??? **/
{
    //pokud je potreba, zvetsim mnozinu
    if (s->cap <= s->length) {
        int *p = NULL;
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

/* function for adding new pair to relation */
int rel_append(rel_t *r, elpair_t pair)
{
    //if needed, increase relation's capacity
    if (r->cap <= r->length) {
        elpair_t *p = NULL;
        p = realloc(r->elem_arr, sizeof(elpair_t)*(r->cap + 1));
        if(p == NULL) {
            memory_err();
            return 0;
        }
        r->elem_arr = p;
        r->cap++;
    }

    //copy a new pair to relation
    r->elem_arr[r->length].e_1 = pair.e_1;
    r->elem_arr[r->length].e_2 = pair.e_2;
    r->length++;

    return 1;
}

/* function for adding a new set to data */
int data_append_s(data_t *d, set_t *s)
{
    //if needed, allocate more memory
    if (d->cap_s <= d->length_s) {
        set_t **p = NULL;
        p = realloc(d->arr_s, sizeof(set_t *)*(d->cap_s + 1));
        if(p == NULL) {
            memory_err();
            return 0;
        }
        d->arr_s = p;
        (d->cap_s)++;
    }

    //copy a pointer to set to data
    d->arr_s[d->length_s] = s;
    d->length_s++;

    return 1;
}

/* function for adding a new relation to data */
int data_append_r(data_t *d, rel_t *r)
{
    //if needed, allocate more memory
    if (d->cap_r <= d->length_r) {
        rel_t **p = NULL;
        p = realloc(d->arr_r, sizeof(rel_t *)*(d->cap_r + 1));
        if(p == NULL) {
            memory_err();
            return 0;
        }
        d->arr_r = p;
        (d->cap_r)++;
    }

    //copy a pointer to relation to data
    d->arr_r[d->length_r] = r;
    d->length_r++;

    return 1;
}

/* funkce pro uvolneni pameti alokovane pro univerzum */
void uni_destroy(uni_t *u)
{
    for (int i = 0; i < u->cap ; i++) {
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

/* funkce pro uvolneni pameti alokovane pro relaci */
void rel_destroy(rel_t *r)
{
    if (r->elem_arr != NULL) {
        free(r->elem_arr);
    }
    r->length = 0;
    r->cap = 0;
}

/* funkce pro uvolneni pameti alokovane pro data */
void data_destroy(data_t *d)
{
    //dealokace mnozin

    if (d->arr_s != NULL) {
        for (int i; i < d->cap_s; i++) {        //conditional jump move based on uninitialized valuse
            set_destroy(d->arr_s[i]);
            free(d->arr_s[i]);
        }

        free(d->arr_s);
    }

    //dealokace relaci
    if (d->arr_r != NULL) {
        for (int i; i < d->cap_r; i++) {        //conditional jump move based on uninitialized valuse
            rel_destroy(d->arr_r[i]);
            free(d->arr_r[i]);
        }

        free(d->arr_r);
    }
}

/* funkce pro kontrolu vlozeneho parametru*/
int check_param(int argc)
{
    if (argc != 2){
        fprintf(stderr, "Invalid number of args\n");
        return 0;
    }
    return 1;
}

/* funkce pro otevreni souboru */
/*FILE *file_open(char **argv)                     //neni potreba nejspis, usetrila by tak jeden radek v text_load()
{
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "Unable to open file\n");
        return NULL;
    }
    return fp;
}*/

/* funkce pro kontrolu, zda je znak podporovan */
int check_char(char c)
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ){
        return 1;
    }

    fprintf(stderr, "Char '%c' is not supported\n", c);
    return 0;
}

/* prodlouzi dany string o 1 znak */
void str_append(char str[], char c, int *len)
{
    //len je delka retezce, tedy udava pozici '\0'
    str[*len] = c;
    str[*len + 1] = '\0';
    (*len)++;
}

/* function to skip all spaces, returns next char after spaces */
char skip_space(FILE *fp)
{
    char c;

    while ( (c = fgetc(fp)) == ' ') {
        continue;
    }

    return c;
}

/* funkce pro nacteni stringu ze souboru */
char load_str(FILE *fp, char str[], int *len)
{
    //fce vraci pri uspechu znak, ktery bezprostredne nasleduje po danem retezci (mezera nebo \n)

    char c;
    *len = 0;  //delka daneho retezce ... potreba pro nacitani do univerza
    str[0] = '\0';

    c = skip_space(fp);

    //konec radku odpovida konci dane mnoziny apod. => navrat
    if (c == '\n') {
        return c;
    }

    do {
        //pokud c neni znak abecedy, vracim 0
        if (check_char(c) == 0){
            return 0;
        }
        str_append(str, c, len);      //prvni prubeh pouziva prvni znak po odstraneni mezer

        c = fgetc(fp);       //ziskani noveho znaku

        //kontrola konce retezce
        if ( c == ' ' || c == '\n') {
            return c;
        }
    } while (*len < ELEM_LEN - 1);

    //odchod mimo while pouze pokud dojde k prekroceni max. delky prvku
    fprintf(stderr, "Element (%s...) exceeds length of %d\n", str, ELEM_LEN - 1);
    return 0;
}

/* funkce pro nacteni prvku radku do univerza */ ///TODO is_keyword(char str[])
int load_uni(FILE *fp, uni_t *u)
{
    //inicializuji univerzum
    uni_create(u);

    char temp_s[ELEM_LEN];
    char c;
    int len;

    do {
        //abychom mohli kontrolovat jak '\n' tak 0, musime si
        //navratovou hodnotu funkce load_str ulozit do promenne c
        c = load_str(fp, temp_s, &len);

        //pokud funkce load_str vrati 0 (=chyba), vratime 0
        if (c == 0){
            return 0;
        }

        ///is_keyword(temp_s);

        //kontrola, zda uz neni prvek v univerzu
        for (int i = 0; i < u->length; i++) {
            if (strcmp(temp_s, u->elem_arr[i]) == 0) {
                fprintf(stderr, "Element %s duplicate in universe\n", temp_s);
                return 0;
            }
        }

        //prodlouzime univerzum o nove nacteny string a opakujeme
        if (uni_append(u, temp_s, len) == 0) {
            return 0;
        }
    } while (c != '\n'); //pokud c je \n, jsme na konci radku -> konec nacitani

    //jinak uspech
    return 1;
}

/* funkce pro pridani univerza jako mnoziny */
int u_to_s(uni_t *u, data_t *d, int line)
{
    set_t *s = malloc(sizeof(set_t));
    if (s == NULL) {
        memory_err();
        return 0;
    }
    set_create(s, line);
    for (int i = 0; i < u->length; i++){
        //naplnime mnozinu vsemi indexy univerza
        if (set_append(s, i) == 0){
            return 0;
        }
    }
    //ulozime mnozinu do seznamu
    if (data_append_s(d, s) == 0){
            return 0;
    }

    return 1;
}

/* funkce pro zjisteni, zda se dany prvek vyskytuje v mnozine */
int isin_set(set_t s,int elem)
{
    for (int i = 0; i < s.length; i++) {
        if (s.elem_arr[i] == elem) {
            return 1;
        }
    }
    return 0;
}

/* function checking if string is in universe */
int isin_uni(uni_t *u, char s[], int line)
{
    //returns -1 when no elem matches, to avoid conflict with index 0

    for (int i = 0; i < u->length; i++) {
        if (strcmp(s, u->elem_arr[i]) == 0) {
            return i;
        }
    }

    fprintf(stderr, "Element %s not in universe (line %d)\n", s, line);
    return -1;
}

/* funkce pro nacteni jednoho prvku do mnoziny */
int elem_to_s(uni_t *u, set_t *s, char temp_s[], int line)
{
    //kontrola, zda neni retezec prazdny - kvuli ukonceni radku
    if (temp_s[0] == '\0') {
        return 1;
    }

    int ind;  //index


    if ((ind = isin_uni(u, temp_s, line)) == -1){
        return 0;
    }
    if (isin_set(*s, ind) == 1) {
        fprintf(stderr, "Element %s duplicate on line %d\n", temp_s, line);
        return 0;
    }

    //kontrola, zda se povedlo prodlouzeni mnoziny
    if (set_append(s, ind) == 0) {
        return 0;
    }

    //jinak uspech
    return 1;
}

/* function for loading all elements in a line to set */
int load_set(FILE *fp, data_t *d, uni_t *u, int line)
{
    //allocation of set
    set_t *s = malloc(sizeof(set_t));
    if (s == NULL) {
        memory_err();
        return 0;
    }

    char temp_s[ELEM_LEN];
    char c;
    int len;    //length of string, used by load_uni => needed as a parameter of load_str

    //inicialization of set
    set_create(s, line);

    do {
        c = load_str(fp, temp_s, &len);

        //if load_str returns 0 (=error), return 0
        if (c == 0){
            return 0;
        }

        //load element to set
        if (elem_to_s(u, s, temp_s, line) == 0) {
            return 0;
        }

    } while( c != '\n');

    //add set to data
    if (data_append_s(d, s) == 0) {
        return 0;
    }
    //else success
    return 1;
}

/* function for loading the 2nd string of a relation */     ///TODO rozdelit
char load_str_r(FILE *fp, char str1[], char str2[], int line)
{
    //when successful, function returns next character after pair (space or \n)
    char c;
    str1[0] = '\0';
    str2[0] = '\0';
    int len = 0;

    c = skip_space(fp);

    //return successfully if end of relation is loaded
    if (c == '\n') {
        return c;
    }

    if (c != '(') {
        fprintf(stderr, "Invalid format of relation on line %d\n", line);
        return 0;
    }

    c = fgetc(fp);       //get new character

    do {

        if ( c == '\n') {
            fprintf(stderr, "Unfinished pair in relation on line %d\n", line);
            return 0;
        }
        //if c is not letter of the alphabet, return 0
        if (check_char(c) == 0){
            return 0;
        }

        str_append(str1, c, &len);          //add character to string

        if (len == ELEM_LEN - 1) {
            fprintf(stderr, "Element (%s...) exceeds length of %d\n", str1, ELEM_LEN - 1);
            return 0;
        }
    } while ((c = fgetc(fp)) != ' ');

    if (len == 0) {
        fprintf(stderr, "Invalid format of relation on line %d\n", line);
        return 0;
    }

    len = 0;
    c = skip_space(fp);

    do {
        if (check_char(c) == 0){
            return 0;
        }
        str_append(str2, c, &len);      //add character to string

        c = fgetc(fp);       //get new character

        //
        if ( c == '\n') {
            fprintf(stderr, "Unfinished pair in relation on line %d\n", line);
            return 0;
        }
        if ( c == ' ') {
            fprintf(stderr, "Invalid format of relation on line %d\n", line);
            return 0;
        }

        if (len == ELEM_LEN - 1) {
            fprintf(stderr, "Element (%s...) exceeds length of %d\n", str1, ELEM_LEN - 1);
            return 0;
        }
    } while (c != ')');

    if (len == 0) {
        fprintf(stderr, "Invalid format of relation on line %d\n", line);
        return 0;
    }

    //success
    return 1;
}

/* function checking if pair is in relation */
int isin_rel(rel_t *r, elpair_t p)
{
    for (int i = 0; i < r->length; i++){
        if ((p.e_1 == r->elem_arr[i].e_1) && (p.e_2 == r->elem_arr[i].e_2)){
            return 1;
        }
    }
    return 0;
}

/* function for assigning pair to relation */
int pair_to_r(uni_t *u, rel_t *r, char temp_s1[], char temp_s2[], int line)
{
    //checking if strings aren't empty because of '\n'
    if (temp_s1[0] == '\0' && temp_s2[0] == '\0') {
        return 1;
    }
    elpair_t p;

    //is pair from universe?
    p.e_1 = isin_uni(u, temp_s1, line);
    p.e_2 = isin_uni(u, temp_s2, line);
    if (p.e_1 == -1 || p.e_2 == -1) {
        return 0;
    }

    //check, if pair is already in relation
    if (isin_rel(r, p) == 1) {
        fprintf(stderr, "Pair (%s %s) duplicate on line %d\n", temp_s1, temp_s2, line);
        return 0;
    }

    //check, if appending the relation finished correctly
    if (rel_append(r, p) == 0) {
        return 0;
    }

    //else success
    return 1;
}

/* function for loading all elements in a line to relation */ ///TODO prvek prave 1
int load_rel(FILE *fp, data_t *d, uni_t *u, int line)
{
    //allocation of relation
    rel_t *r = malloc(sizeof(rel_t));
    if (r == NULL) {
        memory_err();
        return 0;
    }
    char temp_s1[ELEM_LEN];
    char temp_s2[ELEM_LEN];
    char c;

    //inicialization of relation
    rel_create(r, line);

    do {
        //check, if load_str() finished successfully
        if ((c = load_str_r(fp, temp_s1, temp_s2, line)) == 0) {
            return 0;
        }

        //load pair to relation
        if ((pair_to_r(u, r, temp_s1, temp_s2, line)) == 0) {
            return 0;
        }
    } while( c != '\n');

    //add relation to data
    if (data_append_r(d, r) == 0) {
        return 0;
    }
    //else success
    return 1;
}

/* funkce pro nacteni prikazu ze souboru */
/*int load_com(FILE *fp, uni_t *u)
{
    return 1;
}*/

int text_load(char **argv, data_t *d, uni_t *u)
{
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "Unable to open file\n");
        return 0;
    }
    //cti soubor po radcich
    for (int lines = 1; lines <= LINES_MAX; lines++) {
        //zjistim, o jaky typ radku se jedna
        switch(fgetc(fp)) {
            case 'U':
                if (lines != 1) {
                    fprintf(stderr, "Universe on an unexpected line (line %d)\n", lines);
                    return 0;
                }
                if (load_uni(fp, u) == 0){
                    return 0;
                }
                if (u_to_s(u, d, lines) == 0){
                    return 0;
                }

                continue;

            case 'S':
                if (lines == 1) {
                    fprintf(stderr, "Universe expected on line 1 instead of set\n");
                    return 0;
                }
                if (load_set(fp, d, u, lines) == 0) {
                    return 0;
                }

                continue;

            case 'R':
                if (lines == 1) {
                    fprintf(stderr, "Universe expected on line 1 instead of relation\n");
                    return 0;
                }
                if (load_rel(fp, d, u, lines) == 0) {
                    return 0;
                }
                continue;

            case 'C':
                if (lines == 1) {
                    fprintf(stderr, "Universe expected on line 1 instead of command\n");
                    return 0;
                }
                //load_com(fp, u);
                continue;

            case EOF:
                fclose(fp);
                return 1;

            default:
                fprintf(stderr, "Unidentified line number %d\n", lines);
                return 0;
        }
    }

    /*if (lines > LINES_MAX){ ??? } */

    fclose(fp);
    return 0;
}




//prints chosen universe elements
void bool_print(uni_t* u, bool* b)
{
    fprintf(stdout, "S ");

    for (int i = 0; i < u->length; i++)
    {
        if (b[i])
        {
            fprintf(stdout, "%s ", u->elem_arr[i]);
        }
    }

    fprintf(stdout, "\n");

    return;
}


/*
    Commands for sets
*/


//find set defined on line [line]
int set_line(data_t* data, int line)
{
    for (int i = 0; i < data->length_s && i < line; i++)
    {
        //set on line [line] found
        if (data->arr_s[i]->line == line)
        {
            return i;
        }
    }

    //set not found
    fprintf(stderr, "No set defined on line %d.\n", line);
    return -1;
}

//returns whether or not is set on line [line_a] a subset of set on line [line_b]
bool set_sub(data_t* data, uni_t* u, int l_a, int l_b)
{
    bool set_b[u->length];   //elements of set on line [line_b]
    for (int i = 0; i < u->length; i++)
    {
        set_b[i] = false;
    }

    for (int i = 0; i < data->arr_s[l_b]->length; i++)
    {
        set_b[data->arr_s[l_b]->elem_arr[i]] = true;
    }

    //check set on line [line_a]
    for (int i = 0; i < data->arr_s[l_a]->length; i++)
    {
        //extra element not contained in set on line [line_b] found
        if (!set_b[data->arr_s[l_a]->elem_arr[i]])
        {
            return false;
        }
    }

    //no extra elements found
    return true;
}


//prints whether or not is set on line [line] empty
void set_empty(data_t* data, int line)
{
    int l = set_line(data, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    //set has length 0 - is empty
    if (data->arr_s[l]->length == 0)
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set has length greater than 0
    fprintf(stdout, "false\n");
    return;
}

//prints number of elements in set on line [line]
void set_card(data_t* data, int line)
{
    int l = set_line(data, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    fprintf(stdout, "Set on line %d contains %d elements.\n", line, data->arr_s[l]->length);
    return;
}

//prints complement of set on line [line]
void set_complement(data_t* data, uni_t* uni, int line)
{
    int l = set_line(data, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    bool set_comp[uni->length];   //complement of set on line [line]
    for (int i = 0; i < uni->length; i++)
    {
        set_comp[i] = true;
    }

    //elements contained in set on line [line_a]
    for (int i = 0; i < data->arr_s[l]->length; i++)
    {
        set_comp[data->arr_s[l]->elem_arr[i]] = false;
    }

    bool_print(uni, set_comp);
    return;
}

//prints union of sets on lines [line_a] and [line_b]
void set_union(data_t* data, uni_t* uni, int line_a, int line_b)
{
    int l_a = set_line(data, line_a);  //index of set on line [line_a]
    int l_b = set_line(data, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    bool set_uni[uni->length];   //union of sets on lines [line_a] and [line_b]
    for (int i = 0; i < uni->length; i++)
    {
        set_uni[i] = false;
    }

    //elements contained in sets on line [l] - [line_a] and [line_b]
    int l = l_a;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < data->arr_s[l]->length; j++)
        {
            set_uni[data->arr_s[l]->elem_arr[j]] = true;
        }

        l = l_b;
    }


    bool_print(uni, set_uni);
    return;
}

//prints intersect of sets on lines [line_a] and [line_b]
void set_intersect(data_t* data, uni_t* uni, int line_a, int line_b)
{
    int l_a = set_line(data, line_a);  //index of set on line [line_a]
    int l_b = set_line(data, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    bool set_int[uni->length];   //intersect of sets on lines [line_a] and [line_b]
    for (int i = 0; i < uni->length; i++)
    {
        set_int[i] = false;
    }

    //elements contained in both sets on lines [line_a] and [line_b]
    for (int i = 0; i < data->arr_s[l_a]->length; i++)
    {
        for (int j = 0; j < data->arr_s[l_b]->length; j++)
        {
            if (data->arr_s[l_a]->elem_arr[i] == data->arr_s[l_b]->elem_arr[j])
            {
                set_int[data->arr_s[l_a]->elem_arr[i]] = true;
                break;
            }
        }
    }


    bool_print(uni, set_int);
    return;
}

//prints set on line [line_a] minus set on line [line_b]
void set_minus(data_t* data, uni_t* uni, int line_a, int line_b)
{
    int l_a = set_line(data, line_a);  //index of set on line [line_a]
    int l_b = set_line(data, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    bool set_min[uni->length];   //set on line [line_a] minus set on line [line_b]
    for (int i = 0; i < uni->length; i++)
    {
        set_min[i] = false;
    }

    //elements contained in set on line [line_a]
    for (int i = 0; i < data->arr_s[l_a]->length; i++)
    {
        set_min[data->arr_s[l_a]->elem_arr[i]] = true;
    }

    //elements contained in set on line [line_b]
    for (int i = 0; i < data->arr_s[l_b]->length; i++)
    {
        set_min[data->arr_s[l_b]->elem_arr[i]] = false;
    }

    bool_print(uni, set_min);
    return;
}

//prints whether or not is set on line [line_a] a subset of set on line [line_b]
void set_subseteq(data_t* data, uni_t* uni, int line_a, int line_b)
{
    int l_a = set_line(data, line_a);  //index of set on line [line_a]
    int l_b = set_line(data, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    //set on line [line_a] is a subset of set on line [line_b]
    if (set_sub(data, uni, l_a, l_b))
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    fprintf(stdout, "false\n");
    return;
}

//prints whether or not is set on line [line_a] a proper subset of set on line [line_b]
void set_subset(data_t* data, uni_t* uni, int line_a, int line_b)
{
    int l_a = set_line(data, line_a);  //index of set on line [line_a]
    int l_b = set_line(data, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    //set on line [line_a] is a subset of set on line [line_b]
    ///and set on line [line_b] isn't a subset of set on line [line_a]
    if (set_sub(data, uni, l_a, l_b) && !set_sub(data, uni, l_b, l_a))
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    //and/or set on line [line_b] is a subset of set on line [line_a]
    fprintf(stdout, "false\n");
    return;
}

//prints whether or not are sets on lines [line_a] and [line_b] equal
void set_equals(data_t* data, uni_t* uni, int line_a, int line_b)
{
    int l_a = set_line(data, line_a);  //index of set on line [line_a]
    int l_b = set_line(data, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    //set on line [line_a] is a subset of set on line [line_b]
    ///and set on line [line_b] is a subset of set on line [line_a]
    if (set_sub(data, uni, l_a, l_b) && set_sub(data, uni, l_b, l_a))
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    ///and/or set on line [line_b] isn't a subset of set on line [line_a]
    fprintf(stdout, "false\n");
    return;
}


/*
    Commands for relations
*/


//find relation defined on line [line]
int rel_line(data_t* data, int line)
{
    for (int i = 0; i < data->length_r && i < line; i++)
    {
        //relation on line [line] found
        if (data->arr_r[i]->line == line)
        {
            return i;
        }
    }

    //relation not found
    fprintf(stderr, "No relation defined on line %d.\n", line);
    return -1;
}

//returns the domain of relation with index [l]
void rel_domainf(data_t* data, uni_t* uni, int l, bool* rel)
{
    bool rel_dom[uni->length];   //domain of relation with index [l]
    for (int i = 0; i < uni->length; i++)
    {
        rel_dom[i] = false;
    }

    //first elements from relation on line [line]
    for (int i = 0; i < data->arr_r[l]->length; i++)
    {
        rel_dom[data->arr_r[l]->elem_arr[i].e_1] = true;
    }

    *rel = rel_dom;
    return;
}

//returns the codomain of relation with index [l]
void rel_codomainf(data_t* data, uni_t* uni, int l, bool* rel)
{
    bool rel_cod[uni->length];   //codomain of relation with index [l]
    for (int i = 0; i < uni->length; i++)
    {
        rel_cod[i] = false;
    }

    //first elements from relation on line [line]
    for (int i = 0; i < data->arr_r[l]->length; i++)
    {
        rel_cod[data->arr_r[l]->elem_arr[i].e_2] = true;
    }

    *rel = rel_cod;
    return;
}




//prints whether or not is the relation on line [line] reflexive, ???
void rel_reflexive(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    bool* rel_dom;
    rel_domainf(data, uni, l, rel_dom);

    bool* rel_cod;
    rel_codomainf(data, uni, l, rel_cod);

    bool rel_ref[uni->length];   //reflexive elements in relation on line [line]
    for (int i = 0; i < uni->length; i++)
    {
        rel_ref[i] = false;
    }

    for (int i = 0; i < data->arr_r[l]->length; i++)
    {
        if (data->arr_r[l]->elem_arr[i].e_1 == data->arr_r[l]->elem_arr[i].e_2)
        {
            rel_ref[data->arr_r[l]->elem_arr[i].e_1] = true;
        }
    }

    for (int i = 0; i < uni->length; i++)
    {
        if ((rel_dom[i] || rel_cod) && !rel_ref[i])
        {
            fprintf(stdout, "false\n");
            return;
        }
    }

    fprintf(stdout, "true\n");
    return;
}

//prints whether or not is the relation on line [line] symmetric, TO DO
void rel_symmetric(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}

//prints whether or not is the relation on line [line] antisymmetric, TO DO
void rel_antisymmetric(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}

//prints whether or not is the relation on line [line] transitive, TO DO
void rel_transitive(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}

//prints whether or not is the relation on line [line] a function, TO DO
void rel_function(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}

//prints the domain of the function on line [line]
void rel_domain(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    bool* rel_dom;

    /*bool rel_dom[uni->length];   //domain of relation on line [line]
    for (int i = 0; i < uni->length; i++)
    {
        rel_dom[i] = false;
    }

    //first elements from relation on line [line]
    for (int i = 0; i < data->arr_r[l]->length; i++)
    {
        rel_dom[data->arr_r[l]->elem_arr[i].e_1] = true;
    }*/

    rel_domainf(data, uni, l, rel_dom);
    bool_print(uni, rel_dom);
    return;
}

//prints the codomain of the function on line [line]
void rel_codomain(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    bool* rel_cod;  //codomain of relation on line [line]

    /*bool rel_cod[uni->length];   //codomain of relation on line [line]
    for (int i = 0; i < uni->length; i++)
    {
        rel_cod[i] = false;
    }

    //second elements from relation on line [line]
    for (int i = 0; i < data->arr_r[l]->length; i++)
    {
        rel_cod[data->arr_r[l]->elem_arr[i].e_2] = true;
    }*/

    rel_codomainf(data, uni, l, rel_cod);
    bool_print(uni, rel_cod);
    return;
}

//prints whether or not is the relation on line [line] injective, TO DO
void rel_injective(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}

//prints whether or not is the relation on line [line] surjective, TO DO
void rel_surjective(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}

//prints whether or not is the relation on line [line] bijective, TO DO
void rel_bijective(data_t* data, uni_t* uni, int line)
{
    int l = rel_line(data, line);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }

    //TO DO
}


void test_print(uni_t uni, data_t data)
{
    printf("=== Kontrola cteni ze souboru ===\n");
    uni_print(&uni);
    for(int i=0;i<data.length_s; i++){
        set_print(data.arr_s[i], &uni);
    }
    for(int i=0;i<data.length_r; i++){
        rel_print(data.arr_r[i], &uni);
    }
    printf("=== Kontrola cteni ze souboru ===\n\n");
}


int main(int argc, char **argv)
{
    if(check_param(argc) == 0){
        return EXIT_FAILURE;
    }

    uni_t uni;
    data_t data;

    data_create(&data);

    if (text_load(argv,&data, &uni) == 0) {
        return EXIT_FAILURE;
    }

    test_print(uni, data);


    set_empty(&data, 4);
    set_card(&data, 2);
    set_complement(&data, &uni, 1);
    set_union(&data, &uni, 2, 3);
    set_intersect(&data, &uni, 1, 3);
    set_minus(&data, &uni, 2, 3);
    set_subseteq(&data, &uni, 2, 3);
    set_subset(&data, &uni, 3, 2);
    set_equals(&data, &uni, 2, 3);


/*
    rel_reflexive(&data, &uni, 4);  //???
    rel_symmetric(&data, &uni, 4);  //TO DO
    rel_antisymmetric(&data, &uni, 4);  //TO DO
    rel_transitive(&data, &uni, 4); //TO DO
    rel_function(&data, &uni, 4);   //TO DO
    rel_domain(&data, &uni, 4);
    rel_codomain(&data, &uni, 4);
    rel_injective(&data, &uni, 4);  //TO DO
    rel_surjective(&data, &uni, 4); //TO DO
    rel_bijective(&data, &uni, 4);  //TO DO
*/


    data_destroy(&data);
    uni_destroy(&uni);

    return EXIT_SUCCESS;
}
