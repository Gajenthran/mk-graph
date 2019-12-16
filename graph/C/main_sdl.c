/*!
 * \file main_sdl.c
 * \brief
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date 
 */
#include "graph.h"

extern Driver sdl_driver;

int main() {
	char * filename = "files/maze.txt";
    init_graph(&sdl_driver, filename);
    return 0;
}