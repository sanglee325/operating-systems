CC = gcc
CFLAGS = -Wall -g

SOURCE = bitmap.c hash.c list.c testlib.c test_list.c 
TARGET = testlib
OBJ = $(SOURCE:.c=.o)

$(TARGET) : $(OBJ) bitmap.h debug.h hash.h limits.h list.h round.h testlib.h test_list.h
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
		gdb $(TARGET)

clean : 
		rm $(OBJ) $(TARGET)

run:
		./$(TARGET)
