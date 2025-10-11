CC = gcc
CFLAGS = -g -Wall -fopenmp
LIBS =
TARGET = fib
SOURCE = fib.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET) [num_threads]"

clean:
	rm -f $(TARGET)

.PHONY: all clean
