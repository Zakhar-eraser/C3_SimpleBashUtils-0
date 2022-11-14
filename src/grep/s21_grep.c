#include "s21_grep.h"

int main(int argc, char **argv) {
    int out = 0, pcre_opts;
    char *pattern = get_pattern(argc, argv);
    if (pattern) {

    } else {
        printf("Wrong options");
    }
}

char *get_pattern(int argc, char **argv, int *pcre_opts) {
    int opt;
    char *options = "e:ivclnhsf:o";
    char *pattern = "";
    while ((opt = getopt(argc, argv, options)) != -1) {
        switch (opt) {
        case 'e':
            extend_pattern(pattern, optarg);
            break;
        case 'i':
            *pcre_opts = PCRE_CASELESS;
            break;
        case 
        default:
            out = 1;
            break;
        }
    }
}
