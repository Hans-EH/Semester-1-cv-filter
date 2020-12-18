#ifndef P1_SRC_FILTER_H
#define P1_SRC_FILTER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // Functions package
#include <strings.h> // Format package

typedef struct {
    double doubleVal; //density array
    int intVal; //value to be inserted into cv, to declare which to add first.
} Tuple;//defines the tuple as (x,y), where x is double val, and y is intval

#define MAX_WORDS 475 //max words in the CV
#define PP_AMOUNT 62 //amount of personal pronouns.

void include_section();
bool is_word_match();
int calculate_section_weight();
void calculate_section_density();
void calculate_text_density();
int cmp_tuples();
void generate_text();
void remove_punctuation();
void remove_duplicate();
void remove_personal_pronouns();
int levenshtein();

#endif  // P1_SRC_FILTER_H
