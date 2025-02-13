# Define the main source file and ZureLib directory
MAINFILE = maine.c
ZURELIB = ./ZureLib

# Collect all .c and .h files from the ZureLib folder and subdirectories
ZURELIBFILES = $(wildcard $(ZURELIB)/**/*.c)
ZURELIBHEADERS = $(wildcard $(ZURELIB)/**/*.h)

# Define object files corresponding to the .c files in ZureLib
ZURELIBOBJ = $(ZURELIBFILES:.c=.o)
ZURELIBOBJ := $(subst $(ZURELIB),$(ZURELIB)/obj,$(ZURELIBOBJ))

# Compiler and linker flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Wno-unused-variable -I$(ZURELIB) -DBUILD_DLL
LDFLAGS = -lm -lgdi32 -luser32
DLLFLAGS = -shared -fPIC

# Output directories for DLL and object files
BUILD_DIR = ./build
LIBRARY_NAME = zurelib.dll
IMPORT_LIB = zurelib.lib
TARGET = $(BUILD_DIR)/main.exe

# Default target to build the DLL and executable
all: $(TARGET) $(BUILD_DIR)/$(LIBRARY_NAME)

# Link the main executable using the main file and object files
$(TARGET): $(MAINFILE) $(BUILD_DIR)/$(LIBRARY_NAME)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each .c file in ZureLib to object files
$(ZURELIB)/obj/%.o: $(ZURELIB)/%.c
	@mkdir -p $(dir $@)  # Create the full directory path for the object file
	$(CC) $(CFLAGS) -c -o $@ $<

# Build the DLL from the object files
$(BUILD_DIR)/$(LIBRARY_NAME): $(ZURELIBOBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(DLLFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up compiled object files, DLL, import library, and the main executable
clean:
	rm -f $(ZURELIB)/obj/*.o $(TARGET) $(BUILD_DIR)/$(LIBRARY_NAME) $(BUILD_DIR)/$(IMPORT_LIB)
	rm -rf $(ZURELIB)/obj

# Run the main executable
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
