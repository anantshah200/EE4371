/* Program : To find the maximum flow from a source to a sink
 * Author : Anant Shah
 * Date : 2-12-2018
 * Roll Number : EE16B105
 **/

/* I have used the Dinic's algorithm to find the maximum flow path in the given network. The Dinic's algorithm makes use of
 a residual graph and an augmenting path. It works like the Edmund-Karp algorithm but is different in the way that it acts on
 a blocking path. It's time complexity is O(VE^2) which is better then that of Edmund Karp that os O(EV^2). The code for the 
 Dinic's algorithm functions have been taken from Geeks for Geeks. For the residual graph, we add a reverse edge between 
 vertices which provides us with the luxury to decrease the flow in an edge so that eh overall flow increases. We search for 
 an existing path from the source to the destination using the BFS function. The sendFlow() function manages the flows between 
 the edges based on the augmenting path. */

#include<stdio.h>
#include<stdlib.h>
#define MAX_VAL 99999

struct town{
	double coord_x;
	double coord_y;
	int id;
};

typedef struct town tw;

struct AdjListNode{
	double distance; /* Distance of the edge */
	int capacity; /* Capacity of the road based on the road type */
	int destination; /* Destination index of the node */
	int flow; /* Flow of data in an edge */
	struct AdjListNode *next;
};	

struct AdjList{
	struct AdjListNode* head;
};

struct Graph{
	int 	V; /* Number of vertices */
	struct AdjList* array;
	int		*level; /* Stores the level of a node in the graph */
};

struct Node{
	int id;
	struct Node *next;
};

typedef	struct Node nd;
typedef struct AdjListNode alnode;
typedef struct AdjList 	al;
typedef struct Graph grp;

alnode* newAdjListNode(int destination,double distance,int capacity,int flow){
	alnode *new_node = malloc(sizeof(alnode));
	new_node->next = NULL;
	new_node->destination = destination;
	new_node->distance = distance;
	new_node->capacity = capacity;
	new_node->flow = flow;
	return new_node;
}

grp* createGraph(int V){
	/* Utility function to create a graph */
	grp* graph = malloc(sizeof(grp)); /* Allocating memory for the graph */
	graph->V = V;
	graph->array = (al *)malloc(V*sizeof(al));
	graph->level = (int *)malloc(V*sizeof(int));
	for(int i=0;i<V;i++){
		graph->array[i].head = NULL;
		//graph->level[i] = -1;
	}
	return graph;
}

void addEdge(grp *graph,int src,int dest,double distance,int capcity,int flow){
	alnode* newNode = newAdjListNode(dest,distance,capcity,flow);
	alnode *curptr = graph->array[src].head;
	if(graph->array[src].head == NULL){
		graph->array[src].head = newNode;
	}else{
		alnode *curptr = graph->array[src].head;
		while(curptr->next!=NULL){
			curptr = curptr->next;
		}
	curptr->next = newNode;
	}
	/* Since it is a bidirectioal graph, we need to add the edge from destination to source as well */
	/*alnode* newNode_s = newAdjListNode(src,distance,0,0);
	if(graph->array[dest].head == NULL){
		graph->array[dest].head = newNode_s;
		return;
	}
	curptr = graph->array[dest].head;
	while(curptr->next!=NULL){
		curptr = curptr->next;
	}
	curptr->next = newNode_s;*/
}

void print_graph(grp *graph){
	alnode *curptr;
	for(int i=0;i<graph->V;i++){
		curptr = graph->array[i].head;
		printf("%d->",i);
		if(curptr!=NULL){
			while(curptr->next!=NULL){
				printf("%d->",curptr->destination);
				curptr = curptr->next;
			}
			printf("%d\n",curptr->destination);
		}
	}
}

void newTown(tw *node,int id,double coord_x,double coord_y){
	node->id = id;
	node->coord_x = coord_x;
	node->coord_y = coord_y;
}

