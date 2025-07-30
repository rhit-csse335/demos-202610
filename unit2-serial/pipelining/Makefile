# Makefile for Pipeline Performance Demo
# For parallel computing class demonstration

CC = gcc
CFLAGS = -O1 -Wall -Wextra
LIBS = -lrt
TARGET = pipeline_demo
SOURCE = pipeline_demo.c

# Default target
all: $(TARGET)

# Build the demo with minimal optimization to preserve intended behavior
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET)"

# Alternative build with no optimization (for more dramatic differences)
no-opt: CFLAGS = -O0 -Wall -Wextra
no-opt: $(TARGET)
	@echo "Built $(TARGET) with NO optimization (-O0)"

# Alternative build with higher optimization (to show what compiler does)
optimized: CFLAGS = -O3 -Wall -Wextra
optimized: clean $(TARGET)
	@echo "Built $(TARGET) with full optimization (-O3)"
	@echo "Note: Compiler may optimize away the intended differences!"

# Run the demo
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Show different build options
help:
	@echo "Available targets:"
	@echo "  all       - Build with -O1 (recommended for demo)"
	@echo "  no-opt    - Build with -O0 (more dramatic timing differences)"
	@echo "  optimized - Build with -O3 (shows compiler optimizations)"
	@echo "  run       - Build and run the demo"
	@echo "  clean     - Remove built files"
	@echo "  help      - Show this help message"

.PHONY: all no-opt optimized run clean help
