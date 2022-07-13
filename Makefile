#Settings
EXE := fastconverter.exe
LIB := fastconverter.so
BUILD_DIR = build
SRC_DIR := src
INCLUDE_DIR := include
CC := g++
LDFLAGS := -lm
CFLAGS = -Wall -O3 -std=c++11 -I$(INCLUDE_DIR)

all: $(BUILD_DIR)/$(EXE)

shared: $(BUILD_DIR)/$(LIB)
shared: CFLAGS += -fPIC #Add fPIC also for compiler

#Build Executable
$(BUILD_DIR)/$(EXE): $(BUILD_DIR)/FastConverter.o $(BUILD_DIR)/Converter.o $(BUILD_DIR)/Model.o
	$(CC) $(LDFLAGS) -o $@ $^

#Build Shared library
$(BUILD_DIR)/$(LIB): $(BUILD_DIR)/Model.o $(BUILD_DIR)/Converter.o
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $^

#Compile the source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*

.PHONY: all clean

#END
