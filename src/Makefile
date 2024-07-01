CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20

SRCS := state.cpp move.cpp board.cpp action.cpp vector.cpp minimax.cpp

OBJS := $(SRCS:.cpp=.o)

TARGET := shoman

all: $(TARGET)

$(TARGET): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) $(OBJS) main.o -o $(TARGET)

run: $(TARGET)
	./shoman < ../test/initial.txt

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

perft: $(OBJS) perft.o
	$(CXX) $(CXXFLAGS) $(OBJS) perft.o -o perft

clean:
	rm -f $(OBJS)
