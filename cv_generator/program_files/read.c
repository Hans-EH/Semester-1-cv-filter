#include "read.h"

/*the primary function of this file, is to read the keywords and the long cv, while creating the appropiate
data structures, such that it is easier to manipulate the data in the filter.c file.
*/
void start_read(char ***the_keywords_ppp, int *n_keyword_p, char ****cv_long_itemized_pppp, int *n_itemizes_p, int **n_itemized_content_pp, char ****cv_long_sections_pppp, int *n_sections_p, int **n_section_words_pp, char **cv_general_info_pp, int *initial_words_p){

    //char *fnLongCV = malloc(); // In case of user defined file name
    FILE *the_long_CV, *the_keywords;
    char *cv_total_text_p, *keywords_total_text_p;
    the_long_CV = fopen("../long_cv.txt", "r"); // Doing longCV first might have more consecutive space available if pc is low RAM
    the_keywords = fopen("../keywords.txt", "r");
    if(the_long_CV == NULL || the_keywords == NULL){
        // Error, can't open file.
        printf("\nError can't open input file(s)\n"); // temp remove later
        exit(EXIT_FAILURE);
    }
    text_reader(the_long_CV, &cv_total_text_p);
    tag_searcher(cv_total_text_p, cv_long_itemized_pppp, n_itemizes_p, n_itemized_content_pp, cv_long_sections_pppp, n_sections_p, n_section_words_pp, cv_general_info_pp, initial_words_p);
    free(cv_total_text_p);
    text_reader(the_keywords, &keywords_total_text_p);
    worder(keywords_total_text_p, the_keywords_ppp, n_keyword_p);
    free(keywords_total_text_p);
} 

void the_ending(char **the_keywords_pp, int n_keyword, char ***cv_long_Itemiced_ppp, int n_itemices, int *n_itemiced_content_p, char ***cv_long_sections_ppp, int n_sections, int *n_section_words_p, char *cv_general_info_p) {
    int i, j;
    for(i = 0; i < n_keyword; i++){
        free(the_keywords_pp[i]);
    }
    free(the_keywords_pp);
    for(i = 0; i < n_itemices; i++){
        for(j = 0; j < n_itemiced_content_p[i]; j++){
            free(cv_long_Itemiced_ppp[i][j]);
        }
        free(cv_long_Itemiced_ppp[i]);
    }
    free(n_itemiced_content_p);
    free(cv_long_Itemiced_ppp);
    for(i = 0; i < n_sections; i++){
        for(j = 0; j < n_section_words_p[i]; j++){
            free(cv_long_sections_ppp[i][j]);
        }
        free(cv_long_sections_ppp[i]);
    }
    free(n_section_words_p);
    free(cv_long_sections_ppp);
    free(cv_general_info_p);
}

void text_reader(FILE *the_file, char **out_text_pp){
    // Read all text from the file.
    int characters = 0, alottet_array = 300;
    char *the_text_p = malloc(alottet_array * sizeof(char));
    if(the_text_p == NULL){
        // Error can't allocate memmory
        printf("\nError m allo, in text_reader\n"); // Temp remove later
        exit(EXIT_FAILURE);
    } 
    while(1){
        if(characters >= alottet_array){
            // Allocating more memmory when more is required.
            alottet_array += 100;
            the_text_p = (char *) realloc(the_text_p, alottet_array * sizeof(char));
            if(the_text_p == NULL){
                // Error can't reallocate new memmory
                printf("\nError m re allo, in text_reader\n"); // Temp remove later
                exit(EXIT_FAILURE);
            }
        }
        // Reads the next char from the file.
        the_text_p[characters] = fgetc(the_file);

        // End Of File was reatched.
        if(the_text_p[characters] == EOF){
            // Change the_text_p from char array to string.
            the_text_p[characters] = '\0';
            break;
        }
        // Easyer to handel in the rest of the code if there is no '\r'.
        if(the_text_p[characters] != '\r')
            characters++;
    }

    if(characters == 0){
        // Error file is empty
        printf("\nError file is empty, in text_reader\n"); // Temp remove later
        exit(EXIT_FAILURE);
    }
    /* Redo the_text_p to have correct size.
    About realloc() atempts to resize at current memmory location, only if resize fails:
    atempts to find somewhere with the requestet memmory size and moves there. // */
    the_text_p = (char *) realloc(the_text_p, (characters + 1) * sizeof(char));
    if(the_text_p == NULL){
        // Error can't reallocate new memmory, for the final string
        printf("\nError m allo end, in text_reader\n"); // Temp remove later
        exit(EXIT_FAILURE);
    }
    
    // Returning the variables.
    *out_text_pp = the_text_p;

    // All content of the file is in a string, file is no longer needed.
    fclose(the_file);
}

