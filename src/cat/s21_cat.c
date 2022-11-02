#include "s21_cat.h"

int main(int argc, char **argv) {
    int out = 1;
    void (*interpreter)(char *, char) = passthrough;
    if (argc > 1) {
        if (argv[1][0] == '-') {
            char *option = argv[1][1];
        }
    }
}

void printer(
    FILE *file,
    interpreter inter,
    post_interpreter post_inter,
    line_counter line_c) {
    unsigned char p_ch = '\0';
    unsigned char out_str[100];
    size_t lines = 0;
    while (!feof(file)) {
        unsigned char c_ch = fgetc(file);
        line_c(&lines, p_ch, c_ch);
        inter(out_str, c_ch);
        post_inter(out_str, c_ch, &lines, inter);
        printf("%s", out_str);
    }
}

void mark_lines_end(char *out, char c_ch, size_t *count) {
    if ()
}

void number(char *out, char c_ch, size_t *count) {
    if (c_ch == '\n')
        sprintf(out, "\n     %ll  %s", *count, out);
}

void squeeze_blank(char *out, char c_ch, size_t *count) {
    if (*count) {
        inter(out, c_ch);
    } else {
        *count = 0;
        out[0] = '\0';
    }
}

void any_line_counter(size_t *counter, char p_ch, char c_ch) {
    (void)p_ch;
    if ((p_ch == '\0' && c_ch != '\0') || c_ch == '\n')
        *counter++;
}

void non_blank_line_counter(size_t *counter, char p_ch, char c_ch) {
    (void)p_ch;
    if (c_ch != '\n' && c_ch != '\0')
        *counter++;
}

void squeeze_line_counter(size_t *counter, char p_ch, char c_ch) {
    if ((c_ch != '\n' && c_ch != '\0') ||
        (c_ch != '\n' && c_ch != '\0')) {
            *counter++;
        }
}

void interpret_symbol(char *out_str, char symb) {
    if (symb < 32) {
        sprintf(out_str, "^%c", symb + 64);
    } else if (symb == 127) {
        sprintf(out_str, "^?");
    } else if (symb > 127 && symb < 160) {
        char str[3];
        interpret_symbol(str, symb - 64);
        sprintf(out_str, "M-%s", str);
    } else {
        char cs[2] = {symb, 0};
        strcpy(out_str, cs);
    }
}

void passthrough(char *out_str, char c) {
    char cs[2] = {c, 0};
    strcpy(out_str, c);
}
