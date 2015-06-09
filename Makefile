CXX=g++
CXXFLAGS = -Wall -std=c++11

INCLUDES = -I./include -I./include/ResourceManager
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

vpath %.o ./build
vpath %.h ./include:./include/ResourceManager
vpath %.cpp ./src:./src/ResourceManager

all: sfml-res-man-test

.PHONY: clean

sfml-res-man-test: main.cpp ResourceManager.o AnimatedSprite.o ManagedSprite.o
	$(CXX) $^ $(CXXFLAGS) $(LIBS) $(INCLUDES) -o $@
	rm -f *.o

%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS) $(LIBS) $(INCLUDES)
	if [ ! -d "./build" ]; then mkdir build; fi	
	cp $@ ./build

clean:
	rm -f sfml-res-man-test ./build/*.o
