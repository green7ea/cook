OBJS = $(shell ls *.cpp | sed "s|.cpp|.o|g")
OUTPUT = cook++
CXX = clang++
PKGCFLAGS = $(shell pkg-config --cflags jansson)
CXXFLAGS = -pipe -Wall -Wextra -g -march=native -pedantic -std=c++11 $(PKGCFLAGS)
PKGLIBS = $(shell pkg-config --libs jansson)
LIBS = $(PKGLIBS)

.PHONY: all clean

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT)

clean:
	rm -f $(OBJS)
