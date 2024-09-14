CC = gcc
CFLAGS = -c
OBJS = main.o calc.o stack.o queue.o
TARGET = calculator

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lm

main.o: main.c
	$(CC) $(CFLAGS) -o main.o main.c

calc.o: calc.c
	$(CC) $(CFLAGS) -o calc.o calc.c

stack.o: stack.c
	$(CC) $(CFLAGS) -o stack.o stack.c

queue.o: queue.c
	$(CC) $(CFLAGS) -o queue.o queue.c

clean:
	rm -f $(OBJS) $(TARGET)
