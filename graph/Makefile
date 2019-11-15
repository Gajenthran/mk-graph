CC=gcc
CFLAGS=-Wall
SRC=pcc.c fstack.c list.c parser.c djikstra.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

pcc: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o pcc