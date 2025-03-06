CC = gcc
CFLAGS = -Wall -g
TARGET = censor
OBJECTS = main.o censor.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

main.o: main.c censor.h
	$(CC) $(CFLAGS) -c main.c

censor.o: censor.c censor.h
	$(CC) $(CFLAGS) -c censor.c

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running test..."
	@echo "This is a bad word and good text" | ./$(TARGET) -d dict.txt > output.txt
	@cat output.txt
	@echo "\nTest completed."

.PHONY: all clean test