#include "cat.h"

int main(int argc, char **argv) {
  cat_flags flags = {0, 0, 0, 0, 0, 0};
  int arg_opt_count = 0;
  arg_opt_count = parse_options(&flags, argc, argv);
  if (arg_opt_count != -1) {
    while (optind < argc) {
      cat_f(argv, &flags);
      optind += 1;
    }
  }
  return 0;
}

int parse_options(cat_flags *flags, int argc, char **argv) {
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  int parsing_end_flag = 0;
  int parsing_error_flag = 0;
  int option = 0;
  int index = 0;

  while (parsing_end_flag != 1) {
    option = getopt_long(argc, argv, short_options, long_options, &index);
    if (option == -1) {
      parsing_end_flag = 1;
    } else if (option == 'b') {
      flags->numb_n = 1;
    } else if (option == 'e') {
      flags->eeee = 1;
      flags->show_all = 1;
    } else if (option == 'E') {
      flags->eeee = 1;
    } else if (option == 'n') {
      flags->number = 1;
    } else if (option == 's') {
      flags->squeeze_blank = 1;
    } else if (option == 't') {
      flags->tab = 1;
      flags->show_all = 1;
    } else if (option == 'T') {
      flags->tab = 1;
    } else if (option == 'v') {
      flags->show_all = 1;
    } else {
      parsing_error_flag = -1;
      parsing_end_flag = 1;
    }
  }
  if (flags->numb_n == 1) {
    flags->number = 0;
  }
  return parsing_error_flag;
}

void cat_f(char **argv, cat_flags *flags) {
  char *failname = argv[optind];
  FILE *fp = fopen(failname, "r");
  if (fp != NULL) {
    int numb_l = 1;
    char now_c = fgetc(fp);
    numbers(now_c, flags, &numb_l);
    while (now_c != (char)EOF) {
      char next_c = fgetc(fp);
      if (now_c == '\n') {
        eee(flags);
        numbers(next_c, flags, &numb_l);
        if (next_c == '\n' && flags->squeeze_blank) {
          eee(flags);
          while ((next_c = fgetc(fp)) == '\n') {
          }
          numbers(next_c, flags, &numb_l);
        }
      } else if (now_c == '\t' && flags->tab) {
        printf("^I");
      } else if ((now_c < -96 || now_c < 32 || now_c == 127) &&
                 (flags->show_all)) {
        print_nonprinting(now_c);
      } else {
        printf("%c", now_c);
      }
      now_c = next_c;
    }
    fclose(fp);
  } else {
    perror(failname);
  }
}

void numbers(char smb, cat_flags *flags, int *numb_l) {
  if (smb != (char)EOF && (flags->number || (flags->numb_n && smb != '\n'))) {
    printf("%6d\t", (*numb_l)++);
  }
}

void eee(cat_flags *flags) {
  if (flags->eeee) printf("$");
  printf("\n");
}

void print_nonprinting(char symb) {
  if (symb < -96) {
    printf("M-^%c", symb + 196);
  } else if (symb < 32 && symb != 9 && symb != 10) {
    printf("^%c", symb + 64);
  } else if (symb == 127) {
    printf("^?");
  } else {
    printf("%c", symb);
  }
}