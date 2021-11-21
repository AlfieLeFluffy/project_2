/********************************
 *
 * setcal.c
 *
 * ver 0.1
 * =========================
 *
 * 20.11.2021
 *
 *******************************/

/*TODO chybova hlaseni*/

/** hlavickove soubory **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** definice konstant **/
#define ELEM_LEN 31         //maximalni povolena delka retezce + 1
#define LINES_MAX 1000      //max podporovany pocet radku v souboru

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
    int cap_r;          ///TODO capacity na oba
} data_t;

/** definice globalnich funkci **/
//funkce pri uspechu vraci 1, pri neuspechu 0

//vytiskne memory error
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

/* funkce pro tisk mnoziny */
void set_print(set_t *s, uni_t *u)
{
	fprintf(stdout, "Set on line %d contains %d elements which are: ", s->line, s->length);

	for(int i = 0; i < s->length; i++){
		fprintf(stdout, "%s ", u->elem_arr[s->elem_arr[i]]);
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

/* funkce pro rozsireni dat o novou mnozinu */
int data_append_s(data_t *d, set_t *s)
{
    //pokud je potreba, alokuji vice pameti
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

    //priradim na nove misto dany uakzatel na mnozinu
    d->arr_s[d->length_s] = s;
    d->length_s++;

    return 1;
}

/* funkce pro rozsireni dat o novou relaci */
int data_append_r(data_t *d, rel_t *r)
{

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
    /*if (s->elem_arr != NULL) {
        free(s->elem_arr);
    }
    s->length = 0;
    s->cap = 0;*/
}

/* funkce pro uvolneni pameti alokovane pro data */
void data_destroy(data_t *d)
{
    //dealokace mnozin
    if (d->arr_s != NULL) {
        for (int i; i < d->cap_s; i++) {
            set_destroy(d->arr_s[i]);
        }

        free(d->arr_s);
    }

    //dealokace relaci
    if (d->arr_r != NULL) {
        for (int i; i < d->cap_r; i++) {
            rel_destroy(d->arr_r[i]);
        }

        free(d->arr_r);
    }
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

/* funkce pro otevreni souboru */
FILE *file_open(char **argv)                      ///TODO zavrit soubor fp
{
    FILE *fp = fopen(argv[1], "r");
    return fp;
}

/* funkce pro kontrolu, zda je znak podporovan */
int check_char(char c)
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ){
        return 1;
    }
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

/* funkce pro nacteni stringu ze souboru */
char load_str(FILE *fp, char str[], int *len)
{
    //fce vraci pri uspechu znak, ktery bezprostredne nasleduje po danem retezci (mezera nebo \n)
    char c;
    *len = 0;  //delka daneho retezce ... potreba pro nacitani do univerza
    str[0] = '\0';

    //preskoceni vsech mezer
    while ( (c = fgetc(fp)) == ' ') {                                       ///skip_space()??
        continue;
    }

    //konec radku odpovida konci dane mnoziny apod. => navrat
    if (c == '\n') {
        return c;
    }

    do {
        //pokud c neni znak abecedy, vracim 0
        if (check_char(c) == 0){
            fprintf(stderr, "Char %c is not supported\n", c);
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

        ///is_keyword(temp_s);

        //pokud funkce load_str vrati 0 (=chyba), vratime 0
        if (c == 0){
            return 0;
        }
        //prodlouzime univerzum o nove nacteny string a opakujeme
        if (uni_append(u, temp_s, len) == 0) {
            return 0;
        }
    } while (c != '\n'); //pokud c je \n, jsme na konci radku -> konec nacitani

    //jinak uspech
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

/* funkce pro nacteni jednoho prvku do mnoziny */
int elem_to_s(uni_t *u, set_t *s, char temp_s[], int line)
{
    bool found;
    found = false;

    for (int i = 0; i < u->length; i++) {
        //kontrola, zda nalezi dany prvek univerzu
        if (strcmp(temp_s, u->elem_arr[i]) == 0) {
            //kontrola, zda uz dany prvek v mnozine neni
            if (isin_set(*s, i) == 1) {
                fprintf(stderr, "Element %s duplicate on line %d\n", temp_s, line);
                return 0;
            }

            //kontrola, zda se povedlo prodlouzeni mnoziny
            if (set_append(s, i) == 0) {
                return 0;
            }

            //pokud retezec projde kontrolami, je korektni
            found = true;
            break;
        }
    }

    if (!found && s->length != 0) {
        fprintf(stderr, "Element %s not in universe (line %d)\n", temp_s, line);
        return 0;
    }

    //jinak uspech
    return 1;
}

/* funkce pro nacteni prvku radku do mnoziny */
int load_set(FILE *fp, data_t *d, uni_t *u, int line)
{
    set_t *s = malloc(sizeof(set_t *));
    char temp_s[ELEM_LEN];
    char c;
    int len;    //delka daneho retezce, vyuziva load_uni => potreba jako paramentr funkce load_str

    set_create(s, line);

    do {
        c = load_str(fp, temp_s, &len);

        //pokud funkce load_str vrati 0 (=chyba), vratime 0
        if (c == 0){
            return 0;
        }

        //nacteni prvku do mnoziny
        if (elem_to_s(u, s, temp_s, line) == 0) {
            return 0;
        }

    } while( c != '\n');

    //pridani mnoziny do dat
    if (data_append_s(d, s) == 0) {
        return 0;
    }
    //jinak uspech
    return 1;
}

/* funkce pro nacteni prvku radku do relace */ ///TODO prvek prave 1
int load_rel(FILE *fp, uni_t *u)
{
    return 1;
}

/* funkce pro nacteni prikazu ze souboru */
int load_com(FILE *fp, uni_t *u)
{
    return 1;
}

int text_load(FILE *fp, data_t *d, uni_t *u)
{
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
                load_rel(fp, u);
                continue;

            case 'C':
                if (lines == 1) {
                    fprintf(stderr, "Universe expected on line 1 instead of command\n");
                    return 0;
                }
                load_com(fp, u);
                continue;

            case EOF:
                return 1;

            default:
                fprintf(stderr, "Unidentified line number %d\n", lines);
                return 0;
        }
    }

    /*if (lines > LINES_MAX){ ??? } */

    return 1;
}




//find set defined on line [line]
int set_line(set_t* s, int line)
{
    for (int i = 0; i < 3 && i < line; i++)             //l < 3 ???, velikost sets... zbytek funguje
    {
        //set on line [line] found
        if (s[i].line == line)
        {
            return i;
        }
    }

    //set not found
    fprintf(stderr, "No set defined on line %d.\n", line);
    return -1;
}

//prints chosen universe elements
void set_boolprint(char c, uni_t* u, bool* b)
{
    fprintf(stdout, "%c ", c);

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

//returns whether or not is set on line [line_a] a subset of set on line [line_b]
bool set_sub(set_t* s, uni_t* u, int l_a, int l_b)
{
    bool set_b[u->length];   //elements of set on line [line_b]
    for (int i = 0; i < u->length; i++)
    {
        set_b[i] = false;
    }

    for (int i = 0; i < s[l_b].length; i++)
    {
        set_b[s[l_b].elem_arr[i]] = true;
    }

    //check set on line [line_a]
    for (int i = 0; i < s[l_a].length; i++)
    {
        //extra element not contained in set on line [line_b] found
        if (!set_b[s[l_a].elem_arr[i]])
        {
            return false;
        }
    }

    //no extra elements found
    return true;
}

//prints whether or not is set on line [line] empty
void set_empty(set_t* s, int line)
{
    int l = set_line(s, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    //set has length 0 - is empty
    if (s[l].length == 0)
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set has length greater than 0
    fprintf(stdout, "false\n");
    return;
}

//prints number of elements in set on line [line]
void set_card(set_t* s, int line)
{
    int l = set_line(s, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    fprintf(stdout, "Set on line %d contains %d elements.\n", line, s[l].length);
    return;
}

//prints complement of set on line [line]
void set_complement(set_t* s, uni_t* u, int line)
{
    int l = set_line(s, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1)
    {
        return;
    }


    bool set_comp[u->length];   //complement of set on line [line]
    for (int i = 0; i < u->length; i++)
    {
        set_comp[i] = true;
    }

    //elements contained in set on line [line_a]
    for (int i = 0; i < s[l].length; i++)
    {
        set_comp[s[l].elem_arr[i]] = false;
    }

    set_boolprint('S', u, set_comp);
    return;
}

//prints union of sets on lines [line_a] and [line_b]
void set_union(set_t* s, uni_t* u, int line_a, int line_b)
{
    int l_a = set_line(s, line_a);  //index of set on line [line_a]
    int l_b = set_line(s, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    bool set_uni[u->length];   //union of sets on lines [line_a] and [line_b]
    for (int i = 0; i < u->length; i++)
    {
        set_uni[i] = false;
    }

    //elements contained in sets on line [l] - [line_a] and [line_b]
    int l = l_a;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < s[l].length; j++)
        {
            set_uni[s[l].elem_arr[j]] = true;
        }

        l = l_b;
    }


    set_boolprint('S', u, set_uni);
    return;
}

//prints intersect of sets on lines [line_a] and [line_b]
void set_intersect(set_t* s, uni_t* u, int line_a, int line_b)
{
    int l_a = set_line(s, line_a);  //index of set on line [line_a]
    int l_b = set_line(s, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    bool set_int[u->length];   //intersect of sets on lines [line_a] and [line_b]
    for (int i = 0; i < u->length; i++)
    {
        set_int[i] = false;
    }

    //elements contained in both sets on lines [line_a] and [line_b]
    for (int i = 0; i < s[l_a].length; i++)
    {
        for (int j = 0; j < s[l_b].length; j++)
        {
            if (s[l_a].elem_arr[i] == s[l_b].elem_arr[j])
            {
                set_int[s[l_a].elem_arr[i]] = true;
                break;
            }
        }
    }


    set_boolprint('S', u, set_int);
    return;
}

//prints set on line [line_a] minus set on line [line_b]
void set_minus(set_t* s, uni_t* u, int line_a, int line_b)
{
    int l_a = set_line(s, line_a);  //index of set on line [line_a]
    int l_b = set_line(s, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    bool set_min[u->length];   //set on line [line_a] minus set on line [line_b]
    for (int i = 0; i < u->length; i++)
    {
        set_min[i] = false;
    }

    //elements contained in set on line [line_a]
    for (int i = 0; i < s[l_a].length; i++)
    {
        set_min[s[l_a].elem_arr[i]] = true;
    }

    //elements contained in set on line [line_b]
    for (int i = 0; i < s[l_b].length; i++)
    {
        set_min[s[l_b].elem_arr[i]] = false;
    }

    set_boolprint('S', u, set_min);
    return;
}

//prints whether or not is set on line [line_a] a subset of set on line [line_b]
void set_subseteq(set_t* s, uni_t* u, int line_a, int line_b)
{
    int l_a = set_line(s, line_a);  //index of set on line [line_a]
    int l_b = set_line(s, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    //set on line [line_a] is a subset of set on line [line_b]
    if (set_sub(s, u, l_a, l_b))
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    fprintf(stdout, "false\n");
    return;
}

//prints whether or not is set on line [line_a] a proper subset of set on line [line_b]
void set_subset(set_t* s, uni_t* u, int line_a, int line_b)
{
    int l_a = set_line(s, line_a);  //index of set on line [line_a]
    int l_b = set_line(s, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    //set on line [line_a] is a subset of set on line [line_b]
    ///and set on line [line_b] isn't a subset of set on line [line_a]
    if (set_sub(s, u, l_a, l_b) && !set_sub(s, u, l_b, l_a))
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
void set_equals(set_t* s, uni_t* u, int line_a, int line_b)
{
    int l_a = set_line(s, line_a);  //index of set on line [line_a]
    int l_b = set_line(s, line_b);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1)
    {
        return;
    }


    //set on line [line_a] is a subset of set on line [line_b]
    ///and set on line [line_b] is a subset of set on line [line_a]
    if (set_sub(s, u, l_a, l_b) && set_sub(s, u, l_b, l_a))
    {
        fprintf(stdout, "true\n");
        return;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    ///and/or set on line [line_b] isn't a subset of set on line [line_a]
    fprintf(stdout, "false\n");
    return;
}


int main(int argc, char **argv)
{
    if(check_param(argc, argv) == 0){
        return EXIT_FAILURE;
    }

    uni_t uni;
    set_t set;
    data_t data;

    data_create(&data);

    if (text_load(file_open(argv), &data, &uni) == 0) {
        return EXIT_FAILURE;
    }

    printf("Kontrola cteni ze souboru\n");
    uni_print(&uni);
    set_print(data.arr_s[0], &uni);
    set_print(data.arr_s[1], &uni);
    printf("Kontrola cteni ze souboru\n\n");


    //uni_append(&uni, "Ahoj", 5);
    //uni_append(&uni, "1234", 5);

    set_create(&set, 1); //set on line 1, "a" "b"

    set_append(&set, 0);
    set_append(&set, 1);

    //check
    set_t sets[3];
    sets[0] = set;

    set_print(&set, &uni);


    set_create(&set, 3); //set on line 3, "b" "b"
    set_append(&set, 1);
    set_append(&set, 1);
    sets[1] = set;

    set_print(&set, &uni);

    set_create(&set, 4); //set on line 4, empty
    sets[2] = set;

    set_print(&set, &uni);


    set_empty(sets, 4);
    set_card(sets, 2);
    set_complement(sets, &uni, 3);
    set_union(sets, &uni, 1, 3);
    set_intersect(sets, &uni, 1, 3);
    set_minus(sets, &uni, 1, 3);
    set_subseteq(sets, &uni, 3, 4);
    set_subset(sets, &uni, 3, 1);
    set_equals(sets, &uni, 1, 3);

    //data_destroy(&d)

    set_destroy(&set);
    uni_destroy(&uni);

    return EXIT_SUCCESS;
}
