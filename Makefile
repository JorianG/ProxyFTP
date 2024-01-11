# Makefile for ProxyFTP

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = proxy.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = proxy

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and target executable
clean:
	rm -f $(OBJS) $(TARGET)
