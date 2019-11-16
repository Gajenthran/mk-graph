CC=gcc
CFLAGS=-Wall -O2 `sdl2-config --cflags`
LINK=-O2 `sdl2-config --libs` -lSDL2_image -lSDL2_ttf
SRC=pcc.c fstack.c list.c parser.c sdl_driver.c dijkstra.c cl_driver.c
OBJ=$(SRC:.c=.o)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< 

pcc: 	$(OBJ)
	$(CC) $(LINK) $(OBJ) -o $@

clean:
	rm *.o pcc