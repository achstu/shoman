CXX := clang++
CXXFLAGS := -Wall -Wextra -std=c++20

SRCS := main.cpp state.cpp move.cpp board.cpp action.cpp vector.cpp

OBJS := $(SRCS:.cpp=.o)

TARGET := shoman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
