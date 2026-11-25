CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
TARGET = labledger

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

