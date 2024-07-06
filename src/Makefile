CXX := clang++

# basic flags
CXXFLAGS := -std=c++20

# flags for debugging
CXXFLAGS += -Wall -Wextra -Werror -pedantic

# flags for performance
# CXXFLAGS += -march=native -Ofast


SRCS := vector.cpp action.cpp board.cpp state.cpp move.cpp
SRCS += hash.cpp tt.cpp search.cpp

OBJS := $(SRCS:.cpp=.o)

TARGET := shoman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
