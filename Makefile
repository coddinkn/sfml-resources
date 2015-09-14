CXX=g++
CXXFLAGS = -Wall -std=c++11

INCLUDES = -I./include -I./include/ResourceManager
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

vpath %.o ./build
vpath %.h ./include:./include/ResourceManager
vpath %.cpp ./src:./src/ResourceManager

all: build libsfml-res-man.a sfml-res-man-test tidy

.PHONY: build clean tidy

libsfml-res-man.a: ResourceManager.o AnimatedSprite.o SpriteSheet.o
	ar rvs $@ $^

sfml-res-man-test: main.cpp ResourceManager.o AnimatedSprite.o SpriteSheet.o
	$(CXX) $^ $(CXXFLAGS) $(LIBS) $(INCLUDES) -o $@

%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS) $(LIBS) $(INCLUDES)

build:
	if [ ! -d "./build" ]; then mkdir build; fi	

tidy:
	if [ `ls -l *.o 2> /dev/null | wc -l` -ne 0 ]; then mv -f *.o ./build/; fi

clean:
	rm -f libsfml-res-man.a sfml-res-man-test ./build/*.o
