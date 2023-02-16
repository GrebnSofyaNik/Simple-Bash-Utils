#include "grep.h"

int main(int argc, char** argv) {
  FILE* file = NULL;
  grep_flags flag = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char* pattern = malloc(2048 * sizeof(char));
  char* line = malloc(2048 * sizeof(char));
  line[0] = '\0';
  if (argc >= 3) {
    parse_options(argc, argv, line, &flag);
    readerrr(argc, argv, file, line, &flag);
  }
  if (pattern != NULL) {
    free(pattern);
  }
  if (line != NULL) {
    free(line);
  }
  return 0;
}

void parse_options(int argc, char* argv[], char* line, grep_flags* flag) {
  FILE* file = NULL;
  int opt;
  char* fline = malloc(256 * sizeof(char));
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flag->sample = 1;
        flag->all = 1;
        flag->printing = 1;
        strcat(line, optarg);
        strcat(line, "|");
        break;
      case 'i':
        flag->no_reg = 1;
        break;
      case 'v':
        flag->invert = 1;
        flag->negation = REG_NOMATCH;
        break;
      case 'c':
        flag->numb_coincidence_lines = 1;
        break;
      case 'l':
        flag->coincidence_files = 1;
        break;
      case 'n':
        flag->numbers = 1;
        break;
      case 'h':
        flag->coincidence_lines = 1;
        flag->more_than_one_file = 0;
        break;
      case 's':
        fclose(stderr);
        break;
      case 'f':
        flag->sample = 1;
        flag->f = 1;
        file_reader_grep(file, line, fline);
        break;
      case 'o':
        flag->match_o = 1;
        flag->printing = 0;
        break;
      default:
        fprintf(stderr,
                "grep: illegal option -- %c\nusage: grep "
                "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] [-C[num]] "
                "[-e pattern] [-f file] [--binary - files = value] [--color = "
                "when] [--context[= num]] [--directories = action] [--label] "
                "[--line - buffered] [--null] [pattern] [file...]",
                opt);
        break;
    }
  }
  if (flag->sample == 1) {
    line[strlen(line) - 1] = '\0';
  } else {
    strcpy(line, argv[optind]);
    optind++;
  }
  if (fline != NULL) {
    free(fline);
  }
}

void grep_do(FILE* file, char* fileline, char* line, grep_flags* flag,
             char* argv[]) {
  regex_t re;
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  int t;
  int count = 0;
  int countn = 0;
  int cflags = REG_EXTENDED + REG_NEWLINE;
  if (flag->sample) {
    cflags = REG_EXTENDED + REG_NEWLINE;
  }
  if (flag->no_reg == 1) {
    cflags = REG_EXTENDED + REG_ICASE;
  }
  if ((t = regcomp(&re, line, cflags)) != 0) {
    regerror(t, &re, fileline, sizeof(fileline));
    fprintf(stderr, "grep: %s (%s)\n", fileline, line);
    return;
  }
  while (feof(file) == 0 && fgets(fileline, 2048, file) != NULL) {
    countn++;
    print_regex(&re, fileline, flag, argv, &count, &countn, nmatch, pmatch);
    if (flag->coincidence_files == 2) {
      break;
    }
  }
  if (flag->numb_coincidence_lines) {
    if (flag->more_than_one_file) {
      printf("%s:", argv[optind]);
    }
    printf("%d\n", count);
  }
  if (flag->coincidence_files == 2) {
    printf("%s\n", argv[optind]);
    flag->coincidence_files = 1;
  }
  regfree(&re);
}

void file_reader_grep(FILE* file, char* line, char* fline) {
  file = fopen(optarg, "r");
  while (feof(file) == 0 && fgets(fline, 2048, file) != NULL) {
    strcat(line, fline);
    if (fline[(int)strlen(fline) - 1] == '\n' && strlen(fline) != 1) {
      line[(int)strlen(line) - 1] = '\0';
    }
    strcat(line, "|");
  }
  if (file != NULL) {
    fclose(file);
  }
}

