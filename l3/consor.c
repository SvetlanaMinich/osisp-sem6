#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "censor.h"

#define MAX_DICT_SIZE 1000

static char *dictionary[MAX_DICT_SIZE];
static int dict_size = 0;

void load_dictionary(const char *dict_file) {
    FILE *fp = fopen(dict_file, "r");
    if (!fp) {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }

    char word[MAX_WORD_LEN];
    while (fscanf(fp, "%s", word) == 1 && dict_size < MAX_DICT_SIZE) {
        dictionary[dict_size] = strdup(word);
        dict_size++;
    }
    fclose(fp);
}

static int is_word_in_dictionary(const char *word) {
    for (int i = 0; i < dict_size; i++) {
        if (strcasecmp(word, dictionary[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void censor_text(FILE *input, FILE *output) {
    char word[MAX_WORD_LEN];
    int c, pos = 0;

    while ((c = fgetc(input)) != EOF) {
        if (isalpha(c) || isdigit(c)) {
            if (pos < MAX_WORD_LEN - 1) {
                word[pos++] = c;
            }
        } else {
            if (pos > 0) {
                word[pos] = '\0';
                if (is_word_in_dictionary(word)) {
                    fputs(CENSOR_TEMPLATE, output);
                } else {
                    fputs(word, output);
                }
                pos = 0;
            }
            fputc(c, output);
        }
    }

    if (pos > 0) {
        word[pos] = '\0';
        if (is_word_in_dictionary(word)) {
            fputs(CENSOR_TEMPLATE, output);
        } else {
            fputs(word, output);
        }
    }
}