#include "s21_grep.h"

int main(int argc, char **argv) {
    int out = 1, erroroffset;
    const char *error;
    pcre *re;
    match_modifiers mods = {.argc = argc, .argv = argv};
    char *pattern = get_pattern(&mods);
    if (pattern && (optind < argc)) {
        if ((re = pcre_compile(pattern, mods.pcre_opts,
            &error, &erroroffset, NULL))) {
            for (int i = optind; i < argc; i++) {
                FILE *file;
                if ((file = fopen(argv[i], "r"))) {

                } else {
                    if (!mods.hide_filenames)
                        printf("No such file: %s\n", argv[i]);
                }
            }
            printf("Pattern compilation error at %d: %s", erroroffset, error);
        }
    } else {
        printf("Wrong options");
    }
    return out;
}

int find_matches(match_modifiers *mods, FILE *file) {

}

char *get_pattern(match_modifiers *mods) {
    int opt;
    char *options = "e:ivclnhsf:o", *pattern = "";
    while ((opt = getopt(mods->argc, mods->argv, options)) != -1) {
        if (opt == 'e') {
            extend_pattern(pattern, optarg);
        } else if (opt == 'i') {
            mods->pcre_opts = PCRE_CASELESS;
        } else if (opt == 'v') {
            mods->inversion = 1;
        } else if (opt == 'c') {
            mods->only_matches_count = 1;
        } else if (opt == 'l') {
            mods->first_match = 1;
        } else if (opt == 'n') {
            mods->print_line_number = 1;
        } else if (opt == 'h') {
            mods->hide_filenames = 1;
        } else if (opt == 's') {
            mods->hide_warnings = 1;
        } else if (opt == 'f') {
            extend_pattern_from_file(pattern, optarg);
        } else if (opt == 'o') {
            mods->all_matches = 1;
        } else {
            free(pattern);
            pattern = NULL;
        }
    }
    return pattern;
}
