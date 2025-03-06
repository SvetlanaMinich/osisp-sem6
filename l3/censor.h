#ifndef CENSOR_H
#define CENSOR_H

#include <stdio.h>

#define MAX_WORD_LEN 100
#define CENSOR_TEMPLATE "****"

void load_dictionary(const char *dict_file);
void censor_text(FILE *input, FILE *output);

#endif