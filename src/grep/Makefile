CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.o 
		$(CC) $(LFLAGS) s21_grep.c -o s21_grep
clear:
	rm -rf *.o
