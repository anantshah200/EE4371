/* Program : To perform clock tree routing
 * Author : Anant Shah
 * Date : 26-10-2018
 * Roll Number : EE16B105
 **/

#include<stdio.h>
#include<stdlib.h>

struct flipflop{
	int		X_coord; /* X-coordinate of the flip-flop */
	int		Y_coord; /* Y-coordinate of the flip-flop */
	int 	dist; /* Distance of the flip-flop from the 0th flip-flop */
	int		id; /* ID of the flip-flop */
};

struct edge{
	int 	s_id; /* ID of the flip-flop at which the edge starts */
	int 	f_id; /* ID of the flip-flop at which the edge ends */
	int 	dist; /* Length of the edge */
};

typedef struct flipflop ff;
typedef struct edge eg;

struct Node{
	/* struct to store the flip-flops in a linked list */
	ff  clk;
	struct Node *next;
};

struct Node_eg{
	/* struct to store the edges in a linked list */
	eg 	wire;
	struct Node_eg *next;
};

typedef struct Node nd;
typedef struct Node_eg nde;

void add_ff_to_list(nd **sptr,ff *clock){
	nd *newptr = malloc(sizeof(nd));
	newptr->next = NULL;
	newptr->clk.X_coord = clock->X_coord;
	newptr->clk.Y_coord = clock->Y_coord;
	newptr->clk.dist = clock->dist;
	newptr->clk.id = clock->id;
	if(*sptr == NULL){
		*sptr = newptr;
		return;
	}
	nd *curptr = *sptr;
	while(curptr->next!=NULL){
		curptr = curptr->next;
	}
	curptr->next = newptr;
}

void add_edge_to_list(nde **sptr,eg *ffwire){
	nde *newptr = malloc(sizeof(nde));
	newptr->next = NULL;
	newptr->wire.s_id = ffwire->s_id;
	newptr->wire.f_id = ffwire->f_id;
	newptr->wire.dist = ffwire->dist;
	if(*sptr == NULL){
		*sptr = newptr;
		return;
	}
	nde *curptr = *sptr;
	while(curptr->next!=NULL){
		curptr = curptr->next;
	}
	curptr->next = newptr;
}

int comparator(const void *p1,const void *p2){
	/* Defines the order of elements for qsort in user defined data types */
	int y1 = ((ff *)p1)->dist;
	int y2 = ((ff *)p2)->dist;
	return y1-y2; /* Element pointed by p1 goes before element pointed by p2 */
}

int dist_ff(ff *f1,ff *f2){
	/* Function to return the distance of flip-flop f1 from flip-flop f2 */
	return abs(f1->X_coord-f2->X_coord)+abs(f1->Y_coord-f2->Y_coord);
}

void create_edge(eg *cur_edge,int s_id,int f_id,int dist){
	cur_edge->s_id = s_id;
	cur_edge->f_id = f_id;
	cur_edge->dist = dist;
}

