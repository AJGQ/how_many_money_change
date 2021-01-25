CC = gcc
CFLAGS = -O2
LIBS = -lgmp

all: moneych_gmp

moneych_gmp: moneych_gmp.c
		$(CC) $(CFLAGS) -o moneych_gmp moneych_gmp.c $(LIBS)

moneych: moneych.c
		$(CC) $(CFLAGS) -o moneych moneych.c