void worder(char *clean_text_p, char ***words_out_ppp, int *n_words_out_p){
    // Seperates a string text into a array of strings containing individual words.
    int n_the_words = 0, n_words_space = 30, current_char = 0, word_start = 0, i;
    char **the_words_pp = malloc(n_words_space * sizeof(char *));
    if(the_words_pp == NULL){
        // Error can't allocate memmory
        printf("\nError m allo, in worder, for words\n"); // Temp remove later
        exit(EXIT_FAILURE);
    }
    while(1){
        // Handeling of " \0" and "\n\0" lead to while(1) rather than while(clean_text_p[current_char] != '\0').
        if(clean_text_p[current_char] == ' ' || clean_text_p[current_char] == '\n' || clean_text_p[current_char] == '\0'){
            if(current_char != word_start){
                if(n_the_words >= n_words_space){
                    // More memmory is needed.
                    n_words_space += 10;
                    the_words_pp = (char **) realloc(the_words_pp, n_words_space * sizeof(char *));
                    if(the_words_pp == NULL){
                        // Error can't reallocate memmory
                        printf("\nError m re allo, in worder\n"); // Temp remove later
                        exit(EXIT_FAILURE);
                    }
                }
                char *one_word_p = malloc((current_char - word_start + 1) * sizeof(char));
                if(one_word_p == NULL){
                    // Error can't allocate memmory
                    printf("\nError m allo, in worder, for a word\n"); // Temp remove later
                    exit(EXIT_FAILURE);
                }
                // Alternative to strncpy(), don't need "&(clean_text_p[word_start])" calculation.
                for(i = word_start; i < current_char; i++){
                    one_word_p[i - word_start] = clean_text_p[i];
                }
                one_word_p[current_char - word_start] = '\0';
                the_words_pp[n_the_words] = one_word_p;
                n_the_words++;
            }
            if(clean_text_p[current_char] == '\0'){
                break;
            }
            // Keeps track of the index value for the start of the next word.
            word_start = current_char + 1;
        }
        current_char++;
    }
    if(n_the_words == 0){
        // Error no words in the given string
        printf("\nError bad worder function call, in read.c\n"); // Temp remove later
        exit(EXIT_FAILURE);
    }
    // Resize the_words_pp to correct size.
    the_words_pp = (char **) realloc(the_words_pp, n_the_words * sizeof(char *));
    if(the_words_pp == NULL){
        // Error can't allocate memmory
        printf("\nError m re allo final, in worder, for words\n"); // Temp remove later
        exit(EXIT_FAILURE);
    }
    // Returning the outputs, do here for less dereferencing.
    *words_out_ppp = the_words_pp;
    *n_words_out_p = n_the_words;
}

void tag_searcher(char *file_clean_text_p, char ****cv_long_itemized_pppp, int *n_itemizes_p, int **n_itemized_content_pp, char ****cv_long_sections_pppp, int *n_sections_p, int **n_section_words_pp, char **cv_general_info_pp, int *word_counter_p){
    // Handels tag splitting of the longCV.
    int current_char = 1, current_marker = -1, n_tags = 1, allocated_tags = 5, i = 0, j, area_length;
    char *info_text_p, **temp_trash_pp;
    // Contains pointers to the tags, in .  Mangler SÆTNING!!!!!
    char **tag_locations_pp = malloc(allocated_tags * sizeof(char *));
    if(tag_locations_pp == NULL){
        exit(EXIT_FAILURE);
    }
    
    tag_locations_pp[0] = file_clean_text_p;
    while(1){
        if(file_clean_text_p[current_char] == '\n'){
            current_marker = current_char;
        } else if(file_clean_text_p[current_char] == '\0' || (file_clean_text_p[current_char] == '#' && current_marker + 1 == current_char)){
            if(n_tags >= allocated_tags){
                allocated_tags += 5;
                tag_locations_pp = (char **) realloc(tag_locations_pp, allocated_tags * sizeof(char *));
                if(tag_locations_pp == NULL){
                    exit(EXIT_FAILURE);
                }
            }
            tag_locations_pp[n_tags] = &(file_clean_text_p[current_char]);
            n_tags++;
            if(file_clean_text_p[current_char] == '\0')
                break;
        }
        current_char++;
    }

    area_length = tag_locations_pp[1] - tag_locations_pp[0];
    tag_locations_pp[1][-1] = '\0';
    info_text_p = (char *) malloc(area_length * sizeof(char));
    if(info_text_p == NULL){
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < area_length; i++){
        info_text_p[i] = tag_locations_pp[0][i];
    }
    *cv_general_info_pp = info_text_p;
    // Gets number of words in the infoText.
    worder(info_text_p, &temp_trash_pp, word_counter_p);
    for(j = 0; j < *word_counter_p; j++){
        free(temp_trash_pp[j]);
    }
    free(temp_trash_pp);

    line_reader_controle(tag_locations_pp, n_tags, cv_long_itemized_pppp, n_itemizes_p, n_itemized_content_pp, cv_long_sections_pppp, n_sections_p, n_section_words_pp, word_counter_p);
    free(tag_locations_pp);
}

