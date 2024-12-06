# Compiler
CC = gcc

# Target executable
TARGET = matrix-multiplier

# Source file
SRC = matrix-multiplier.c

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up generated files
clean:
	rm -f $(TARGET)