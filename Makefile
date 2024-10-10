# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Directories
SRC_DIR = src
LIB_DIR = libs
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name
EXECUTABLE = lexer

# Include directory
INCLUDES = -I$(LIB_DIR)

# Main target
all: $(BUILD_DIR) $(EXECUTABLE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run target
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean target
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Phony targets
.PHONY: all clean run
