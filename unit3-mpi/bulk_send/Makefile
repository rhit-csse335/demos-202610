# Makefile for Pipeline Performance Demo
# For parallel computing class demonstration

CC = mpicc
CFLAGS = -g -Wall -O2
LIBS = 
TARGET = bulk_send
SOURCE = bulk_send.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"

clean:
	rm -f $(TARGET)

.PHONY: all clean
