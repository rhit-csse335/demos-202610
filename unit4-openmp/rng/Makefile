CC = gcc
CFLAGS = -g -Wall -fopenmp
LIBS = -lm
TARGET = rng_demo
SOURCE = rng_demo.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET) <num_threads> <samples_per_thread>"

clean:
	rm -f $(TARGET)

.PHONY: all clean
