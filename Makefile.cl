CC=gcc
CFLAGS=-O2 
LINK=-O2
SRC=graph.c cl_driver.c main_cl.c list.c parser.c dijkstra.c fstack.c mat.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

graphcl:	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o graphcl
