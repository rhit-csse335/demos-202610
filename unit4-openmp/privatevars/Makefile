CC = gcc
CFLAGS = -g -Wall -fopenmp
LIBS =
TARGET = privatevars
SOURCE = privatevars.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET)"

clean:
	rm -f $(TARGET)

.PHONY: all clean
