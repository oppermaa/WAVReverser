# Compiler
CC = gcc

# Source files
SRCS = main.c file_lib.c wav.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = wavReverser

# Default rule
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Compile .c files to .o
%.o: %.c
	$(CC) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