void get_minimum_wirelength(ff *ff_init,ff *ff_locations,int X_chip_size,int Y_chip_size,int N_ff){
	int 	min_dist; /* Minimum distance of the current flip-flop being processed from the flip-flop that have already been processed */
	int 	cur_dist;
	int  	min_id; /* ID of the flip-flop which is nearest to the flip-flop currently being processed */
	int 	min_Xcoord = X_chip_size; /* X coordinate of the closest flip flop */
	int 	min_Ycoord = Y_chip_size; /* Y coordinate of the closest flip flop */
	int 	total_ff = N_ff; /* Total number of flip-flops on the board */
	nd 		*n_ff_locations = NULL; /* A pointer to see which flip-flops have been detected */
	nde 	*tree_edges = NULL; /* A pointer to the list of edges */
	eg 		*cur_edge = NULL; /* Current edge which needs to be added to the tree */
	ff 		*new_ff; /* An intermediate flip-flop introduced so as to decrease the overall length of the wire */
	nd 		*extra_ff_locations = NULL; /* Coordinates of the flip-flops introduced in the model */
	int 	wire_len = 0; /* Length of the wire required */
	nd 		*curptr; /* Pointer to iterate through the list of flip-flops */
	int 	mean_x = X_chip_size/2; /* Mean of the chip  */
	int 	mean_y = Y_chip_size/2; /* Mean of the chip */
	int 	d_mean_alpha, d_mean_beta; /* Distance of the two conditions from the mean */

	qsort((void *)ff_locations,N_ff,sizeof(ff),comparator); /* Sort the flip-flops based on the distances from the origin */
	add_ff_to_list(&n_ff_locations,ff_init);

	for(int i=0;i<N_ff;i++){
		curptr = n_ff_locations;
		min_dist = X_chip_size+Y_chip_size+1;
		min_id = 0;
		while(curptr->next!=NULL){
			cur_dist = dist_ff((ff_locations+i),&(curptr->clk));
			if(cur_dist<min_dist){
				min_dist = cur_dist;
				min_id = curptr->clk.id;
				min_Xcoord = curptr->clk.X_coord;
				min_Ycoord = curptr->clk.Y_coord;
			}
			curptr = curptr->next;
		}
		cur_dist = dist_ff((ff_locations+i),&(curptr->clk));
		if(cur_dist<min_dist){
			min_dist = cur_dist;
			min_id = curptr->clk.id;
			min_Xcoord = curptr->clk.X_coord;
			min_Ycoord = curptr->clk.Y_coord;
		}
		/* Closest flip flop has now been obtained. Need to check if they lie on the smae line */
		if( (ff_locations+i)->X_coord==min_Xcoord || (ff_locations+i)->Y_coord==min_Ycoord ){
			cur_edge = (eg *)malloc(sizeof(eg));
			create_edge(cur_edge,min_id,(ff_locations+i)->id,min_dist);
			add_edge_to_list(&tree_edges,cur_edge);
			free(cur_edge);
		}else{
			//d_mean_alpha = abs(mean_x-min_Xcoord) + abs(mean_y-(ff_locations+i)->Y_coord);
			//d_mean_beta = abs(mean_x-(ff_locations+i)->X_coord) + abs(mean_y-min_Ycoord);
			//if(d_mean_alpha < d_mean_beta){
				new_ff = (ff *)malloc(sizeof(ff));
				new_ff->X_coord = min_Xcoord;
				new_ff->Y_coord = (ff_locations+i)->Y_coord;
				new_ff->dist = dist_ff(new_ff,ff_init);
				new_ff->id = ++total_ff;
			/*}else{
				new_ff = (ff *)malloc(sizeof(ff));
				new_ff->X_coord = (ff_locations+i)->X_coord;
				new_ff->Y_coord = min_Ycoord;
				new_ff->dist = dist_ff(new_ff,ff_init);
				new_ff->id = ++total_ff;
			}*/
			add_ff_to_list(&n_ff_locations,new_ff);
			cur_edge = (eg *)malloc(sizeof(eg));
			create_edge(cur_edge,min_id,new_ff->id,abs(new_ff->Y_coord-min_Ycoord));
			add_edge_to_list(&tree_edges,cur_edge);
			free(cur_edge);
			cur_edge = (eg *)malloc(sizeof(eg));
			create_edge(cur_edge,new_ff->id,(ff_locations+i)->id,abs(min_Xcoord-(ff_locations+i)->X_coord));
			add_edge_to_list(&tree_edges,cur_edge);
			free(cur_edge);
			add_ff_to_list(&extra_ff_locations,new_ff);
		}
		wire_len += min_dist;
		add_ff_to_list(&n_ff_locations,(ff_locations+i));
	}

	/* Printing the results */
	int num_ff_added = total_ff - N_ff;
	printf("%d\n",num_ff_added);
	if(num_ff_added!=0){
		nd *curptr = extra_ff_locations;
		while(curptr->next!=NULL){
			printf("%d %d\n",curptr->clk.X_coord,curptr->clk.Y_coord);
			curptr = curptr->next;
		}
		printf("%d %d\n",curptr->clk.X_coord,curptr->clk.Y_coord);
	}
	printf("%d\n",total_ff);
	nde *curedge = tree_edges;
	while(curedge->next!=NULL){
		printf("%d %d\n",curedge->wire.s_id,curedge->wire.f_id);
		curedge = curedge->next;
	}
	printf("%d %d\n",curedge->wire.s_id,curedge->wire.f_id);
	printf("Minimum Distance : %d\n",wire_len);
}

int main(int argc,char **argv){
	int 	X_chip_size; /* Size of the chip in the X-direction */
	int 	Y_chip_size; /* Size of the chip in the Y-direction */
	int		N_ff; /* Number of flip flops */
	int		X_startff, Y_startff; /* X and Y coordinates if the 0th flip-flop */
	int		Ne; /* Number of extra vertices added */
	ff 		*ff_init; /* Flip flop with id 0 */
	ff 		*ff_locations; /* An array consisting of all the flip-flops */
	int 	trivial_len = 0;

	ff_init = (ff *)malloc(sizeof(ff));
	scanf("%d %d",&X_chip_size,&Y_chip_size);
	//scanf("%d %d",&X_startff,&Y_startff); /* Read the coordinates of the 0th flip flop */
	scanf("%d %d",&(ff_init->X_coord),&(ff_init->Y_coord));

	ff_init->id = 0;
	ff_init->dist = 0;
	scanf("%d",&N_ff);

	ff_locations = (ff *)malloc(sizeof(ff)*N_ff);

	for(int i=0;i<N_ff;i++){
			scanf("%d %d",&((ff_locations+i)->X_coord),&((ff_locations+i)->Y_coord));
			(ff_locations+i)->id = i+1;
			(ff_locations+i)->dist = abs(ff_init->X_coord - (ff_locations+i)->X_coord) + abs(ff_init->Y_coord - (ff_locations+i)->Y_coord);
			trivial_len += (ff_locations+i)->dist;
	}

	get_minimum_wirelength(ff_init,ff_locations,X_chip_size,Y_chip_size,N_ff);

}