void readerrr(int argc, char* argv[], FILE* file, char* line,
              grep_flags* flag) {
  extern int optind;
  if ((flag->invert == 1) && (flag->match_o == 1)) {
    flag->match_o = 0;
    flag->printing = 1;
  }
  if (argc - optind > 1 && flag->coincidence_lines == 0) {
    flag->more_than_one_file = 1;
  }
  char* fileline = malloc(2048 * sizeof(char));
  while (optind < argc) {
    file = fopen(argv[optind], "r");
    if (file_exist(file)) {
      grep_do(file, fileline, line, flag, argv);
    } else {
      fprintf(stderr, "grep: %s: No such files or directory\n", argv[optind]);
    }
    optind++;
  }
  if (fileline != NULL) {
    free(fileline);
  }
  if (file != NULL) {
    fclose(file);
  }
}

void read_line(regmatch_t* pmatch, char* fileline) {
  for (int i = pmatch->rm_so; i < pmatch->rm_eo; i++) {
    printf("%c", fileline[i]);
  }
  printf("\n");
}

void print_regex(regex_t* re, char* fileline, grep_flags* flag, char* argv[],
                 int* count, int* countn, size_t nmatch, regmatch_t* pmatch) {
  if (flag->sample && flag->numbers != 1 && flag->numb_coincidence_lines != 1 &&
      flag->coincidence_files != 1 && flag->match_o != 1) {
    if (regexec(re, fileline, 0, NULL, 0) == flag->negation) {
      if (flag->more_than_one_file) {
        printf("%s:", argv[optind]);
      }
      if (flag->printing == 1 && flag->match_o != 1) {
        fputs(fileline, stdout);
        if (fileline[(int)strlen(fileline) - 1] != '\n') {
          printf("\n");
        }
      }
    }
  }
  if (flag->numb_coincidence_lines) {
    if (regexec(re, fileline, 0, NULL, 0) == flag->negation) {
      (*count)++;
    }
  }
  if (flag->coincidence_files > 0) {
    if (regexec(re, fileline, 0, NULL, 0) == flag->negation) {
      flag->coincidence_files = 2;
    }
  }
  if (flag->match_o && flag->numb_coincidence_lines != 1 &&
      flag->coincidence_files == 0) {
    if (regexec(re, fileline, 0, NULL, 0) == flag->negation) {
      if (flag->more_than_one_file) {
        printf("%s:", argv[optind]);
      }
      if (flag->numbers) {
        printf("%d:", *countn);
      }
      while (regexec(re, fileline, nmatch, pmatch, 0) == 0) {
        read_line(pmatch, fileline);
        fileline += pmatch->rm_eo;
      }
    }
  }
  if (flag->numbers && flag->numb_coincidence_lines != 1 &&
      flag->coincidence_files != 1) {
    if (regexec(re, fileline, 0, NULL, 0) == flag->negation) {
      if (flag->coincidence_files != 2) {
        if (flag->more_than_one_file) {
          printf("%s:", argv[optind]);
        }
        printf("%d:", *countn);
        if (flag->printing == 1 || flag->match_o == 1) {
          fputs(fileline, stdout);
          if (fileline[(int)strlen(fileline) - 1] != '\n') {
            printf("\n");
          }
        }
      }
    }
  }
  if (flag->printing == 1 && flag->match_o != 1 && flag->sample != 1 &&
      flag->numb_coincidence_lines != 1 && flag->f != 1 &&
      flag->coincidence_files == 0 && flag->numbers != 1) {
    if (regexec(re, fileline, 0, NULL, 0) == flag->negation) {
      if (flag->more_than_one_file) {
        printf("%s:", argv[optind]);
      }
      fputs(fileline, stdout);
      if (fileline[(int)strlen(fileline) - 1] != '\n') {
        printf("\n");
      }
    }
  }
}

int file_exist(FILE* file) {
  int r = 0;
  if (file != NULL) {
    r = 1;
  }
  return r;
}