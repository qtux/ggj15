# $<	the first dependency
# $@	name of the target
# $+	list of all dependencies
# $^	list of all dependencies (unique elements)
VERSION 	= 1.0
CC			= clang++
#CC			= g++
LIBS		= -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
CFLAGS		= -Wall -std=c++11 -O2
BUILDDIR	= build
BINARYDIR	= bin

# windows crosscompiler variables
WIN_CC		= /usr/bin/i686-w64-mingw32-g++
SFML_PATH	= SFML-2.2

# define code paths
SOURCE_PATH	= src
SOURCES		= $(wildcard $(SOURCE_PATH)/*.cpp)
OBJECT		= $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# build all
all: $(OBJECT)
	mkdir -p $(BINARYDIR)
	$(CC) -o $(BINARYDIR)/run $^ $(LIBS)

# build window release using g++ (via MinGW)
win: CC		=	$(WIN_CC)
win: CFLAGS	:=	$(CFLAGS) -I $(SFML_PATH)/include
win: LIBS	:=	-L $(SFML_PATH)/lib $(LIBS)
win: all

# build window release using g++ (via MinGW) fixing std::to_string
win_fix: CC		=	$(WIN_CC)
win_fix: CFLAGS	:=	$(CFLAGS) -I $(SFML_PATH)/include -DFIX_TO_STRING
win_fix: LIBS	:=	-L $(SFML_PATH)/lib $(LIBS)
win_fix: all

# build static window release using g++ (via MinGW) fixing std::to_string

# create object files in BUILDIR
$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

# remove object and binary files
clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(BINARYDIR)

png:
	cd img && mogrify -layers merge -format png *.xcf

ogg:
	cd sound && for input in *.wav; do if [ ! -f $${input%.*}.ogg ]; then ffmpeg -i $$input -acodec libvorbis $${input%.*}.ogg; fi done