void line_reader_controle(char **tags_pp, int n_of_tags, char ****cv_long_itemized_pppp, int *n_itemizes_p, int **n_itemized_content_pp, char ****cv_long_sections_pppp, int *n_sections_p, int **n_section_words_pp, int *n_words_in_start_p){
    int current_itemice = 0, allocated_itemices = 4, i, j, temp_test, n_items, allocated_items, temp_word_counter, temp_line_result;
    char *free_text_variations[32] = {"freetext", "Freetext", "freeText", "FreeText", "freetext.", "Freetext.", "freeText.", "FreeText.",
    "free text", "Free text", "free Text", "Free Text", "free text.", "Free text.", "free Text.", "Free Text.",
    " freetext", " Freetext", " freeText", " FreeText", " freetext.", " Freetext.", " freeText.", " FreeText.",
    " free text", " Free text", " free Text", " Free Text", " free text.", " Free text.", " free Text.", " Free Text."};
    char *temp_first_line_p, *temp_next_line_location_p, **temp_words_pp;
    int *n_items_in_item_p = malloc(allocated_itemices * sizeof(int));
    char ***itemices_ppp = malloc(allocated_itemices * sizeof(char **));
    if(itemices_ppp == NULL || n_items_in_item_p == NULL){
        exit(EXIT_FAILURE);
    }
    for(i = 1; i < n_of_tags - 1; i++){
        if(tags_pp[i + 1][0] != '\0')
            tags_pp[i + 1][-1] = '\0';
        // &(tags_pp[i][1]) remove # from the start of the first line.
        if(line_reader(&(tags_pp[i][1]), &temp_next_line_location_p, &temp_first_line_p) <= 0){
            // Tag is invalid, can't guarantee sufficient information to run the rest of the program.
            exit(EXIT_FAILURE);
        }
        temp_test = 1;
        for(j = 0; j < 32 && temp_test; j++)
            temp_test = strcmp(temp_first_line_p, free_text_variations[j]);
        
        if(temp_test){
            if(current_itemice >= allocated_itemices){
                allocated_itemices += 2;
                itemices_ppp = (char ***) realloc(itemices_ppp, allocated_itemices * sizeof(char **));
                n_items_in_item_p = (int *) realloc(n_items_in_item_p, allocated_itemices * sizeof(int));
                if(itemices_ppp == NULL || n_items_in_item_p == NULL){
                    exit(EXIT_FAILURE);
                }
            }
            worder(temp_next_line_location_p, &temp_words_pp, &temp_word_counter);
            for(j = 0; j < temp_word_counter; j++){
                free(temp_words_pp[j]);
            }
            free(temp_words_pp);
            *n_words_in_start_p += temp_word_counter;

            allocated_items = 5;
            n_items = 1;
            itemices_ppp[current_itemice] = (char **) malloc(allocated_items * sizeof(char *));
            if(itemices_ppp[current_itemice] == NULL){
                exit(EXIT_FAILURE);
            }
            itemices_ppp[current_itemice][0] = temp_first_line_p;
            while(1){
                if(n_items >= allocated_items){
                    allocated_items += 5;
                    itemices_ppp[current_itemice] = (char **) realloc(itemices_ppp[current_itemice], allocated_items * sizeof(char *));
                    if(itemices_ppp[current_itemice] == NULL){
                        exit(EXIT_FAILURE);
                    }
                }
                temp_line_result = line_reader(temp_next_line_location_p, &temp_next_line_location_p, &(itemices_ppp[current_itemice][n_items]));
                if(temp_line_result >= 0){
                    n_items++;
                    if(temp_line_result == 0)
                        break;
                } else if(temp_line_result == -2){
                    break;
                }
            }
            itemices_ppp[current_itemice] = (char **) realloc(itemices_ppp[current_itemice], n_items * sizeof(char *));
            if(n_items == 1 || itemices_ppp[current_itemice] == NULL){
                exit(EXIT_FAILURE);
            }
            n_items_in_item_p[current_itemice] = n_items;
            current_itemice++;
        } else {
            free(temp_first_line_p);
            section_treater(temp_next_line_location_p, cv_long_sections_pppp, n_sections_p, n_section_words_pp);
        }
    }
    if(current_itemice == 0){
        exit(EXIT_FAILURE);
    }
    itemices_ppp = (char ***) realloc(itemices_ppp, current_itemice * sizeof(char **));
    n_items_in_item_p = (int *) realloc(n_items_in_item_p, current_itemice * sizeof(int));
    if(itemices_ppp == NULL || n_items_in_item_p == NULL){
        exit(EXIT_FAILURE);
    }
    *cv_long_itemized_pppp = itemices_ppp;
    *n_itemized_content_pp = n_items_in_item_p;
    *n_itemizes_p = current_itemice;
}

