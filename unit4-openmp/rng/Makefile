CC = gcc
CFLAGS = -g -Wall -fopenmp
LIBS = -lm
TARGETS = rng_demo rng_demo_safe

# Default target
all: $(TARGETS)

rng_demo: rng_demo.c
	$(CC) $(CFLAGS) -o rng_demo rng_demo.c $(LIBS)
	@echo "Built rng_demo with flags: $(CFLAGS)"

rng_demo_safe: rng_demo_safe.c
	$(CC) $(CFLAGS) -o rng_demo_safe rng_demo_safe.c $(LIBS)
	@echo "Built rng_demo_safe with flags: $(CFLAGS)"

clean:
	rm -f $(TARGETS)

.PHONY: all clean
