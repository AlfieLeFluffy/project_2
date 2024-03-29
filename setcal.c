/********************************
 *
 * setcal.c
 *
 * ver 1.5
 * =========================
 *
 * 05.12.2021
 *
 * xjobra01
 * xsopfo00
 * xvasak01
 * xvlach24
 *
 *******************************/

/** header files **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** definitions of new data types **/
//universe
typedef struct{
	int length;
	int cap;
	char **elem_arr;
} uni_t;

//set
typedef struct{
	int line;
	int length;
	int cap;
	int *elem_arr;
} set_t;

//element of relation
typedef struct{
    int e_1;
    int e_2;
} elpair_t;

//relation
typedef struct{
	int line;
	int length;         //number of pairs
	int cap;
	elpair_t *elem_arr;
} rel_t;

//universe and all sets and relations
typedef struct{
    uni_t uni;
    set_t **arr_s;      //array of all set pointers
    int length_s;
    int cap_s;
    rel_t **arr_r;      //array of all relation pointers
    int length_r;
    int cap_r;
} data_t;

//function pointer
typedef int (*pfunc)(data_t*, int, int*, int);

/** function prototypes **/
//prototypes of command functions because of function pointers
int set_empty(data_t*, int, int*, int);
int set_card(data_t*, int, int*, int);
int set_complement(data_t*, int, int*, int);
int set_union(data_t*, int, int*, int);
int set_intersect(data_t*, int, int*, int);
int set_minus(data_t*, int, int*, int);
int set_subseteq(data_t*, int, int*, int);
int set_subset(data_t*, int, int*, int);
int set_equals(data_t*, int, int*, int);

int rel_reflexive(data_t*, int, int*, int);
int rel_symmetric(data_t*, int, int*, int);
int rel_antisymmetric(data_t*, int, int*, int);
int rel_transitive(data_t*, int, int*, int);
int rel_function(data_t*, int, int*, int);
int rel_domain(data_t*, int, int*, int);
int rel_codomain(data_t*, int, int*, int);
int rel_injective(data_t*, int, int*, int);
int rel_surjective(data_t*, int, int*, int);
int rel_bijective(data_t*, int, int*, int);
int rel_closure_ref(data_t*, int, int*, int);
int rel_closure_sym(data_t*, int, int*, int);
int rel_closure_trans(data_t*, int, int*, int);

/** definitions of constants **/
#define ELEM_LEN 31         //max. allowed length of strings + 1 ... used also for commands (changeable if needed)
#define LINES_MAX 1000      //max. allowed number of lines in a file
#define DIGITS_LINES_MAX 5  //digits of LINES_MAX + 1
#define COM_NUM 22          //number of supported commands
#define ARG_MAX 3           //max. number of arguments of any command
#define TRUE "true"
#define FALSE "false"

//array of names of all supported commands
const char com_arr_n[COM_NUM][ELEM_LEN] = {
    "empty", "card",                                                                    //commands without parameter universe
    "complement", "union", "intersect", "minus", "subseteq", "subset", "equals",        //other set commands
    "reflexive", "symmetric", "antisymmetric", "transitive", "function", "domain", "codomain", "injective", "surjective", "bijective",  //rel commands
    "closure_ref", "closure_sym", "closure_trans"};                                                  //bonus rel commands

//array of function pointers
const pfunc com_arr_p[COM_NUM] = {
    &set_empty, &set_card, &set_complement, &set_union, &set_intersect, &set_minus, &set_subseteq, &set_subset, &set_equals,
    &rel_reflexive, &rel_symmetric, &rel_antisymmetric, &rel_transitive, &rel_function, &rel_domain, &rel_codomain, &rel_injective, &rel_surjective, &rel_bijective,
    &rel_closure_ref, &rel_closure_sym, &rel_closure_trans};


/** definitions of functions **/
//functions return 1 (or -1) when successful and 0 when not if not stated differently

/*
    Memory and printing functions
*/

/* prints memory error */
void memory_err()
{
    fprintf(stderr, "Memory error\n");
}
 /* prints error for invalid num of args */
void arg_err(int line)
{
    fprintf(stderr, "Invalid number of command arguments on line %d\n", line);
}

/* function for printing universe */
void uni_print(uni_t *u)
{
    fprintf(stdout, "U");
    if (u->length == 0) {
        fprintf(stdout, "\n");
        return;
	}
	fprintf(stdout, " ");

    for (int i = 0; i < u->length; i++) {
        if (i == u->length - 1) {
            fprintf(stdout, "%s\n", u->elem_arr[i]);
        }
        else {
            fprintf(stdout, "%s ", u->elem_arr[i]);
        }
    }
}

/* find set defined on line [line] */
int set_line(data_t* data, int line)
{
    for (int i = 0; i < data->length_s && i < line; i++){
        //set on line [line] found
        if (data->arr_s[i]->line == line){
            return i;
        }
    }

    //set not found
    fprintf(stderr, "No set defined on line %d.\n", line);
    return -1;
}

/* function for printing set */
void set_print(data_t *d, int line)
{
	set_t *s = d->arr_s[set_line(d, line)];

	fprintf(stdout, "S");
	if (s->length == 0) {
        fprintf(stdout, "\n");
        return;
	}
	fprintf(stdout, " ");

	for(int i = 0; i < s->length; i++){
		if (i == s->length - 1) {
            fprintf(stdout, "%s\n", d->uni.elem_arr[s->elem_arr[i]]);
        }
        else {
            fprintf(stdout, "%s ", d->uni.elem_arr[s->elem_arr[i]]);
        }
	}
}

