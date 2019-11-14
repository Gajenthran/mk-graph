#include<stdio.h>
// #include<conio.h>
#define INFINITY 9999
#define MAX 6
 
void dijkstra(int G[MAX][MAX],int n,int startnode);
 
int main()
{
	int G[MAX][MAX] = {
		{0, 1, 3, 0, 0, 0},
		{0, 0, 1, 8, 0, 0},
		{0, 0, 0, 3, 2, 2},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 2, 2, 0},
		{0, 0, 0, 0, 1, 0}
	};

	int n = MAX, u = 0;
	dijkstra(G,n,u);
	
	return 0;
}

/* void dijkstra(vec_t v, int dep) {
	int i, ** cost = (int **)malloc(v.nbn * sizeof(*cost));
	assert(cost);
	int nbs = calculer_nb_succ(v), dist[v.nbn], pred[v.nbn];
	int visited[v.nbn], count, min_d, next_n, i, j;
	for(i = 0; i < v.nbn; i++) {
		cost[i] = (int *)malloc(nbs[i] * sizeof(*cost[i]));
		assert(cost[i]);
	}

	for(i = 0; i < v.nbn; i++) {
    lis_t l = v.n[i].l;
    while(l) {
    	j = 0;
    	if(l->an->in == dep) {
    		cost[i][j] = 1000;
    	} else {
    		cost[i][j] = l->an->in;
    	}
    	j++;
    }
	}

	for(i = 0; i < v.nbn; i++) {
		dist[i] = cost[dep][i];
		pred[i] = dep;
		visited[i] = 0;
	}

	dist[dep] = 0;
	visited[dep] = 1;
	count = 1;

	while(count < v.nbn - 1) {
		min_d = 1000;

		for(i = 0; i < v.nbn; i++) {
			if(dist[i] < min_d && !visited[i]) {
				min_d = dist[i];
				next_n = i;
			}
		}

		visited[next_n] = 1;
		for(i = 0; i < v.nbn; i++) {
			if(!visited[i]) {
				if(min_d + cost[next_n][i] < dist[i]) {
					dist[i] = min_d + cost[next_n][i];
					pred[i] = next_n;
				}
			}
		}
		count++;
	}

	for(i = 0; i < v.nbn; i++) {
		if(i != dep) {
			printf("\nDistance of node %d = %d", i, dist[i]);
			printf("\nPath = %d",i);

			j = i;
			do {
				j = pred[j];
				printf(" <- %d ", j);
			} while(j != dep);
		}
	}
	printf("\n");
} */

void dijkstra(int G[MAX][MAX], int n, int startnode) {
	int cost[MAX][MAX], distance[MAX], pred[MAX];
	int visited[MAX], count, mindistance, nextnode, i, j;

	// pred[] stores the predecessor of each node
	// count gives the number of nodes seen so far
	// create the cost matrix
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			if(G[i][j] == 0)
				cost[i][j] = INFINITY;
			else
				cost[i][j] = G[i][j];
		}
	}

	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			printf("- %d\n", cost[i][j]);
		}
		printf("\n");
	}

	// initialize pred[],distance[] and visited[]
	for(i = 0; i < n; i++) {
		distance[i] = cost[startnode][i];
		pred[i] = startnode;
		visited[i] = 0;
	}

	distance[startnode] = 0;
	visited[startnode] = 1;
	count = 1;

	while(count < n-1) {
		mindistance = INFINITY;

		// nextnode gives the node at minimum distance
		for(i = 0; i < n; i++) {
			if(distance[i] < mindistance && !visited[i]) {
				mindistance = distance[i];
				nextnode = i;
				printf("toto\n");
			}
		}

		// check if a better path exists through nextnode			
		visited[nextnode] = 1;
		for(i = 0; i < n; i++) {
			if(!visited[i]) {
				if(mindistance + cost[nextnode][i] < distance[i]) {
					distance[i] = mindistance + cost[nextnode][i];
					pred[i] = nextnode;
				}
			}
		}
		printf("\n");
		count++;
	}

	// print the path and distance of each node
	for(i = 0; i < n; i++) {
		if(i != startnode) {
			printf("\nDistance of node %d = %d", i, distance[i]);
			printf("\nPath = %d",i);

			j = i;
			do {
				j = pred[j];
				printf(" <- %d ", j);
			} while(j != startnode);
		}
	}
	printf("\n");
}