void printNodes(tw *nodes,int V){
	for(int i=0;i<V;i++){
		printf("%d %lf %lf\n",(nodes+i)->id,(nodes+i)->coord_x,(nodes+i)->coord_y);
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

int BFS(grp *graph,int start_node,int final_node){
	/* Function to assign level to the nodes */
	for(int i=0;i<graph->V;i++){
		graph->level[i] = -1;
	}
	graph->level[start_node] = 0; /* Set the level of the start node to 0 */
	nd *head = NULL;
	nd *tail = NULL;
	enqueue(&head,&tail,start_node);
	while(is_queue_empty(&head)!=1){
		int temp = dequeue(&head);
		alnode *curptr = graph->array[temp].head;
		while(curptr!=NULL){
			if(graph->level[curptr->destination]<0 && curptr->flow<curptr->capacity){
				graph->level[curptr->destination] = graph->level[temp]+1;
				enqueue(&head,&tail,curptr->destination);
			}
			curptr = curptr->next;
		}
	}
	if(graph->level[final_node]<0)		return -1;
	else								return 1;
}

int sendFlow(grp *graph,int u,int flow,int final_node,int *start){
	if(u==final_node)	return flow;
	alnode *curptr = graph->array[u].head;
	int 	curr_flow;
	int 	temp_flow;
	int 	size =0;
	while(curptr!=NULL){
		curptr=curptr->next;
		++size;
	} /* Got the size of the adjacency list */
	alnode *newptr;
	for( ;start[u]<size;start[u]++){
		/* Picking the next edge from the adjacency list of u */
		newptr = graph->array[u].head;
		for(int k=0;k<start[u];k++){
			newptr = newptr->next;
		}
		if((graph->level[newptr->destination] == (graph->level[u]+1)) && newptr->flow<newptr->capacity){
				if(flow <= (newptr->capacity-newptr->flow))	curr_flow = flow;
				else										curr_flow = newptr->capacity - newptr->flow;
				temp_flow = sendFlow(graph,newptr->destination,curr_flow,final_node,start);
				if(temp_flow>0){
					newptr->flow += temp_flow; // add flow to the current edge
					// subtract flow from the reverse edge of the current edge
					alnode *tempcur; /* Temporary pointer to iterate through the reverse list */
					tempcur = graph->array[newptr->destination].head;
					while(tempcur->destination!=u){
						tempcur = tempcur->next;
					}
					tempcur->flow -= temp_flow;
					return temp_flow;
				}
		}
	}
	return 0;
}

void print(grp *graph){
	/* Utility function to print the intermediate nodes */
	alnode *curptr;
	int V = graph->V;
	for(int i=0;i<V;i++){
		curptr = graph->array[i].head;
		while(curptr!=NULL){
			if(curptr->flow>0)	printf("%d %d %d\n",i,curptr->destination,curptr->flow);
			curptr = curptr->next;
		}
	}
}

int max_transport_units(grp *graph,int start_node,int final_node){
	/* Function to find the maximum possible transport units from a starting node to an ending node */
	int	V = graph->V; /* Number of vertices in the graph */
	/* Find all the paths from the source to the destination irrespective of the time taken */
	if(start_node==final_node)		return -1;
	int total = 0; /* Initialize the result for the maximum flow */
	int flow;
	/* Augment the flow while there is a path from source to sink */
	while(BFS(graph,start_node,final_node)==1){
		int *start = (int *)malloc(sizeof(int)*(V+1)); /* Store how many edges are visited */
		for(int i=0;i<V+1;i++){
			start[i] = 0;
		}
		/* While flow is not 0 in the graph */
		while((flow=sendFlow(graph,start_node,MAX_VAL,final_node,start))){
			//printf("Hello\n");
			total += flow;
		}
	}
	print(graph);
	return total;
}

int main(int argc,char **argv){
	int 	num_edges; /* Number of edges in the graph */
	int 	V; /* Variable to store the number of vertices in the graph */
	int 	src;
	int 	dest;
	char 	road_type;
	double 	distance;
	double 	coord_y;
	double 	coord_x;
	int		start_node; /* Start node */
	int		final_node; /* Final node */
	FILE	*fp;

	fp = fopen("tn_graph.txt","r");
	fscanf(fp,"%d",&V); 
    grp* graph = createGraph(V); 
    tw *nodes = malloc(V*sizeof(tw));

    for(int i=0;i<V;i++){
    	fscanf(fp,"%lf %lf",&coord_x,&coord_y);
    	newTown((nodes+i),i,coord_x,coord_y); /* Create a new town based on the longitudes and latitudes */
    }
    fscanf(fp,"%d",&num_edges);
    for(int i=0;i<num_edges;i++){
    	fscanf(fp,"%d %d %c %lf",&src,&dest,&road_type,&distance);
    	if(road_type=='p')	{
    		addEdge(graph,src,dest,distance,100,0); /* Adding the edges to the linked lists in the graph */
    		addEdge(graph,dest,src,distance,100,0); /* Adding the edges to the linked lists in the graph */
    	}
    	else if(road_type=='s')	{
    		addEdge(graph,src,dest,distance,50,0); /* Initialize with 0 flow */
    		addEdge(graph,dest,src,distance,50,0); /* Initialize with 0 flow */
    	}
    	else if(road_type=='t')		{
    		addEdge(graph,src,dest,distance,20,0);
    		addEdge(graph,dest,src,distance,20,0);
    	}
    }
    fclose(fp);
    scanf("%d",&start_node);
    scanf("%d",&final_node);
    //printf("%d\n",start_node);
    //printf("%d\n",final_node);
    int sum = max_transport_units(graph,start_node,final_node);
    //printf("%d\n",sum);
    //print_graph(graph);
}
