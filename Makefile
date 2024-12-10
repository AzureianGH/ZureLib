# Define the main source file and ZureLib directory
MAINFILE = main.c
ZURELIB = ./ZureLib

# Collect all .c and .h files from the ZureLib folder and subdirectories
ZURELIBFILES = $(wildcard $(ZURELIB)/**/*.c)
ZURELIBHEADERS = $(wildcard $(ZURELIB)/**/*.h)

# Define object files corresponding to the .c files in ZureLib
ZURELIBOBJ = $(ZURELIBFILES:.c=.o)
ZURELIBOBJ := $(subst $(ZURELIB),$(ZURELIB)/obj,$(ZURELIBOBJ))

# Compiler and linker flags
CC = gcc
#include window GUI library
CFLAGS = -Wall -Wextra -Werror -g -Wno-unused-variable -lgdi32 -luser32
LDFLAGS = -lm -lgdi32 -luser32

# Final output target
TARGET = main

# Default target to build the main executable
all: $(TARGET)

# Link the main executable using the main file and object files
$(TARGET): $(MAINFILE) $(ZURELIBOBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each .c file in ZureLib to object files
$(ZURELIB)/obj/%.o: $(ZURELIB)/%.c
	@mkdir -p $(dir $@)  # Create the full directory path for the object file
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up compiled object files and the main executable
clean:
	rm -f $(ZURELIB)/obj/*.o $(TARGET)
	rm -rf $(ZURELIB)/obj

.PHONY: all clean
