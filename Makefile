
BIN = cpr
SRC = main.c cpr.c
OBJ = $(SRC:.c=.o)
HDR = $(SRC:.c=.h)

CC = gcc
CFLAGS = -Wall

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

%.o: %.c $(HDR) # Note1
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rvf *.o $(BIN)


# Note1: Note that $(HDR) is important here. It forces recompilation if there
# is changes to the headers.
# Behavior can be tested by running the sequence of commands 
# "make, touch cpr.h, make" with and without the $(HDR) entry.
