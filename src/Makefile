SHELL = /bin/sh
CC=g++
FLAGS=-Wall -Wextra -Werror
CFLAGS:=$(FLAGS)
SRC=$(wildcard brick_game/tetris/*.c)
OBJS=$(patsubst %.c,%.o,$(SRC))
TEST_SRC=$(wildcard tests/*.cpp)
TEST_OBJS=$(patsubst %.cpp,%.o,$(TEST_SRC))
GCOV_FLAGS=-coverage
GTEST_FLAGS=-lgtest -pthread -lrt -lm -lsubunit
GCOVR=gcovr -e "tests/" -e ".*h"
GCOVRFLAGS=--html --html-details -o coverage.html
DIR = ~/BrickGames

.PHONY: all clean test gcov_report  install uninstall dvi dist s21_snake.a s21_tetris.a

all: clean install

clean:
	-rm -rf *.o *.gcno *.gcda *.info *.a report/ test tests/*.o tests/*.gcno tests/*.gcda html latex *.txt

test: clean uninstall s21_snake.a
	$(CC) $(CFLAGS) tests/*.cpp -o test  -lm -lgtest -lsubunit -L. -l:s21_snake.a
	-./test

s21_tetris.a:
	gcc $(CFLAGS) $^ -c brick_game/tetris/*.c
	ar rc $@ *.o
	ranlib s21_tetris.a

s21_snake.a:
	$(CC) $(CFLAGS) $^ -c -lstdc++ brick_game/snake/*.cpp
	ar rc $@ *.o
	ranlib s21_snake.a

gcov_report: clean add_coverage s21_snake.a test rm_coverage
	$(GCOVR) $(GCOVRFLAGS)
	open coverage.html
	rm -rf *.gcda *.gcno

add_coverage:
        $(eval CFLAGS:=$(CFLAGS) $(GCOV_FLAGS))
rm_coverage:
		$(eval CFLAGS:=$(FLAGS))

control: clean test
	-valgrind --tool=memcheck --leak-check=yes ./test
	-clang-format -n brick_game/*.h tests/*.cpp brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h brick_game/snake/*.cpp brick_game/snake/*.h gui/desktop/*.cpp gui/desktop/*.cpp
	make clean

fix_clang:
	clang-format -i *.c *.h brick_game/*.h tests/*.cpp brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h brick_game/snake/*.cpp brick_game/snake/*.h gui/desktop/*.cpp gui/desktop/*.cpp
install:clean
	mkdir $(DIR)
	make install_snake_cli
	make clean
	make install_tetris_cli
	make clean
	make install_snake_desktop
	make clean
	make install_tetris_desktop
	make clean
	mv tetris_cli tetris_desktop snake_cli snake_desktop $(DIR)

uninstall:
	-rm -rf $(DIR)

install_snake_desktop: s21_snake.a
	g++ $(CFLAGS) -c ./gui/desktop/s21_desktop_gui.cpp -L. -l:s21_snake.a `pkg-config gtkmm-4.0 --cflags --libs`
	g++ $(CFLAGS) ./gui/desktop/main.cpp *.o -o snake_desktop -L. -l:s21_snake.a `pkg-config gtkmm-4.0 --cflags --libs`

install_tetris_cli: s21_tetris.a
	gcc $(CFLAGS) -c ./gui/cli/s21_cli_frontend.c	-lcurses
	gcc $(CFLAGS) ./gui/cli/main.c *.o -o tetris_cli -L. -l:s21_tetris.a -lcurses

install_snake_cli: s21_snake.a
	g++ $(CFLAGS) -c ./gui/cli/s21_cli_frontend.c	-lcurses
	g++ $(CFLAGS) ./gui/cli/main.c *.o -o snake_cli -L. -l:s21_snake.a -lcurses

install_tetris_desktop: s21_tetris.a
	g++ $(CFLAGS) -c ./gui/desktop/s21_desktop_gui.cpp -L. -l:s21_tetris.a `pkg-config gtkmm-4.0 --cflags --libs`
	g++ $(CFLAGS) ./gui/desktop/main.cpp *.o -o tetris_desktop -L. -l:s21_tetris.a `pkg-config gtkmm-4.0 --cflags --libs`
