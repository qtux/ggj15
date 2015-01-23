# $<	the first dependency
# $@	name of the target
# $+	list of all dependencies
# $^	list of all dependencies (unique elements)
VERSION 	= 1.0
CC			= clang++
#CC			= g++
LIBS		= -lsfml-graphics -lsfml-window -lsfml-system
CFLAGS		= -Wall -std=c++11
BUILDDIR	= build
BINARYDIR	= bin

# define code paths
SOURCE_PATH	= src
SOURCES		= $(wildcard $(SOURCE_PATH)/*.cpp)
OBJECT		= $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# build all
all: $(OBJECT)
	mkdir -p $(BINARYDIR)
	$(CC) $(LIBS) $^ -o $(BINARYDIR)/run

# create object files in BUILDIR
$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

# remove object and binary files
clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(BINARYDIR)
