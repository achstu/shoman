CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++20


main: main.o state.o move.o tile.o
	$(CXX) main.o state.o move.o tile.o -o main


main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

state.o: state.cpp state.h
	$(CXX) $(CXXFLAGS) -c state.cpp

move.o: move.h move.cpp
	$(CXX) $(CXXFLAGS) -c move.cpp

tile.o: tile.h tile.cpp
	$(CXX) $(CXXFLAGS) -c tile.cpp 
