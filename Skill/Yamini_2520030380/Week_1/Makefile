CC = gcc
CFLAGS = -Wall -Wextra

TARGET = skill_week1
SOURCE = Practical_Week_1.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run:
	./$(TARGET)

trace:
	strace -f -o trace.txt ./$(TARGET)

clean:
	rm -f $(TARGET) trace.txt
