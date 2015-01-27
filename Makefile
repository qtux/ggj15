# $<	the first dependency
# $@	name of the target
# $+	list of all dependencies
# $^	list of all dependencies (unique elements)
VERSION 	= 1.0
CC			= clang++
#CC			= g++
LIBS		= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CFLAGS		= -Wall -std=c++11
BUILDDIR	= build
BINARYDIR	= bin

# windows variables
#WIN_CC		= g++
WIN_CC		= /usr/bin/i686-w64-mingw32-g++
SFML_PATH	= SFML-2.2
WIN_LIBS	= -s -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsndfile -lopenal32 -ljpeg -lglew -lfreetype -lgdi32 -lopengl32 -lwinmm
WIN_CFLAGS	= -Wall -std=c++11 -DSFML_STATIC -I $(SFML_PATH)/include
FIX_CFLAGS	= -Wall -std=c++11 -DSFML_STATIC -DFIX_TO_STRING -I $(SFML_PATH)/include

# define code paths
SOURCE_PATH	= src
SOURCES		= $(wildcard $(SOURCE_PATH)/*.cpp)
OBJECT		= $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# build all
all: $(OBJECT)
	mkdir -p $(BINARYDIR)
	$(CC) $(LIBS) $^ -o $(BINARYDIR)/run

# build static window release using g++ (via MinGW)
win_static: CFLAGS=$(WIN_CFLAGS)
win_static: CC=$(WIN_CC)
win_static: $(OBJECT)
	mkdir -p $(BINARYDIR)
	$(CC) -L $(SFML_PATH)/lib -o $(BINARYDIR)/run.exe $^ $(WIN_LIBS)

# build static window release using g++ (via MinGW) fixing std::to_string
win_static_with_fix: CFLAGS=$(FIX_CFLAGS)
win_static: CC=$(WIN_CC)
win_static_with_fix: $(OBJECT)
	mkdir -p $(BINARYDIR)
	$(CC) -L $(SFML_PATH)/lib -o $(BINARYDIR)/run.exe $^ $(WIN_LIBS)

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
