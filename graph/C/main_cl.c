/*!
 * \file main_cl.c
 * \brief 
 * \author PANCHALINGAMOORTHY Gajenthran
 * \date
 */
#include "graph.h"

extern driver_t cl_driver;

int main() {
	char * filename = "files/maze.txt";
    init_graph(&cl_driver, filename);
    return 0;
}