/* find relation defined on line [line] */
int rel_line(data_t* data, int line)
{
    for (int i = 0; i < data->length_r && i < line; i++){
        //relation on line [line] found
        if (data->arr_r[i]->line == line){
            return i;
        }
    }

    //relation not found
    fprintf(stderr, "No relation defined on line %d.\n", line);
    return -1;
}

/* function for printing relation */
void rel_print(data_t *d, int line)
{
    rel_t *r = d->arr_r[rel_line(d, line)];

    fprintf(stdout, "R");
	if (r->length == 0) {
        fprintf(stdout, "\n");
        return;
	}
	fprintf(stdout, " ");

	for(int i = 0; i < r->length; i++){
		if (i == r->length - 1) {
            fprintf(stdout, "(%s %s)\n", d->uni.elem_arr[r->elem_arr[i].e_1], d->uni.elem_arr[r->elem_arr[i].e_2] );
        }
        else {
            fprintf(stdout, "(%s %s) ", d->uni.elem_arr[r->elem_arr[i].e_1], d->uni.elem_arr[r->elem_arr[i].e_2] );
        }
	}
}

/* function for universe initialization */
void uni_create(uni_t *u)
{
    u->length = 0;
    u->cap = 0;
    u->elem_arr = NULL;
}

/* function for set initialization */
void set_create(set_t *s, int line)
{
    s->line = line;
    s->length = 0;
    s->cap = 0;
    s->elem_arr = NULL;
}

/* function for relation initialization */
void rel_create(rel_t *r, int line)
{
    r->line = line;
    r->length = 0;
    r->cap = 0;
    r->elem_arr = NULL;
}

/* function for data initialization */
void data_create(data_t *d)
{
    d->arr_s = NULL;
    d->length_s = 0;
    d->cap_s = 0;
    d->arr_r = NULL;
    d->length_r = 0;
    d->cap_r = 0;

    uni_create(&(d->uni));
}

/* function for adding a new element to universe */
int uni_append(uni_t *u, char *elem, int str_len)
{
    //check, if string isn't empty because of end of line
    if (str_len == 0) {
        return 1;
    }

    //if needed, increase capacity of array of universe element pointers
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

    //allocation of memory for new element
    u->elem_arr[u->length] = malloc((str_len+1) * sizeof(char));
    if( u->elem_arr[u->length] == NULL) {
        memory_err();
        return 0;
    }

    //copy new element to universe
    strcpy(u->elem_arr[u->length], elem);

    u->length++;

    return 1;
}

/* function for adding a new element to set */
int set_append(set_t *s, int elem)
{
    //if needed, increase set's capacity
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

    //copy new element to set
    s->elem_arr[s->length] = elem;
    s->length++;

    return 1;
}

/* function for adding a new pair to relation */
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

/* function for deallocating memory used by universe */
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

/* function for deallocating memory used by set */
void set_destroy(set_t *s)
{
    if (s->elem_arr != NULL) {
        free(s->elem_arr);
    }
    s->length = 0;
    s->cap = 0;
}

/* function for deallocating memory used by relation */
void rel_destroy(rel_t *r)
{
    if (r->elem_arr != NULL) {
        free(r->elem_arr);
    }
    r->length = 0;
    r->cap = 0;
}

/* function for deallocating memory used by data */
void data_destroy(data_t *d)
{
    //deallocation of memory used by sets
    if (d->arr_s != NULL) {
        for (int i = 0; i < d->cap_s; i++) {
            set_destroy(d->arr_s[i]);
            free(d->arr_s[i]);
        }

        free(d->arr_s);
    }

    //deallocation of memory used by relations
    if (d->arr_r != NULL) {
        for (int i = 0; i < d->cap_r; i++) {
            rel_destroy(d->arr_r[i]);
            free(d->arr_r[i]);
        }

        free(d->arr_r);
    }
}

/* deallocates all allocated memory */
void destroy_all(data_t *d, uni_t *u)
{
    uni_destroy(u);
    data_destroy(d);
}

/*
    Functions for loading file content
*/

/* function for checking the number of program's parameters */
int check_param(int argc)
{
    if (argc != 2){
        fprintf(stderr, "Invalid number of args\n");
        return 0;
    }
    return 1;
}

/* function for checking if a character is supported */
int check_char(char c)
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ){
        return 1;
    }

    fprintf(stderr, "Char '%c' is not supported\n", c);
    return 0;
}

/* function for checking if a character is supported for commands */
int check_char_com(char c)
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
        return 1;
    }

    fprintf(stderr, "Char '%c' is not supported\n", c);
    return 0;
}

