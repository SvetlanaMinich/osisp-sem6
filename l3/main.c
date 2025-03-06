#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "censor.h"

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s -d <dictionary_file> [-o <output_file>]\n", prog_name);
    fprintf(stderr, "  -d <dictionary_file> : Path to dictionary file (required)\n");
    fprintf(stderr, "  -o <output_file>    : Optional output file (default: stdout)\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    const char *dict_file = NULL;
    const char *output_file = NULL;
    FILE *output = stdout;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            if (++i < argc) dict_file = argv[i];
            else print_usage(argv[0]);
        } else if (strcmp(argv[i], "-o") == 0) {
            if (++i < argc) output_file = argv[i];
            else print_usage(argv[0]);
        } else {
            print_usage(argv[0]);
        }
    }

    if (!dict_file) {
        print_usage(argv[0]);
    }

    // Load dictionary
    load_dictionary(dict_file);

    // Open output file if specified
    if (output_file) {
        output = fopen(output_file, "w");
        if (!output) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
    }

    // Process input and censor text
    censor_text(stdin, output);

    // Clean up
    if (output != stdout) {
        fclose(output);
    }

    return 0;
}