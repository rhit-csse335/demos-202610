# Makefile for LU factorization gprof demo
CC = gcc
CFLAGS = -Wall -O2 -pg

TARGET = lu_demo
SOURCE = lu_demo.c

# Default target
all: $(TARGET)

# Build the program with profiling support
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Run the program and generate profiling data
run: $(TARGET)
	./$(TARGET)

# Generate gprof report (requires gmon.out from running the program)
profile: run
	gprof $(TARGET) gmon.out > gprof_report.txt

# Clean up generated files
clean:
	rm -f $(TARGET) gmon.out gprof_report.txt



.PHONY: all run profile clean 