/* appends string by one character */
void str_append(char str[], char c, int *len)
{
    //len equals to the position of '\0'
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

/* function for loading a string from file */
char load_str(FILE *fp, char str[], int *len, char mode)
{
    //if [mode] == 'c' run in command mode, else run normally
    //when successful, function returns the character immediately after the loaded string (space or '\n')

    char c;
    *len = 0;  //length of loaded string ... needed for loading to universe
    str[0] = '\0';

    c = skip_space(fp);

    //end of line equals to end of the set etc. => return
    if (c == '\n') {
        return c;
    }

    do {
        //if c isn't a supported letter, return 0
        if (mode == 'c') {
            if(check_char_com(c) == 0){
                return 0;
            }
        }
        else if(check_char(c) == 0){
            return 0;
        }
        str_append(str, c, len);      //first iteration uses a first character after removing all spaces

        c = fgetc(fp);       //get the next character

        //end of string check
        if ( c == ' ' || c == '\n') {
            return c;
        }
    } while (*len < ELEM_LEN - 1);

    //leaving while loop is possible only when max. supported length of element is exceeded
    fprintf(stderr, "String (%s...) exceeds length of %d\n", str, ELEM_LEN - 1);
    return 0;
}

/* function determines whether or not a string is setcal keyword */
int is_keyword(char str[])
{
    for (int i = 0; i < COM_NUM; i++){
        if (strcmp(str, com_arr_n[i]) == 0){
            return 1;
        }
    }
    if ((strcmp(str, TRUE) == 0) || (strcmp(str, FALSE) == 0)){
        return 1;
    }
    return 0;
}

/* function for loading a line as universe */
int load_uni(FILE *fp, uni_t *u)
{
    char temp_s[ELEM_LEN];
    char c;
    int len;

    do {
        //to check '\n' and 0 separately, we need to save the
        //return value of 'load_str'

        c = load_str(fp, temp_s, &len, 'n');

        //if 'load_str' returns 0 => error
        if (c == 0){
            return 0;
        }

        if (is_keyword(temp_s) == 1){
            fprintf(stderr, "Keywords cannot be set as universe elements (used: '%s')\n", temp_s);
            return 0;
        }

        //check if element isn't already in universe
        for (int i = 0; i < u->length; i++) {
            if (strcmp(temp_s, u->elem_arr[i]) == 0) {
                fprintf(stderr, "Element %s duplicate in universe\n", temp_s);
                return 0;
            }
        }

        //append universe by the loaded string and repeat
        if (uni_append(u, temp_s, len) == 0) {
            return 0;
        }
    } while (c != '\n'); //if c is '\n' the end of line is reached => end load

    //else success
    return 1;
}

/* function for adding universe as set to data */
int u_to_s(data_t *d, int line)
{
    set_t *s = malloc(sizeof(set_t));
    if (s == NULL) {
        memory_err();
        return 0;
    }
    set_create(s, line);
    for (int i = 0; i < d->uni.length; i++){
        //fill set with all universe's indexes
        if (set_append(s, i) == 0){
            return 0;
        }
    }

    //copy set pointer to array
    if (data_append_s(d, s) == 0){
            return 0;
    }

    return 1;
}

/* function for finding out if element already is in set */
int isin_set(set_t s, int elem)
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
    //returns -1 when no element matches, to avoid conflict with index 0

    for (int i = 0; i < u->length; i++) {
        if (strcmp(s, u->elem_arr[i]) == 0) {
            return i;
        }
    }

    fprintf(stderr, "Element %s not in universe (line %d)\n", s, line);
    return -1;
}

/* function for loading one element to set */
int elem_to_s(uni_t *u, set_t *s, char temp_s[], int line)
{
    //check if string isn't empty because of end of line
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

    //check if appending set was successful
    if (set_append(s, ind) == 0) {
        return 0;
    }

    //else success
    return 1;
}

