#ifndef SRC_CAT_H_
#define SRC_CAT_H_
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#define short_options "+vbeEnstT"

typedef struct cat_flags {
  int numb_n;
  int number;
  int squeeze_blank;
  int eeee;
  int tab;
  int show_all;
} cat_flags;

int parse_options(cat_flags *flags, int argc, char **argv);
void cat_f(char **argv, cat_flags *flags);
void numbers(char smb, cat_flags *flags, int *numb_l);
void eee(cat_flags *flags);
void print_nonprinting(char symb);

#endif  // SRC_CAT_H
