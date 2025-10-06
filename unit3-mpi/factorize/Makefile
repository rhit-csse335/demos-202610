# Makefile for Pipeline Performance Demo
# For parallel computing class demonstration

CC = mpicc
CFLAGS = -g -Wall
LIBS = -lm
TARGET = factorize
SOURCE = factorize.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET)"

clean:
	rm -f $(TARGET)

.PHONY: all clean