/* function for loading all elements in a line to set */
int load_set(FILE *fp, data_t *d, int line)
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
        c = load_str(fp, temp_s, &len, 'n');
        //if load_str returns 0 (=error), return 0
        if (c == 0){
            set_destroy(s);
            free(s);
            return 0;
        }
        //load element to set
        if (elem_to_s(&(d->uni), s, temp_s, line) == 0) {
            set_destroy(s);
            free(s);
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

/* function for loading the first element of a relation pair */
int load_str_r_1(FILE *fp, char str1[], int line)
{
    int len = 0;
    char c;

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

    //else success
    return 1;
}

/* function for loading the second element of a relation pair */
int load_str_r_2(FILE *fp, char str2[], int line)
{
    int len = 0;
    char c = skip_space(fp);

    do {
        if (check_char(c) == 0){
            return 0;
        }
        str_append(str2, c, &len);      //add character to string

        c = fgetc(fp);       //get new character

        if ( c == '\n') {
            fprintf(stderr, "Unfinished pair in relation on line %d\n", line);
            return 0;
        }
        if ( c == ' ') {
            fprintf(stderr, "Invalid format of relation on line %d\n", line);
            return 0;
        }

        if (len == ELEM_LEN - 1) {
            fprintf(stderr, "Element (%s...) exceeds length of %d\n", str2, ELEM_LEN - 1);
            return 0;
        }
    } while (c != ')');

    if (len == 0) {
        fprintf(stderr, "Invalid format of relation on line %d\n", line);
        return 0;
    }

    //else success
    return 1;
}

/* function for loading the 2nd string of a relation */
char load_str_r(FILE *fp, char str1[], char str2[], int line)
{
    //when successful, function returns next character after pair (space or \n)
    char c;
    str1[0] = '\0';
    str2[0] = '\0';


    c = skip_space(fp);

    //return successfully if end of relation is loaded
    if (c == '\n') {
        return c;
    }

    if (c != '(') {
        fprintf(stderr, "Invalid format of relation on line %d\n", line);
        return 0;
    }

    if (load_str_r_1(fp, str1, line) == 0){
        return 0;
    }

    if (load_str_r_2(fp, str2, line) == 0){
        return 0;
    }

    //success if both strings were loaded correctly
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

/* function for loading all elements in a line to relation */
int load_rel(FILE *fp, data_t *d, int line)
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
            rel_destroy(r);
            free(r);
            return 0;
        }

        //load pair to relation
        if ((pair_to_r(&(d->uni), r, temp_s1, temp_s2, line)) == 0) {
            rel_destroy(r);
            free(r);
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

/* function for getting digits from a file */
void get_digits(FILE *fp, char *c, char strnum[])
{
    for (int j = 1; j < DIGITS_LINES_MAX; j++) {
        *c = fgetc(fp);
        if (*c == ' ' || *c == '\n') {
            break;
        }
        strnum[j] = *c;
        strnum[j+1] = '\0';
    }
}

/* function for converting string to int and adding it to array */
int snum_to_arr(int i, int arg_arr[], char strnum[], int line)
{
    char *p_end = NULL;  //pointer to end of number in strtoul()

    arg_arr[i] = strtoul(strnum, &p_end, 10);

    if (p_end[0] != '\0' || arg_arr[i] < 1 || arg_arr[i] > LINES_MAX) {
        fprintf(stderr, "Invalid format of command argument on line %d\n", line);
        return 0;
    }
    return 1;

}

/* function for loading command arguments from file */
int load_com_args(FILE *fp, int *arg_count, int arg_arr[], int line)
{
    char c;
    int i;
    char strnum[DIGITS_LINES_MAX];  //number in string

    for (i = 0; i < ARG_MAX; i++) {
        //get first digit of a number
        c = skip_space(fp);
        if (c == '\n') {
            (*arg_count) = i;
            return 1;
        }
        //add it to string
        strnum[0] = c;
        strnum[1] = '\0';

        get_digits(fp, &c, strnum);

        //convert string to int and add it to array
        if (snum_to_arr(i, arg_arr, strnum, line) == 0) {
            return 0;
        }

        if (c == '\n') {
            (*arg_count) = i+1;
            return 1;
        }
        //reset
        strnum[0] = '\0';
    }

    if (skip_space(fp) == '\n') {
        (*arg_count) = i+1;
        return 1;
    }
    //else failure
    arg_err(line);
    return 0;
}

/* function for loading and executing command from file */
int load_com(FILE *fp, data_t *d, int line)
{
    char com[ELEM_LEN];
    int len;
    char c;
    bool found = false;
    int com_i;          //index of command in array of commands
    int arg_count = 0;
    int arg_arr[ARG_MAX];

    //load command
    if ((c = load_str(fp, com, &len, 'c')) == 0) {
        return 0;
    }
    if (c == '\n') {
        fprintf(stderr, "Command arguments missing on line %d\n", line);
        return 0;
    }
    //check, if command exists
    for (int i = 0; i < COM_NUM; i++) {
        if (strcmp(com, com_arr_n[i]) == 0) {
            com_i = i;
            found = true;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "Unknown command (%s) on line %d\n", com, line);
        return 0;
    }
    //load command arguments
    if (load_com_args(fp, &arg_count, arg_arr, line) == 0) {
        return 0;
    }
    //execute commands
    if ( (*com_arr_p[com_i])(d, arg_count, arg_arr, line) == 0 ){
        return 0;
    }

    return 1;
}

/* function for loading and checking a line with a universe on it */
int caseU(FILE *fp, data_t *d, int lines)
{
    //check if universe is on line 1
    if (lines != 1) {
        fprintf(stderr, "Universe on an unexpected line (line %d)\n", lines);
        return 0;
    }

    //load universe from line
    if (load_uni(fp, &(d->uni)) == 0){
        return 0;
    }
    //load universe as a set into data
    if (u_to_s(d, lines) == 0){
        return 0;
    }

    //else success
    uni_print(&(d->uni));
    return 1;
}

/* function for loading and checking a line with a set on it */
int caseS(FILE *fp, data_t *d, int lines)
{
    //check if set is not on line 1
    if (lines == 1) {
        fprintf(stderr, "Universe expected on line 1 instead of set\n");
        return 0;
    }

    //load set into data
    if (load_set(fp, d, lines) == 0) {
        return 0;
    }

    //else success
    set_print(d, lines);
    return 1;
}

/* function for loading and checking a line with a relation on it */
int caseR(FILE *fp, data_t *d, int lines)
{
    //check if relation in not on line 1
    if (lines == 1) {
        fprintf(stderr, "Universe expected on line 1 instead of relation\n");
        return 0;
    }

    //load relation into data
    if (load_rel(fp, d, lines) == 0) {
        return 0;
    }

    //else success
    rel_print(d, lines);
    return 1;
}

/* function for loading and checking a line with a command on it */
int caseC(FILE *fp, data_t *d, int lines)
{
    //check if command in not on line 1
    if (lines == 1) {
        fprintf(stderr, "Universe expected on line 1 instead of command\n");
        return 0;
    }

    //load and execute command
    if (load_com(fp, d, lines) == 0) {
        return 0;
    }

    //else success
    return 1;
}

/* function for loading setcal-formatted text from a file */
int text_load(FILE *fp, data_t *d)
{
    //read file by lines
    for (int lines = 1; lines <= LINES_MAX + 1; lines++) {
        //check the type of line
        switch(fgetc(fp)) {
            case 'U':
                if (caseU(fp, d, lines) == 0){
                    return 0;
                }
                continue;

            case 'S':
                if (caseS(fp, d, lines) == 0){
                    return 0;
                }
                continue;

            case 'R':
                if (caseR(fp, d, lines) == 0){
                    return 0;
                }
                continue;

            case 'C':
                if (caseC(fp, d, lines) == 0){
                    return 0;
                }
                continue;

            case EOF:
                return 1;

            default:
                fprintf(stderr, "Unidentified line number %d\n", lines);
                return 0;
        }
    }
    fprintf(stderr, "Number of lines exceeds %d\n", LINES_MAX);
    return 0;
}

/* function for opening a file and loading its content */
int file_load(char **argv, data_t *d)
{
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL){
        fprintf(stderr, "Unable to open file\n");
        return 0;
    }

    if (text_load(fp, d) == 0){
        fclose(fp);
        destroy_all(d, &(d->uni));
        return 0;
    }
    fclose(fp);
    return 1;
}

/* function checks the number of args given vs the number of expected args */
int com_arg_check(int expected_args, int arg_count, int lines)
{
    if (arg_count != expected_args){
        arg_err(lines);
        return 0;
    }
    return 1;
}

/* adds chosen universe elements to a set which is added to data */
int bool_append(data_t *data, bool* b, set_t *set)
{
    for (int i = 0; i < data->uni.length; i++){
        //all marked elements are added to set
        if (b[i]){
            if (set_append(set, i) == 0) {
                return 0;
            }
        }
    }

    if (data_append_s(data, set) == 0) {
        return 0;
    }

    return 1;
}

/* resets bool array */
void bool_reset(uni_t* uni, bool* arr, bool b)
{
    for (int i = 0; i < uni->length; i++){
        arr[i] = b;
    }
}

/*
    Functions for set commands
*/

/* returns whether or not is set on line [line_a] a subset of set on line [line_b] */
bool set_sub(data_t* data, int l_a, int l_b)
{
    bool set_b[data->uni.length];   //elements of set on line [line_b]
    bool_reset(&(data->uni), set_b, false);

    for (int i = 0; i < data->arr_s[l_b]->length; i++){
        set_b[data->arr_s[l_b]->elem_arr[i]] = true;
    }

    //check set on line [line_a]
    for (int i = 0; i < data->arr_s[l_a]->length; i++){
        //extra element not contained in set on line [line_b] found
        if (!set_b[data->arr_s[l_a]->elem_arr[i]]){
            return false;
        }
    }

    //no extra elements found
    return true;
}

/* prints whether or not is set on line [line] empty */
int set_empty(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (arg_count != 1){
        arg_err(lines);
        return 0;
    }

    int l = set_line(data, arg_arr[0]);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1){
        return 0;
    }


    //set has length 0 - is empty
    if (data->arr_s[l]->length == 0){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }

    //set has length greater than 0
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}

