/* Program : Find the capital of an island
 * Author : Anant Shah
 * Date : 4-10-2018
 * Roll Number : EE16B105
 **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define	INF 9999999
#define MAX_TOWNS 20000
#define MAX_WORD_LEN 32
#define MAX_ISLANDS 1000

struct	vertex{
	/* Data-type which holds the town name as well as its ID */
	char	*town; /* Name of the town */
	int	id; /* ID of the town */
};

struct Node{
	int id;
	struct Node *next;
};

typedef	struct Node nd;
typedef struct vertex vt;

void create_graph(int *graph,int V){
	/* Function to initialize the graph with weights */
	/* Parameters : graph - representing the adjacency matrix
			V - number of vertices
	 */
	for(int i=0;i<V;i++){
		for(int j=0;j<V;j++){
			if(i==j)	graph[i*V+j] = 0;
			else		graph[i*V+j] = INF;
		}
	} 
}

void add_edge(int *graph,int V,int src,int dest,int weight){
	/* Function to add an edge to the graph */
	/* Parameters : graph - representing the adjacency matrix
	 * 		V - number of vertices
	 * 		src - Source index to be added
	 * 		dest - Destination index to be added
	 *		weight - Weight of the edge in the graph
	 */
	graph[src*V+dest] = weight;
	graph[dest*V+src] = weight;	
}

void floyd_warshall(int *graph,int V){
	/* Function to perform the floyd-warshall algorithm */
	for(int k=0;k<V;k++){
		for(int i=0;i<V;i++){
			for(int j=0;j<V;j++){
				if(graph[i*V+k]+graph[k*V+j] < graph[i*V+j])
					graph[i*V+j] = graph[i*V+k] + graph[k*V+j];
			}
		}
	}
}

void enqueue(nd **head,nd **tail,int vertex){
	/* Function to enqueue the vertex into the queue while performing the BFS */
	nd *newptr = malloc(sizeof(nd));
	newptr->id = vertex;
	newptr->next = NULL;
	if(*head == NULL){
		*head = newptr;
		*tail = *head;
		return;
	}
	(*tail)->next = newptr;
	*tail = newptr;
}

int dequeue(nd **head){
	if(*head == NULL){
		printf("error : dequeue from an empty queue\n");
		exit(0);
	}
	nd *oldhead = *head;
	*head = (*head)->next;
	return oldhead->id;
}

int is_queue_empty(nd **head){
	/* Function to check if a queue is empty or no */
	if(*head == NULL)	return 1;
	return -1;
}

void swapStrings(int lptr,int rptr,char **str){
        char *temp;
        temp = *(str+lptr);
        *(str+lptr) = *(str+rptr);;
        *(str+rptr) = temp;
}

int DictionaryPartition(int l,int r,char **str){
        int partition = r;
        int lptr = l;
        int rptr = r-1;
        int temp;

        while(lptr<=rptr)
        {
                while(strcmp(*(str+lptr),*(str+partition))<0)                              lptr++;
                while(strcmp(*(str+rptr),*(str+partition))>0&&rptr>0)                      rptr--;
                /* I need to swap these two elements */
                if(lptr==partition)     return partition;
                if(rptr<=lptr){
                        swapStrings(lptr,partition,str);
                        return lptr;
                }
                swapStrings(lptr,rptr,str);
                lptr++;
                rptr--;
        }

        swapStrings(lptr,partition,str);
        return lptr;

}

void CreateDictionary(int l,int r,char **str){
        if(r<=l)        return;
        int p = DictionaryPartition(l,r,str);
        CreateDictionary(l,p-1,str);
        CreateDictionary(p+1,r,str);
}


