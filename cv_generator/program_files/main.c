#include "read.h"
#include "filter.h"
#include "format.h"

/*
This program reads the input cv and keyword and requirements, filterets it 
and outputs the new CV as a latex document.
Project done by Ming Hui Sun, David Rasmusen, Mikkel Kaa, Hans Heje
*/

int main(void){
    printf("########  ########   #######        ## ########  ######  ########    ##     ## #### ########    ###\n##     ## ##     ## ##     ##       ## ##       ##    ##    ##       ##     ##  ##     ##      ## ##   \n##     ## ##     ## ##     ##       ## ##       ##          ##       ##     ##  ##     ##     ##   ##  \n########  ########  ##     ##       ## ######   ##          ##       ##     ##  ##     ##    ##     ## \n##        ##   ##   ##     ## ##    ## ##       ##          ##        ##   ##   ##     ##    ######### \n##        ##    ##  ##     ## ##    ## ##       ##    ##    ##         ## ##    ##     ##    ##     ## \n##        ##     ##  #######   ######  ########  ######     ##          ###    ####    ##    ##     ## \n");
    //wordsinsections tells how many words in each paragraph/section. section count is total section numbers.
    int sections_count, *words_in_sections, keyword_count;
    char **keywords, ***sections_out;
    char ***itemiced_sections_ppp;
    int n_itemices, *n_itemiced_content_p; // Number of itemices, Number of items in each itemices
    char *cv_general_info_p;
    int initial_word_count;
    start_read(&keywords, &keyword_count, &itemiced_sections_ppp, &n_itemices, &n_itemiced_content_p, &sections_out, &sections_count, &words_in_sections, &cv_general_info_p, &initial_word_count);
    double *density_of_section = calloc(sections_count,sizeof(double)); //defines a density for each sections/paragraph
    char *cv_filtered_freetext; //makes a dynamic variable to later be malloced to be used to dynamically change length/words in new cv
    bool *included_sections = calloc(sections_count,sizeof(bool)); //defines an array of which paragraphs/sections should be included
    
    remove_personal_pronouns(words_in_sections, sections_count, sections_out);
    calculate_text_density(sections_out, keywords, words_in_sections, sections_count, keyword_count, density_of_section);
    include_section(density_of_section,sections_out,words_in_sections,sections_count,initial_word_count ,included_sections);
    generate_text(included_sections,sections_out,sections_count,words_in_sections,&cv_filtered_freetext);
    
    printf("Filtered free text:");
    printf("\n\n%s\n", cv_filtered_freetext);

    printf("keywords: ");
    for (int i = 0; i < keyword_count; i++) {
        printf("%s, ",keywords[i]);
    }
    
    printf("\n\ninclude? -- Section ID -- Section Density -- Section Text\n");
    for (int i = 0; i < sections_count; i++) {
        printf("\n[%d] (%d): %lf -- ",included_sections[i],i,density_of_section[i]);
    
        for (int j = 0; j < words_in_sections[i]; j++) {
            printf("%s ",sections_out[i][j]);
        }
    }
    run_pdfLaTeX(cv_general_info_p, itemiced_sections_ppp, n_itemices, n_itemiced_content_p, cv_filtered_freetext);

    //free variables
    free(density_of_section);
    free(included_sections);
    free(cv_filtered_freetext); //freeinng variables
    printf("Executed program correctly");
    
    // All rest of code here
    the_ending(keywords, keyword_count, itemiced_sections_ppp, n_itemices, n_itemiced_content_p, sections_out, sections_count, words_in_sections, cv_general_info_p);
    return 0;
}