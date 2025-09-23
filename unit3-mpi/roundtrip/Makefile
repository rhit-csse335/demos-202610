CC = mpicc
CFLAGS = -Wall -O2
TARGET = mpi_timing
SOURCE = mpi_timing.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	mpirun -np 2 ./$(TARGET)

.PHONY: clean run
