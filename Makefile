CXX=g++
CXXFLAGS = -Wall -std=c++11

INCLUDES = -Iinclude -Iinclude/ResourceManager
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

vpath %.h include:include/ResourceManager
vpath %.cpp src:src/ResourceManager

all: sfml-res-man-test

.PHONY: clean

sfml-res-man-test: main.cpp ResourceManager.o AnimatedSprite.o ManagedSprite.o
	$(CXX) $^ $(CXXFLAGS) $(LIBS) $(INCLUDES) -o $@

%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS) $(LIBS) $(INCLUDES)

clean:
	rm -f sfml-res-man-test *.o
