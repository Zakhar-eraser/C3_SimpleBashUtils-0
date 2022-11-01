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
    post_interpreter post_inter,
    line_counter line_c) {
    unsigned char p_ch;
    unsigned char out_str[5];
    size_t lines = 0;
    while (!feof(file)) {
        unsigned char c_ch = fgetc(file);
        post_inter(out_str, c_ch, &lines);
        printf("%s", out_str);
    }
}

void any_line_counter(size_t *counter, char p_ch, char c_ch) {
    (void)p_ch;
    (void)c_ch;
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
        char cs[2] = {symb + 64, 0};
        strcpy(out_str, strcat("^", cs));
    } else if (symb == 127) {
        strcpy(out_str, "^?");
    } else if (symb > 127 && symb < 160) {
        char str[3];
        char cs[2] = {symb - 64, 0};
        interpret_symbol(str, cs);
        strcpy(out_str, strcat("M-", str));
    } else {
        char cs[2] = {symb, 0};
        strcpy(out_str, cs);
    }
}

void passthrough(char *out_str, char c) {
    char cs[2] = {c, 0};
    strcpy(out_str, c);
}

//void replace(char *str, char req, char rep) {
//    for (char *c = str; c != 0; c++) {
//        if (*c == req) {
//            *c = rep;
//        }
//    }
//}
