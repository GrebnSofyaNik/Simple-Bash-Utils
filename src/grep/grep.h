#ifndef SRC_GRAP_H_
#define SRC_GRAP_H_
#include <getopt.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define short_options "+e:ivclnhsf:o"

typedef struct grep_flags {
  int printing;
  int sample;  // ожидает шаблон для поиска
  int no_reg;  // игнорирует регистр
  int invert;  // исключает шаблон в поиске
  int numb_coincidence_lines;  // выводит количество совпадающих строк
  int coincidence_files;  // выводит строки без имени файла
  int numbers;            // выводит также номер строки
  int coincidence_lines;  // выводит найденые строки без имени файла
  int noerr;  // не выводит ошибки о несуществующих или нечитаемых файлах
  int f;  // получает регулярные выражения из файла
  int match_o;  // печатает только совпадающие не пустые строки
  int all;
  int more_than_one_file;
  int negation;
} grep_flags;

void parse_options(int argc, char* argv[], char* line, grep_flags* flag);
void grep_do(FILE* file, char* fileline, char* line, grep_flags* flag,
             char* argv[]);
void file_reader_grep(FILE* file, char* line, char* fline);
void readerrr(int argc, char* argv[], FILE* file, char* line, grep_flags* flag);
void read_line(regmatch_t* pmatch, char* fileline);
void print_regex(regex_t* re, char* fileline, grep_flags* flag, char* argv[],
                 int* count, int* countn, size_t nmatch, regmatch_t* pmatch);
int file_exist(FILE* file);

#endif  // SRC_GREP_H