/* prints number of elements in set on line [line] */
int set_card(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int line = arg_arr[0];
    int l = set_line(data, line);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1){
        return 0;
    }

    fprintf(stdout, "%d\n", data->arr_s[l]->length);
    return 1;
}

/* prints complement of set on line [line] */
int set_complement(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }
    int l = set_line(data, arg_arr[0]);  //index of set on line [line]

    //invalid argument [line]
    if (l == -1){
        return 0;
    }


    bool set_comp[data->uni.length];   //complement of set on line [line]
    bool_reset(&(data->uni), set_comp, true);

    //elements contained in set on line [line_a]
    for (int i = 0; i < data->arr_s[l]->length; i++){
        set_comp[data->arr_s[l]->elem_arr[i]] = false;
    }

    set_t *set = malloc(sizeof(set_t));
    set_create(set, lines);

    if (bool_append(data, set_comp, set) == 0) {
        set_destroy(set);
        free(set);
        return 0;
    }

    set_print(data, lines);
    return 1;
}

/* prints union of sets on lines [line_a] and [line_b] */
int set_union(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(2, arg_count, lines) == 0) {
        return 0;
    }

    int l_a = set_line(data, arg_arr[0]);  //index of set on line [line_a]
    int l_b = set_line(data, arg_arr[1]);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1) {
        return 0;
    }

    bool set_uni[data->uni.length];   //union of sets on lines [line_a] and [line_b]
    bool_reset(&(data->uni), set_uni, false);

    //elements contained in sets on lines [line_a] and [line_b]
    int l = l_a;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < data->arr_s[l]->length; j++) {
            set_uni[data->arr_s[l]->elem_arr[j]] = true;
        }

        l = l_b;
    }

    set_t *set = malloc(sizeof(set_t));
    set_create(set, lines);

    if (bool_append(data, set_uni, set) == 0) {
        set_destroy(set);
        free(set);
        return 0;
    }

    set_print(data, lines);
    return 1;
}

/* prints intersect of sets on lines [line_a] and [line_b] */
int set_intersect(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(2, arg_count, lines) == 0){
        return 0;
    }

    int l_a = set_line(data, arg_arr[0]);  //index of set on line [line_a]
    int l_b = set_line(data, arg_arr[1]);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1) {
        return 0;
    }


    bool set_int[data->uni.length];   //intersect of sets on lines [line_a] and [line_b]
    bool_reset(&(data->uni), set_int, false);

    //elements contained in both sets on lines [line_a] and [line_b]
    for (int i = 0; i < data->arr_s[l_a]->length; i++) {
        for (int j = 0; j < data->arr_s[l_b]->length; j++) {
            if (data->arr_s[l_a]->elem_arr[i] == data->arr_s[l_b]->elem_arr[j]) {
                set_int[data->arr_s[l_a]->elem_arr[i]] = true;
                break;
            }
        }
    }

    set_t *set = malloc(sizeof(set_t));
    set_create(set, lines);

    if (bool_append(data, set_int, set) == 0) {
        set_destroy(set);
        free(set);
        return 0;
    }
    set_print(data, lines);

    return 1;
}

/* prints set on line [line_a] minus set on line [line_b] */
int set_minus(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(2, arg_count, lines) == 0) {
        return 0;
    }

    int l_a = set_line(data, arg_arr[0]);  //index of set on line [line_a]
    int l_b = set_line(data, arg_arr[1]);  //index of set on line [line_b]


    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1) {
        return 0;
    }


    bool set_min[data->uni.length];   //set on line [line_a] minus set on line [line_b]
    bool_reset(&(data->uni), set_min, false);

    //elements contained in set on line [line_a]
    for (int i = 0; i < data->arr_s[l_a]->length; i++) {
        set_min[data->arr_s[l_a]->elem_arr[i]] = true;
    }

    //elements contained in set on line [line_b]
    for (int i = 0; i < data->arr_s[l_b]->length; i++) {
        set_min[data->arr_s[l_b]->elem_arr[i]] = false;
    }

    set_t *set = malloc(sizeof(set_t));
    set_create(set, lines);

    if (bool_append(data, set_min, set) == 0) {
        set_destroy(set);
        free(set);
        return 0;
    }

    set_print(data, lines);
    return 1;
}

