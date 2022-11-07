#include "s21_cat.h"

int main(int argc, char **argv) {
    int out = 1;
    interpreter inter;
    post_interpreter post_inter;
    line_counter line_c;
    if (argc > 1) {
        int path_str_num;
        if (argv[1][0] == '-') {
            path_str_num = 2;
            out = !((argc > 2) && select_option(argv[1] + 1, &inter,
                &post_inter, &line_c));
        } else {
            path_str_num = 1;
            inter = passthrough;
            post_inter = post_passthrough;
            line_c = no_counter;
            out = 0;
        }
        for (; (!out) && (path_str_num < argc); path_str_num++) {
            FILE *file = fopen(argv[path_str_num], "r");
            if (file) {
                printer(file, inter, post_inter, line_c);
            } else {
                out = 1;
            }
        }
    }
    if (out)
        printf("n/a");
    return out;
}

char select_option(char *option, interpreter *inter,
    post_interpreter *post_inter,
    line_counter *line_c) {
    char out = 1;
    if (!strcmp(option, NUMBER_NONBLANK) ||
        !strcmp(option, NUMBER_NONBLANK_GNU)) {
        *inter = passthrough;
        *post_inter = number;
        *line_c = non_blank_line_counter;
    } else if (!strcmp(option, SHOW_ENDS)) {
        *inter = interpret_nonprint;
        *post_inter = mark_lines_end;
        *line_c = any_line_counter;
    } else if (!strcmp(option, SHOW_ENDS_GNU))  {
        *inter = passthrough;
        *post_inter = mark_lines_end;
        *line_c = any_line_counter;
    } else if (!strcmp(option, NUMBER) ||
               !strcmp(option, NUMBER_GNU)) {
        *inter = passthrough;
        *post_inter = number;
        *line_c = any_line_counter;
    } else if (!strcmp(option, SQUEEZE_BLANK) ||
               !strcmp(option, SQUEEZE_BLANK_GNU)) {
        *inter = passthrough;
        *post_inter = squeeze_blank;
        *line_c = non_blank_line_counter;
    } else {
        out = 0;
    }
    return out;
}

void printer(
    FILE *file,
    interpreter inter,
    post_interpreter post_inter,
    line_counter line_c) {
    char p_ch = '\0';
    char out_str[100];
    size_t lines = 0;
    char countered = 0;
    while (1) {
        char c_ch = fgetc(file);
        if (!feof(file)) {
            countered = line_c(&lines, p_ch, c_ch);
            inter(out_str, c_ch, 0);
            post_inter(out_str, countered, &lines);
            countered = 0;
            printf("%s", out_str);
            p_ch = c_ch;
        } else {
            break;
        }
    }
}

void mark_lines_end(char *out, char changed, size_t *count) {
    (void)count;
    if (changed || out[0] == '\0') {
        sprintf(out, "$\n");
    }
}

void number(char *out, char changed, size_t *count) {
    if (!(*count)) {
        char tmp[5];
        strcpy(tmp, out);
        (*count)++;
        sprintf(out, "     1  %s", tmp);
    } else if (changed) {
        sprintf(out, "\n     %ld  ", *count);
    }
}

void squeeze_blank(char *out, char changed, size_t *count) {
    (void)count;
    if (!changed && (out[0] == '\n'))
        out[0] = '\0';
}

void post_passthrough(char *out, char changed, size_t *count) {
    (void)out;
    (void)changed;
    (void)count;
}

char any_line_counter(size_t *counter, char p_ch, char c_ch) {
    (void)p_ch;
    char changed = 0;
    if (c_ch == '\n') {
        (*counter)++;
        changed = 1;
    }
    return changed;
}

char non_blank_line_counter(size_t *counter, char p_ch, char c_ch) {
    char changed = 0;
    if (c_ch == '\n' && p_ch != '\n') {
        (*counter)++;
        changed = 1;
    }
    return changed;
}

char no_counter(size_t *counter, char p_ch, char c_ch) {
    (void)counter;
    (void)p_ch;
    (void)c_ch;
    return 0;
}

void interpret_nonprint(char *out_str, char symb, char twice) {
    if (twice && (symb == '\t')) {
        strcpy(out_str, "\t");
    } else if (twice && (symb == '\n')) {
        strcpy(out_str, "\n");
    } else if ((symb >= 0) && (symb < 32)) {
        sprintf(out_str, "^%c", (symb + 64));
    } else if (symb == 127) {
        sprintf(out_str, "^?");
    } else if (symb < -96) {
        char str[3];
        interpret_nonprint(str, symb + 128, 1);
        sprintf(out_str, "M-%s", str);
    } else {
        sprintf(out_str, "%c", symb);
    }
}

void passthrough(char *out_str, char c, char twice) {
    (void)twice;
    sprintf(out_str, "%c", c);
}