int line_reader(char *the_text_in_p, char **the_next_line_pp, char **the_line_out_pp){
    int the_reader = 0, allocated_chars = 20; //marking = 0, temp_word_counter = 0;
    char *the_line_p = malloc(allocated_chars * sizeof(char));
    if(the_line_p == NULL){
        exit(EXIT_FAILURE);
    }
    while(1){
        if(the_reader >= allocated_chars){
            // Need more memmory.
            allocated_chars += 20;
            the_line_p = (char *) realloc(the_line_p, allocated_chars * sizeof(char));
            if(the_line_p == NULL){
                exit(EXIT_FAILURE);
            }
        }
        if(the_text_in_p[the_reader] == '\n' || the_text_in_p[the_reader] == '\0')
            break;
        the_line_p[the_reader] = the_text_in_p[the_reader];
        the_reader++;
    }
    // Pointer to start of next line.
    *the_next_line_pp = &(the_text_in_p[the_reader + 1]);
    // Something went wrong.
    if(the_reader == 0){
        free(the_line_p);
        if(the_text_in_p[the_reader] == '\0')
            return -2;
        return -1;
    }
    the_line_p[the_reader] = '\0';
    the_line_p = (char *) realloc(the_line_p, (the_reader + 1) * sizeof(char));
    if(the_line_p == NULL){
        exit(EXIT_FAILURE);
    }
    *the_line_out_pp = the_line_p;
    
    if(the_text_in_p[the_reader] == '\0')
        return 0;
    else
        return 1;
}

void section_treater(char *the_free_text_p, char ****the_sections_pppp, int *then_sections_p, int **the_n_word_sections_pp){
    int the_reader = 0, current_marker = -1, n_sections = 1, n_section_allocated = 10, i;
    char **the_sections_temp_pp = malloc(n_section_allocated * sizeof(char *));
    if(the_sections_temp_pp == NULL){
        exit(EXIT_FAILURE);
    }
    the_sections_temp_pp[0] = the_free_text_p;
    while(the_free_text_p[the_reader] != '\0'){
        if(the_reader == current_marker && the_free_text_p[the_reader] == '\n' && the_free_text_p[the_reader + 1] != '\0'){
            if(n_sections >= n_section_allocated){
                n_section_allocated += 10;
                the_sections_temp_pp = (char **) realloc(the_sections_temp_pp, n_section_allocated * sizeof(char *));
                if(the_sections_temp_pp == NULL){
                    exit(EXIT_FAILURE);
                }
            }
            the_free_text_p[the_reader - 1] = '\0';
            the_sections_temp_pp[n_sections] = &(the_free_text_p[the_reader + 1]);
            n_sections++;
        } else if(the_free_text_p[the_reader] == '\n'){
            current_marker = the_reader + 1;
        }
        the_reader++;
    }
    char ***final_sections_ppp = malloc(n_sections * sizeof(char **));
    int *words_in_sections_p = malloc(n_sections * sizeof(int));
    if(final_sections_ppp == NULL || words_in_sections_p == NULL){
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < n_sections; i++){
        worder(the_sections_temp_pp[i], &(final_sections_ppp[i]), &(words_in_sections_p[i]));
    }
    free(the_sections_temp_pp);
    *the_sections_pppp = final_sections_ppp;
    *then_sections_p = n_sections;
    *the_n_word_sections_pp = words_in_sections_p;
}