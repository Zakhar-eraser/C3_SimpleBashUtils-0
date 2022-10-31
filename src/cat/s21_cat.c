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
