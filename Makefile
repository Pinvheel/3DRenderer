# Compiler to use
CXX := g++

# Compiler flags
CXXFLAGS := -Iinclude -Wall -Wextra -pedantic -std=c++17

# Libraries to link
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Source files (all .cpp files in src/)
SRC := $(wildcard src/*.cpp)

# Object files (replace .cpp with .o)
OBJ := $(SRC:src/%.cpp=build/%.o)

# Output binary name
TARGET := main

# Default target (built when you just run "make")
all: $(TARGET)

# Link step: build the final executable from object files
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

# Compilation step: build .o files from .cpp files
# $< = first prerequisite (the .cpp file)
# $@ = target (the .o file)
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
build:
	mkdir -p build

# Run cpplint on all headers and sources
lint:
	cpplint src/*.cpp include/*.hpp

# "make clean" will delete build artifacts
clean:
	rm -rf build $(TARGET)

# Declare these names as "phony" so make doesn't confuse them with files
.PHONY: all clean lint