/* prints whether or not is set on line [line_a] a subset of set on line [line_b] */
int set_subseteq(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(2, arg_count, lines) == 0){
        return 0;
    }

    int l_a = set_line(data, arg_arr[0]);  //index of set on line [line_a]
    int l_b = set_line(data, arg_arr[1]);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1){
        return 0;
    }

    //set on line [line_a] is a subset of set on line [line_b]
    if (set_sub(data, l_a, l_b)){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}

/* prints whether or not is set on line [line_a] a proper subset of set on line [line_b] */
int set_subset(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(2, arg_count, lines) == 0){
        return 0;
    }

    int l_a = set_line(data, arg_arr[0]);  //index of set on line [line_a]
    int l_b = set_line(data, arg_arr[1]);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1){
        return 0;
    }

    //set on line [line_a] is a subset of set on line [line_b]
    //and set on line [line_b] isn't a subset of set on line [line_a]
    if (set_sub(data, l_a, l_b) && !set_sub(data, l_b, l_a)){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    //and/or set on line [line_b] is a subset of set on line [line_a]
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}

/* prints whether or not are sets on lines [line_a] and [line_b] equal */
int set_equals(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(2, arg_count, lines) == 0){
        return 0;
    }

    int l_a = set_line(data, arg_arr[0]);  //index of set on line [line_a]
    int l_b = set_line(data, arg_arr[1]);  //index of set on line [line_b]

    //invalid argument [line_a] and/or [line_b]
    if (l_a == -1 || l_b == -1){
        return 0;
    }

    //set on line [line_a] is a subset of set on line [line_b]
    //and set on line [line_b] is a subset of set on line [line_a]
    if (set_sub(data, l_a, l_b) && set_sub(data, l_b, l_a)){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }

    //set on line [line_a] isn't a subset of set on line [line_b]
    //and/or set on line [line_b] isn't a subset of set on line [line_a]
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}


/*
    Functions for relation commands
*/

/* returns chosen elements of relation with index [l] */
void rel_elements(data_t* data, int l, int el, bool* rel)
{
    bool_reset(&(data->uni), rel, false);

    //chosen elements from relation with index [l]
    for (int i = 0; i < data->arr_r[l]->length; i++){
        if (el == 1){
            rel[data->arr_r[l]->elem_arr[i].e_1] = true;
        }
        else{
            rel[data->arr_r[l]->elem_arr[i].e_2] = true;
        }
    }

    return;
}

/* function for determining if an x or y value of a relation is not duplicated */
bool rel_dupl(data_t* data, int l, int el)
{
    bool rel_el[data->uni.length];  //chosen values from relation on line [line]
    bool_reset(&(data->uni), rel_el, false);

    for (int i = 0; i < data->arr_r[l]->length; i++){
        if (el == 1){
            //x value found multiple times
            if (rel_el[data->arr_r[l]->elem_arr[i].e_1]){
                return false;
            }

            rel_el[data->arr_r[l]->elem_arr[i].e_1] = true;
        }
        else{
            //y value found multiple times
            if (rel_el[data->arr_r[l]->elem_arr[i].e_2]){
                return false;
            }

            rel_el[data->arr_r[l]->elem_arr[i].e_2] = true;
        }
    }

    return true;
}

/* prints whether or not is the relation on line [line] reflexive */
int rel_reflexive(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0) {
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1) {
        return 0;
    }

    elpair_t temp_pair;

    //find a reflexive element for each element in universe
    for (int i = 0; i < data->uni.length; i++) {
        temp_pair.e_1 = i;
        temp_pair.e_2 = i;

        //find (i i)
        if (isin_rel(data->arr_r[l], temp_pair) == 0) {
            fprintf(stdout, "%s\n", FALSE);
            return -1;
        }
    }

    //else success
    fprintf(stdout, "%s\n", TRUE);
    return 1;
}

/* prints whether or not is the relation on line [line] symmetric */
int rel_symmetric(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1) {
        return 0;
    }

    elpair_t temp_pair;

    //for all pairs find a pair they are symmetric with
    for (int i = 0; i < data->arr_r[l]->length; i++) {
        //skip diagonal pairs because they are symmetric to themselves
        if (data->arr_r[l]->elem_arr[i].e_1 != data->arr_r[l]->elem_arr[i].e_2) {
            temp_pair.e_1 = data->arr_r[l]->elem_arr[i].e_2;
            temp_pair.e_2 = data->arr_r[l]->elem_arr[i].e_1;

            //for i = (a b) find (b a)
            if (isin_rel(data->arr_r[l], temp_pair) == 0) {
                fprintf(stdout, "%s\n", FALSE);
                return -1;
            }
        }
    }

    //else success
    fprintf(stdout, "%s\n", TRUE);
    return 1;
}

/* prints whether or not is the relation on line [line] antisymmetric */
int rel_antisymmetric(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1) {
        return 0;
    }

    elpair_t temp_pair;

    //if pair is not on diagonal and they have a pair they are symmetric with, relation is not antisymmetric
    for (int i = 0; i < data->arr_r[l]->length; i++) {
        //don't check diagonal pairs
        if (data->arr_r[l]->elem_arr[i].e_1 != data->arr_r[l]->elem_arr[i].e_2) {
            temp_pair.e_1 = data->arr_r[l]->elem_arr[i].e_2;
            temp_pair.e_2 = data->arr_r[l]->elem_arr[i].e_1;

            //for i = (a b) find (b a)
            if (isin_rel(data->arr_r[l], temp_pair) == 1) {
                fprintf(stdout, "%s\n", FALSE);
                return -1;
            }
        }
    }

    //else success
    fprintf(stdout, "%s\n", TRUE);
    return 1;
}

