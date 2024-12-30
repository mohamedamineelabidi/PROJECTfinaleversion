# Makefile
CC = gcc
CFLAGS = -Wall -mwindows
LDFLAGS = -lcomctl32

SRCS = main.c medicine.c gui.c sales.c
OBJS = $(SRCS:.c=.o)
TARGET = pharmacy.exe

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	del *.o $(TARGET)