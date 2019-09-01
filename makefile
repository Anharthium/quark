# makefile to compile quark

# executable name
EXE = quark

# default cpp compiler
CC = g++

# compiler flags
CFLAGS = -std=c++11 -Wall

# preprocesser MACROS to include
MACROS = 

# link external libraries
LDLIB = 

# source directory
SRCDIR = src

# header directory
HEADERDIR = include

# header files
HEADER = $(wildcard $(HEADERDIR)/*.h)

# preprocessor flags (-I )
CPPFLAGS = -I$(HEADERDIR) 

# linker flags (-L)
LDFLAGS = 

# source files 
SRC = $(wildcard $(SRCDIR)/*.cpp)

.PHONY: clean

# build quark
$(EXE): $(SRC) $(HEADER)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS) $(CPPFLAGS) $(MACROS) $(LDFLAGS) $(LDLIB)
	$(info "Building $(EXE) complete.")

# clean quark
clean: 
	$(RM) $(EXE)