/* prints whether or not is the relation on line [line] transitive */
int rel_transitive(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1) {
        return 0;
    }

    elpair_t temp_pair;

    //check if relation is transitive for each pair
    for (int i = 0; i < data->arr_r[l]->length; i++) {
        //don't check diagonal pairs, because they don't generate anything new
        if (data->arr_r[l]->elem_arr[i].e_1 != data->arr_r[l]->elem_arr[i].e_2) {
            for (int j = 0; j < data->arr_r[l]->length; j++) {
                //if i = (? b) and j = (b ??)
                if (data->arr_r[l]->elem_arr[i].e_2 == data->arr_r[l]->elem_arr[j].e_1) {
                    temp_pair.e_1 = data->arr_r[l]->elem_arr[i].e_1;
                    temp_pair.e_2 = data->arr_r[l]->elem_arr[j].e_2;

                    //if (? ??) not found -> relation is not transitive
                    if (isin_rel(data->arr_r[l], temp_pair) == 0) {
                        fprintf(stdout, "%s\n", FALSE);
                        return -1;
                    }
                }
            }
        }
    }

    //else success
    fprintf(stdout, "%s\n", TRUE);
    return 1;
}

/* prints whether or not is the relation on line [line] a function */
int rel_function(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1){
        return 0;
    }

    //x values aren't in relation on line [line] more than once
    if (rel_dupl(data, l, 1)){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }

    fprintf(stdout, "%s\n", FALSE);
    return 1;
}

/* prints the domain of the function on line [line] */
int rel_domain(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument [line]
    if (l == -1){
        return 0;
    }

    bool rel_dom[data->uni.length];  //domain of relation on line [line]
    rel_elements(data, l, 1, rel_dom);

    set_t *set = malloc(sizeof(set_t));
    set_create(set, lines);

    if (bool_append(data, rel_dom, set) == 0) {
        set_destroy(set);
        free(set);
        return 0;
    }

    set_print(data, lines);
    return 1;
}

/* prints the codomain of the function on line [line] */
int rel_codomain(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]

    //invalid argument
    if (l == -1){
        return 0;
    }

    bool rel_cod[data->uni.length];  //codomain of relation on line [line]
    rel_elements(data, l, 2, rel_cod);

    set_t *set = malloc(sizeof(set_t));
    set_create(set, lines);

    if (bool_append(data, rel_cod, set) == 0) {
        set_destroy(set);
        free(set);
        return 0;
    }

    set_print(data, lines);
    return 1;
}

/* are all of of relation's elements x/y values part of sets defining x/y */
int rel_elems_in_sets(rel_t *r, set_t *sx, set_t *sy)
{
    for (int i = 0; i < r->length; i++){
        //check if x is an element of the x set
        if (isin_set(*sx, r->elem_arr[i].e_1) == 0){
            return 0;
        }

        //check if y is an element of the y set
        if (isin_set(*sy, r->elem_arr[i].e_2) == 0){
            return 0;
        }
    }
    return 1;
}

/* decides if relation on line l, defined by sets on lines x and y is injective */
int inj(data_t *data, int l, int x, int y)
{
    //not specified if whether to return error or not

    if (rel_elems_in_sets(data->arr_r[l], data->arr_s[x], data->arr_s[y]) == 0) {
        return 0;
    }

    //x values aren't in relation on line [line] more than once (injective has to be a function)
    if (!rel_dupl(data, l, 1)){
        return -1;
    }

    //y values aren't in relation on line [line] more than once
    if (!rel_dupl(data, l, 2)){
        return -1;
    }
    return 1;
}

