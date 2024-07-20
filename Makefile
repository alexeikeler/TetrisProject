# Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
# Code snippets from the lectures where used

.SUFFIXES:
.PRECIOUS: %.o
.PHONY: all compile checkstyle test clean

CXX = clang++ -std=c++17 -g -Wall -Wextra -Wdeprecated -fsanitize=address -I/usr/include/freetype2
MAIN_BINARIES = $(basename $(wildcard *Main.cpp))
TEST_BINARIES = $(basename $(wildcard *Test.cpp))
LIBS = -lncurses
# use the following line if you use the OpenGL-based TerminalManager
#LIBS = -lncurses  -lglfw -lGL -lX11 -lrt -ldl -lfreetype
TESTLIBS = -lgtest -lgtest_main -lpthread
OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cpp %Test.cpp, $(wildcard *.cpp))))


all: compile test #checkstyle

compile: $(MAIN_BINARIES) $(TEST_BINARIES)

checkstyle:
	clang-format --dry-run -Werror *.h *.cpp

test: $(TEST_BINARIES)
	for T in $(TEST_BINARIES); do ./$$T || exit; done

%.o: %.cpp *.h
	$(CXX) -c $<

%Main: %Main.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS) $(TESTLIBS)

clean:
	rm -f *Main
	rm -f *Test
	rm -f *.o
	rm -fr .vscode

format:
	clang-format -i *.cpp *.h
