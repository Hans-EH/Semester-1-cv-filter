#include "format.h"

/*
This program will check the format and edited it accordingly to how a CV should be visualized. 
There are included a LaTeX format to enhance readability for the user and recruiter,
and if there is information about either the education or work experience
then the program will automatically format it self into bullet points text.
A picture of yourselves is required for most of the jobs that are available.
This program will at last print out a pdf-file for the user. 
*/

void output_capitalized(char *capitalized_text) {
    // Two markers that are temporary out of the index
    int marker_index_1 = -1;
    int marker_index_2 = -1;
    
    //Calculating length.
    int length = strlen(capitalized_text);
    // Capitalized the first paragraph for the first letter with toupper
    *capitalized_text = toupper(*capitalized_text);

    // This will capitalize for other scenarios
    for(int i = 1; i < length - 1; i++) { // length are going down with -1 because it must stop at '.'
        if(marker_index_1 == i) {
            if(capitalized_text[i] == ' ') {
                // Capitalize the next letter
                capitalized_text[i+1] = toupper(capitalized_text[i+1]);
            }
            if(capitalized_text[i] == '\\') {
                marker_index_2 = i + 1;
            }
        }
        else if(marker_index_2 == i && capitalized_text[i] == '\\') {
            // Capitalize the next letter
            capitalized_text[i+1] = toupper(capitalized_text[i+1]);
        } 
        else if(capitalized_text[i] == '.' || capitalized_text[i] == '?' || capitalized_text[i] == '!') {
            marker_index_1 = i + 1;
        }
        if(capitalized_text[i] == 'i' && capitalized_text[i-1] == ' ' && capitalized_text[i+1] == ' ') {
            capitalized_text[i] = toupper(capitalized_text[i]);
        }
    }
}

/*
This function will make some modification for the plain text like structure, layout, font etc.
The function should also convert a plain text into a LaTeX format.
*/
void output_general_contents(char *array_general_information, FILE *general_CV) { 
    output_capitalized(array_general_information);
    fprintf(general_CV,"\\begin{center}\n \\begin{minipage}[b]{0.45\\textwidth} \n\\subsection*{General information}\n%s\\newline\n \\end{minipage}\n \\hfill", array_general_information);
}

/* 
Load a picture to the CV. That can be essential for the recruiter.
This will also set the picture at the right side of the CV.
*/
void output_picture(FILE *picture) {
    //Put your picture here after the phrase figures/...
    // This figure can be adjusted, so it will be at the right side of the CV
    fprintf(picture, "\n\\begin{minipage}[b]{4.5cm}\n \\includegraphics[height=4.25cm]{../picture}\n \\end{minipage}\n \\end{center}\n\n");
}

void output_essential_contents(char ***array_essential_contents, int amount_of_itemizes, int *amount_of_items_in_a_itemize, FILE *essenctial_CV) {
    for(int i = 0; i < amount_of_itemizes; i++) {
        // The print is going to start at index 0, because of 'i' is going to start at 0.
        fprintf(essenctial_CV, "\\section*{%s}\n\\begin{itemize}\n", array_essential_contents[i][0]);
        for(int j = 1; j < amount_of_items_in_a_itemize[i]; j++) {
            fprintf(essenctial_CV, "\\item %s\n", array_essential_contents[i][j]);
        }
        fprintf(essenctial_CV, "\\end{itemize}\n");
    }    
}

/*
This function will make some modification for the plain text like structure, layout, font etc.
The function should also convert a plain text into a LaTeX file.
*/
void output_LaTeX_free_text(char *array_free_text, FILE *free_text_CV) {
    output_capitalized(array_free_text);
    // Prints out the free text that has been filtered to the CV.tex file.
    fprintf(free_text_CV,"\n\\section*{Background Information}\n%s\n", array_free_text);
}

void run_pdfLaTeX(char *run_general_info, char ***run_essential_info, int itemizes, int *items_for_G_info_and_essential_info, char *run_free_text) {
    // Create or overwrite a LaTeX file, and check it if file can be created or overwritten.
    FILE *final_CV;
    final_CV = fopen("CV.tex", "w");
    
    if(final_CV == NULL) { 
        printf("Cannot open file\n"); 
        exit(EXIT_FAILURE); 
    }
    output_general_contents(run_general_info, final_CV);
    output_picture(final_CV);
    output_essential_contents(run_essential_info, itemizes, items_for_G_info_and_essential_info, final_CV);
    output_LaTeX_free_text(run_free_text, final_CV);
    fclose(final_CV);
}