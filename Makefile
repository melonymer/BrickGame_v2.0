CC = gcc
FLAGS = -Wall -Werror -Wextra -lncurses -std=c11 
NAME = tetris

OS := $(shell uname)
SOURCE = brick_game/tetris/*.c gui/cli/front.c
FORTESTSOURCE = brick_game/tetris/*.c
TESTSOURCE = tests/test.c

ifeq ($(OS),Linux)
	OPEN_CMD = open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS),Darwin)
	OPEN_CMD = open
	ADD_LIB = -lcheck 
endif

all: install


install: 
	@$(CC) $(FLAGS) $(SOURCE) -o $(NAME)

uninstall:
	@rm $(NAME)

dvi:
	open readme.md

dist: 
	mkdir tetris_game
	mkdir tetris_game/src
	cp -R brick_game tetris_game/src
	cp -R gui tetris_game/src
	cp -R img tetris_game/src
	cp -R Makefile tetris_game/src
	cp -R readme.md tetris_game/src
	tar cvzf tetris_game.tgz tetris_game/
	rm -rf tetris_game

test: 
	$(CC) $(FLAGS) $(TESTSOURCE) $(FORTESTSOURCE) -o $(NAME) $(ADD_LIB)
	./$(NAME)

gcov_report:
	$(CC) --coverage $(FLAGS) $(TESTSOURCE) $(FORTESTSOURCE) -o $(NAME) $(ADD_LIB)
	./$(NAME)
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

clean:
	@rm -f $(NAME) 
	@rm -f *.gc*
	@rm -f s21_test.info
	@rm -rf report
	@rm -f *.o *.a
	@rm -rf *.dSYM

format:
	clang-format -style=Google -i *.c *.h

check:
	clang-format -style=Google -n *.c *.h

lcov:
	brew install lcov

brew:
	cd
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh

.PHONY: all install uninstall dvi dist test gcov_report clean format check lcov 

