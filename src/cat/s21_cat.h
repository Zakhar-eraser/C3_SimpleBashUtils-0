#ifndef _SRC_CAT_S21_CAT_H_
#define _SRC_CAT_S21_CAT_H_

#include <string.h>
#include <stdio.h>
#include "common/utils.h"

#define TAB "^I"
#define NL "^J"

#define NUMBER_NONBLANK "b"
#define NUMBER_NONBLANK_GNU "-number-nonblank"

#define SHOW_ENDS "e"
#define SHOW_ENDS_GNU "E"

#define NUMBER "n"
#define NUMBER_GNU "-number"

#define SQUEEZE_BLANK "s"
#define SQUEEZE_BLANK_GNU "-squeeze-blank"

#define SHOW_TABS "t"
#define SHOW_TABS_GNU "T"

void interpret_symbol(char *out_str, char symb);
void passthrough(char *out_str, char c);
void printer(FILE *file,
    void (*sign_interpreter)(char *, char),
    void (*line_counter)(size_t *, char, char));
//void replace(char *str, char req, char rep);
#endif  // _SRC_CAT_S21_CAT_H_