/* prints whether or not is the relation on line [line] injective */
int rel_injective(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(3, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation
    int x = set_line(data, arg_arr[1]);  //index of set defining x values
    int y = set_line(data, arg_arr[2]);  //index of set defining y values

    //invalid argument
    if (l == -1 || x == -1 || y == -1){
        return 0;
    }
    //return value of inj is stored in check in order to check it multiple times
    int check = inj(data, l, x, y);
    if (check == 0){
        fprintf(stdout, "%s\n", FALSE);
        return -1;
    }
    if (check == 1){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}

/* decides if relation on line l, defined by sets on lines x and y is surjective */
int surj(data_t *data, int l, int x, int y)
{
    //not specified if whether to return error or not

    if (rel_elems_in_sets(data->arr_r[l], data->arr_s[x], data->arr_s[y]) == 0) {
        return 0;
    }

    //bool array for marking which elements were seen in relation as a y value
    bool set_arr[data->arr_s[y]->length];
    for (int i = 0; i < data->arr_s[y]->length; i++){
        set_arr[i] = false;
    }

    //trying to find an element equal to each y value of relation
    for (int i = 0; i < data->arr_r[l]->length; i++){
        for (int j = 0; j < data->arr_s[y]->length; j++){
            if (data->arr_r[l]->elem_arr[i].e_2 == data->arr_s[y]->elem_arr[j]){
                set_arr[j] = true;  //mark found element in array
                break;
            }
        }
    }

    //if false value is found, element of set was not used in relation as a y value -> not surjective
    for (int i = 0; i < data->arr_s[y]->length; i++){
        if (!set_arr[i]){
            return -1;
        }
    }
    return 1;
}

/* prints whether or not is a relation surjective */
int rel_surjective(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(3, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]
    int x = set_line(data, arg_arr[1]);  //index of set defining x values
    int y = set_line(data, arg_arr[2]);  //index of set defining y values

    //invalid argument
    if (l == -1 || x == -1 || y == -1){
        return 0;
    }

    //return value of inj is stored in check in order to check it multiple times
    int check = surj(data, l, x, y);
    if (check == 0){
        fprintf(stdout, "%s\n", FALSE);
        return -1;
    }
    if (check == 1){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}

/* prints whether or not is the relation on line [line] bijective */
int rel_bijective(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(3, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]
    int x = set_line(data, arg_arr[1]);  //index of set defining x values
    int y = set_line(data, arg_arr[2]);  //index of set defining y values

    //invalid argument
    if (l == -1 || x == -1 || y == -1){
        return 0;
    }

    int checkinj = inj(data, l, x, y);
    int checksurj = surj(data, l, x, y);
    if(checkinj == 0 || checksurj == 0){
        fprintf(stdout, "%s\n", FALSE);
        return -1;
    }
    if (checkinj == 1 && checksurj == 1){
        fprintf(stdout, "%s\n", TRUE);
        return 1;
    }
    fprintf(stdout, "%s\n", FALSE);
    return -1;
}

/* copies relation to an empty relation */
int rel_copy(rel_t *src, rel_t *dst)
{
    for (int i = 0; i < src->length; i++) {
        if (rel_append(dst, src->elem_arr[i]) == 0) {
            return 0;
        }
    }

    return 1;
}

/* make relation a reflexive closure and add it to data */
int cl_ref(data_t* data, rel_t *closure)
{
    elpair_t temp_pair;
    //check if all universe diagonal elements are in relation with themselves
    for (int i = 0; i < data->uni.length; i++) {
        temp_pair.e_1 = i;
        temp_pair.e_2 = i;

        //try to find (i i) in the relation
        if (isin_rel(closure, temp_pair) == 0) {
            //add it if not found
            if (rel_append(closure, temp_pair) == 0) {
                return 0;
            }
        }
    }

    //add closure to data
    if (data_append_r(data, closure) == 0) {
        return 0;
    }

    return 1;
}

/* prints relation's reflexive closure */
int rel_closure_ref(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]
    //invalid argument
    if (l == -1){
        return 0;
    }

    rel_t *closure = malloc(sizeof(rel_t));
    rel_create(closure, lines);

    if (rel_copy(data->arr_r[l], closure) == 0) {
        rel_destroy(closure);
        free(closure);
        return 0;
    }

    if (cl_ref(data, closure) == 0) {
        rel_destroy(closure);
        free(closure);
        return 0;
    }

    rel_print(data, lines);
    return 1;
}

/* make relation a symmetric closure and add it to data */
int cl_sym(data_t* data, rel_t *closure)
{
    elpair_t temp_pair;
    //for all pairs find a pair they are symmetric with
    for (int i = 0; i < closure->length; i++) {
        //skip diagonal pairs because they are symmetric to themselves
        if (closure->elem_arr[i].e_1 != closure->elem_arr[i].e_2) {
            temp_pair.e_1 = closure->elem_arr[i].e_2;
            temp_pair.e_2 = closure->elem_arr[i].e_1;
            //for i = (a b) find (b a)
            if (isin_rel(closure, temp_pair) == 0) {
                //add it if not found
                if (rel_append(closure, temp_pair) == 0) {
                    return 0;
                }
            }
        }
    }

    //add closure to data
    if (data_append_r(data, closure) == 0) {
        return 0;
    }

    return 1;
}

/* prints relation's symmetric closure */
int rel_closure_sym(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]
    //invalid argument
    if (l == -1){
        return 0;
    }

    rel_t *closure = malloc(sizeof(rel_t));
    rel_create(closure, lines);

    if (rel_copy(data->arr_r[l], closure) == 0) {
        rel_destroy(closure);
        free(closure);
        return 0;
    }

    if (cl_sym(data, closure) == 0) {
        rel_destroy(closure);
        free(closure);
        return 0;
    }

    rel_print(data, lines);
    return 1;
}

/* make relation a transitive closure and add it to data */
int cl_trans(data_t* data, rel_t *closure)
{
    elpair_t temp_pair;
    bool added = true;

    while(added) {
        added = false;
        //check if relation is transitive for each pair
        for (int i = 0; i < closure->length; i++) {
            //don't check diagonal pairs, because they don't generate anything new
            if (closure->elem_arr[i].e_1 != closure->elem_arr[i].e_2) {
                for (int j = 0; j < closure->length; j++) {
                    //if i = (? b) and j = (b ??)
                    if (closure->elem_arr[i].e_2 == closure->elem_arr[j].e_1) {
                        temp_pair.e_1 = closure->elem_arr[i].e_1;
                        temp_pair.e_2 = closure->elem_arr[j].e_2;
                        //if (? ??) not found -> relation is not transitive
                        if (isin_rel(closure, temp_pair) == 0) {
                            //add it if not found
                            if (rel_append(closure, temp_pair) == 0) {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }

    //add closure to data
    if (data_append_r(data, closure) == 0) {
        return 0;
    }

    return 1;
}

/* prints relation's transitive closure */
int rel_closure_trans(data_t* data, int arg_count, int arg_arr[], int lines)
{
    if (com_arg_check(1, arg_count, lines) == 0){
        return 0;
    }

    int l = rel_line(data, arg_arr[0]);  //index of relation on line [line]
    //invalid argument
    if (l == -1){
        return 0;
    }

    rel_t *closure = malloc(sizeof(rel_t));
    rel_create(closure, lines);

    if (rel_copy(data->arr_r[l], closure) == 0) {
        rel_destroy(closure);
        free(closure);
        return 0;
    }

    if (cl_trans(data, closure) == 0) {
        rel_destroy(closure);
        free(closure);
        return 0;
    }

    rel_print(data, lines);
    return 1;
}


int main(int argc, char *argv[])
{
    //check program parameters
    if(check_param(argc) == 0){
        return EXIT_FAILURE;
    }

    data_t data;
    data_create(&data);

    //open file and load its content
    if (file_load(argv, &data) == 0) {
        return EXIT_FAILURE;
    }

    //memory deallocation
    destroy_all(&data, &(data.uni));

    return EXIT_SUCCESS;
}

