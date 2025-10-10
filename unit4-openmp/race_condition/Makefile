CC = gcc
CFLAGS = -g -Wall -fopenmp
LIBS =
TARGET = race_condition
SOURCE = race_condition.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET) <num_threads> <count_to>"

clean:
	rm -f $(TARGET)

.PHONY: all clean
