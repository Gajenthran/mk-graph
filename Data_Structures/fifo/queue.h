#ifndef FIFO_H___
#define FIFO_H___

#define SIZE_QUEUE 1024

extern void initQueue(void);
extern void enqueue(int v);
extern int 	dequeue(void);
extern int 	empty(void);
extern void clean(void);


#endif