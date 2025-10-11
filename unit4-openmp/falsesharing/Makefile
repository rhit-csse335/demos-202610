CC = gcc
CFLAGS = -g -Wall -fopenmp -O2
LIBS =
TARGET = falsesharing
SOURCE = falsesharing.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET) [num_threads] [iterations]"

clean:
	rm -f $(TARGET)

.PHONY: all clean
