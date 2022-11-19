#include "s21_grep.h"

int main(int argc, char **argv) {
    int out = 0, erroroffset, argind;
    const char *error;
    pcre *re = NULL;
    match_modifiers mods = {.argc = argc, .argv = argv};
    char *pattern = get_pattern(&mods, &argind);
    if (pattern && (argind < argc)) {
        if (argc - argind == 1)
            mods.hide_filenames = 1;
        if ((re = pcre_compile(pattern, mods.pcre_opts,
            &error, &erroroffset, NULL))) {
            for (int i = argind; i < argc; i++) {
                FILE *file;
                if ((file = fopen(argv[i], "r"))) {
                    find_matches_in_file(&mods, file, argv[i], re);
                } else {
                    out = 1;
                    if (!(mods.hide_warnings))
                        printf("No such file: %s", argv[i]);
                }
            }
        } else {
            out = 2;
            printf("Pattern compilation error at %d: %s\n", erroroffset, error);
        }
    } else {
        printf("Usage: s21_grep [options] template [file_name]");
        out = 2;
    }
    if (re)
        pcre_free(re);
    free(pattern);
    return out;
}

void find_matches_in_file(
        match_modifiers *mods,
        FILE *file,
        char *filename,
        pcre *re) {
    size_t lines_counter = 0;
    size_t matched_lines_counter = 0;
    char *line = NULL;
    size_t line_cap = 0;
    ssize_t line_len = 0;
    while ((line_len = getline(&line, &line_cap, file)) > 0) {
        lines_counter++;
        int matched = find_match_in_line(mods, re, line, line_len);
        if ((matched && !(mods->inversion)) || (!matched && mods->inversion)) {
            matched_lines_counter++;
            if (!(mods->only_matches_count) && !(mods->first_match)) {
                if (!(mods->hide_filenames))
                    printf("%s:", filename);
                if (mods->print_line_number)
                    printf("%lu:", lines_counter);
                if (!(mods->all_matches)) {
                    printf("%s", line);
                    if (line[line_len - 1] != '\n')
                        printf("\n");
                }
            }
        }
        free(line);
        line = NULL;
    }
    if (mods->only_matches_count) {
        if (!(mods->hide_filenames))
            printf("%s:", filename);
        printf("%lu\n", matched_lines_counter);
    }
    if (mods->first_match && matched_lines_counter)
        printf("%s\n", filename);
}

int find_match_in_line(
        match_modifiers *mods,
        pcre *re,
        char *str,
        ssize_t str_len) {
    int out = 0;
    const size_t OVECCOUNT = 300;
    int ovector[OVECCOUNT];
    ovector[0] = 0;
    ovector[1] = 1;
    int repeat = mods->all_matches && !(mods->first_match);
    do {
        int opts = 0;
        if (ovector[0] == ovector[1]) {
            if ((ssize_t)(ovector[0]) == str_len) {
                repeat = 0;
            } else {
                opts = PCRE_NOTEMPTY_ATSTART | PCRE_ANCHORED;
            }
        }
        int rc = pcre_exec(
            re, NULL,
            str, str_len,
            0, opts,
            ovector, OVECCOUNT);
        if ((rc == PCRE_ERROR_NOMATCH) && (opts == 0))
            repeat = 0;
        if (rc > 0) {
            out = 1;
            if (mods->all_matches) {
                print_matches(str, ovector, rc);
                int skip = ovector[rc * 3 - 2];
                str += skip;
                str_len -= skip;
            }
        }
    } while(repeat);
    return out;
}

void print_matches(char *subject, int *ovector, int rc) {
    for (int i = 0; i < rc; i++) {
        char *sub_start = subject + ovector[2 * i];
        int sub_len = ovector[2 * i + 1] - ovector[2 * i];
        printf("%.*s\n", sub_len, sub_start);
    }
}

char *get_pattern(match_modifiers *mods, int *argind) {
    int opt, patterned = 0, cont = 1;
    char *options = "e:ivclnhsf:o", *pattern = NULL;
    while (((opt = getopt(mods->argc, mods->argv, options)) != -1) &&
            cont) {
        if (opt == 'e') {
            patterned = 1;
            pattern = extend_pattern(pattern, optarg);
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
            if (!(pattern = extend_pattern_from_file(pattern, optarg)))
                cont = 0;
        } else if (opt == 'o') {
            mods->all_matches = 1;
        } else {
            cont = 0;
            printf("Wrong options\n");
            free(pattern);
            pattern = NULL;
        }
    }
    *argind = optind;
    if (!patterned && (optind < mods->argc - 1)) {
        pattern = extend_pattern(pattern, mods->argv[optind]);
        (*argind)++;
    }
    return pattern;
}

char *extend_pattern(char *old, char *add) {
    char *tmp = (char *)realloc(old, sizeof(char) * (len(old) + len(add)));
    if (tmp) {
        strcat(tmp, add);
        old = tmp;
    }
    return old;
}

char *extend_pattern_from_file(char *old, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        size_t line_cap = 0;
        char *line = NULL;
        ssize_t line_len = getline(&line, &line_cap, file);
        if (line_len > 0) {
            if (line[line_len - 1] == '\n')
                line[line_len - 1] = '\0';
            old = extend_pattern(old, line);
            free(line);
        }
    } else {
        printf("No such file: %s", filename);
        free(old);
        old = NULL;
    }
    return old;
}

size_t len(char *str) {
    size_t l = 0;
    if (str)
        l = strlen(str);
    return l;
}
