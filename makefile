# makefile to compile quark

# executable name
EXE = quark

# default cpp compiler
CC = g++

# compiler flags
CFLAGS = -std=c++11 -Wall

# preprocesser flags to include
PREFLAGS = 

# link external libraries
LDLIB = 

# source directory
SRCDIR = src

# source files 
SRC = $(wildcard $(SRCDIR)/*.cpp)

.PHONY: clean

# build quark
$(EXE): $(SRC)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS) $(PREFLAGS) $(LDLIB)
	$(info "Building $(EXE) complete.")

# clean quark
clean: 
	$(RM) $(EXE)
