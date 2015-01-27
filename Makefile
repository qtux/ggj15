# $<	the first dependency
# $@	name of the target
# $+	list of all dependencies
# $^	list of all dependencies (unique elements)
VERSION 	= 1.0
CC			= clang++
#CC			= g++
LIBS		= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
WIN_LIBS	= -static-libgcc -lsfml-audio-s -lsndfile -lopenal32 -lsfml-graphics-s -lfreetype -lglew -ljpeg -lsfml-window-s -lopengl32 -lgdi32 -lsfml-system-s -lwinmm
CFLAGS		= -Wall -std=c++11
WIN_CFLAGS	= -Wall -std=c++11 -DSFML_STATIC -ISFML-2.2/include -LSFML-2.2/lib
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

# build static window release using MinGW
win_static:
	mkdir -p $(BINARYDIR)
	g++ $(WIN_CFLAGS) $(WIN_LIBS) $(SOURCES) -o $(BINARYDIR)/run.exe

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