void 	find_capitals(int *dist,int V,vt *towns){
	/* Function to find the capital of a set of towns */
	/* Parameters : dist - All source shortest paths
			V - number of vertices in the graph
			towns - the vertices of the graph
 	*/
	nd	*head=NULL; /* Head of the queue to perform BFS */
	nd 	*tail=NULL; /* Tail of the queue to perform BFS */
	int	visited[V]; /* Array to keep track is a node hsa been visited or not */
	int	min_dist = INF; /* Minimum sum of distances in an island */
	int	d; /* Sum of distnces from a town to other towns on an island */
	int	cur_ver; /* The current vertex being processed */
	int	capitals[MAX_TOWNS]; /* Array to store the id's of the capitals of islands */
	int	capital_count = 0; /* Total number of capitals */
	int	capital;
	int	num_towns = 0; /* Number of towns per island */
	int	*island; /* Matrix to hold the adjacency matrix of each island */
	int	island_t[MAX_ISLANDS]; /* Keeping a track of all the towns in an island */

	for(int i=0;i<V;i++){
		visited[i] = 0;
	}
	for(int i=0;i<V;i++){
		if(visited[i]==0){
			visited[i] = 1;
			enqueue(&head,&tail,i);
			island_t[num_towns++] = i;
			while(is_queue_empty(&head)!=1){
				cur_ver = dequeue(&head);
				for(int j=0;j<V;j++){
					if(dist[cur_ver*V+j]!=INF && visited[j]!=1) {
						enqueue(&head,&tail,j);
						island_t[num_towns++] = j;
						visited[j] = 1;
					}
				}
			}
			island = (int *)malloc(sizeof(int)*num_towns);
			create_graph(island,num_towns);
			for(int i=0;i<num_towns;i++){
				for(int j=0;j<num_towns;j++)	island[i*num_towns+j] = dist[island_t[i]*V+island_t[j]]; 
			}
			floyd_warshall(island,num_towns);
			for(int i=0;i<num_towns;i++){
				d = 0;
				for(int j=0;j<num_towns;j++){
					d += island[i*num_towns+j];
				}
				if(d<min_dist){
					min_dist = d;
					capital = island_t[i];
				}
				if(d==min_dist){
					if(strcmp((towns+capital)->town,(towns+island_t[i])->town)>0)       capital = island_t[i];
				}
			}
			capitals[capital_count++] = capital;
			num_towns = 0;
			min_dist = INF;
			free(island);
		}	
	}
	char	*capital_names[capital_count];

	for(int i=0;i<capital_count;i++){
		capital_names[i] = (char *)malloc(sizeof(char)*MAX_WORD_LEN);
		strcpy(capital_names[i],(towns+capitals[i])->town);
	}
	CreateDictionary(0,capital_count-1,capital_names);
	for(int i=0;i<capital_count;i++){
		printf("%s\n",capital_names[i]);
	}
}

int town_exists(vt *towns,char *label,int size){
	/* Function to check if a town already exists */
	
	if(size==0)	return -1;
	for(int i=0;i<size;i++){
		if(strcmp((towns+i)->town,label)==0)	return i;
	}
	return -1;
}

int main(int argc,char **argv){

	int	num_towns; /* Number of towns present in the islands */
	int 	num_roads; /* Number of roads which connect the towns */
	int	*graph; /* Adjacency matrix to represent the graph */
	vt	*towns;
	char	*source; /* Source town */
	char	*destination; /* Destination town */
	int	time; /* Time to travel between the 2 towns */
	int	num_cur_towns=0; /* Number of towns currently added in the graph */
	int	src_id;
	int	dest_id;
	int	*shortest_path; /* Pointer to the all pairs shortest path matrix */

	towns = (vt *)malloc(sizeof(vt)*MAX_TOWNS);
	scanf("%d %d",&num_towns,&num_roads);
	graph = (int *)malloc(sizeof(int)*num_towns*num_towns);
	create_graph(graph,num_towns); /* Initialize the adjacency matrix of the graph */

	for(int i=0;i<num_roads;i++){
		source = (char *)malloc(sizeof(char)*MAX_WORD_LEN);
		destination = (char *)malloc(sizeof(char)*MAX_WORD_LEN);
		scanf("%s %s %d",source,destination,&time);
		src_id = town_exists(towns,source,num_cur_towns);
		if(src_id==-1){
			(towns+num_cur_towns)->town = source;
			(towns+num_cur_towns)->id = num_cur_towns;
			src_id = num_cur_towns++;
		}
		dest_id = town_exists(towns,destination,num_cur_towns);
		if(dest_id==-1){
			(towns+num_cur_towns)->town = destination;
			(towns+num_cur_towns)->id = num_cur_towns;
			dest_id = num_cur_towns++;
		}
		add_edge(graph,num_towns,src_id,dest_id,time);
	}
	
	/* Now need to execute the floyd warshall algorithm on the adjacency matrix */

	find_capitals(graph,num_towns,towns);	
	free(towns);
	free(graph);
}
