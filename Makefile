CC = gcc
CFLAGS = -g -flto #-Os
LIBS = -lgmp

all: moneych_mat moneych_gmp moneych

moneych_mat: moneych_mat.c
		$(CC) $(CFLAGS) -o moneych_mat moneych_mat.c $(LIBS)

moneych_gmp: moneych_gmp.c
		$(CC) $(CFLAGS) -o moneych_gmp moneych_gmp.c $(LIBS)

moneych: moneych.c
		$(CC) $(CFLAGS) -o moneych moneych.c
