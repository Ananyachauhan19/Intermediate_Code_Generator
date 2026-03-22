# Makefile for Intermediate Code Generator
# Compiler Design Project

CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = icg
SRC_DIR = src/compiler
OBJ_DIR = build
SOURCES = \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/utils.cpp \
	$(SRC_DIR)/symbol_table.cpp \
	$(SRC_DIR)/infix_postfix.cpp \
	$(SRC_DIR)/tac_generator.cpp \
	$(SRC_DIR)/optimizer.cpp \
	$(SRC_DIR)/display.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
HEADERS = \
	$(SRC_DIR)/structures.h \
	$(SRC_DIR)/utils.h \
	$(SRC_DIR)/symbol_table.h \
	$(SRC_DIR)/infix_postfix.h \
	$(SRC_DIR)/tac_generator.h \
	$(SRC_DIR)/optimizer.h \
	$(SRC_DIR)/display.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts

clean:
	rm -f $(OBJECTS) $(TARGET) icg.exe *.o
	@echo "Clean complete!"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Display project structure
info:
	@echo "Project Structure:"
	@echo "=================="
	@echo "Headers: $(HEADERS)"
	@echo "Sources: $(SOURCES)"
	@echo "Target:  $(TARGET)"

.PHONY: all clean run info
