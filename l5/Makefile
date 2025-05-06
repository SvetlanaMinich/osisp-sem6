CC=gcc
CFLAGS=-pthread -O2
TARGET=stress_test

all: $(TARGET)

$(TARGET): stress_test.c
	$(CC) $(CFLAGS) -o $(TARGET) stress_test.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
