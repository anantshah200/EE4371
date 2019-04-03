/* Program : To transport X supplies from source to destination in minimum amount of time
 * Author : Anant Shah
 * Date : 2-12-2018
 * Roll Number : EE16B105
 **/

/* I have discussed with EE16B066 regarding the implementation of this code */
/* The code for Dinic's algorithm has been taken from Geeks for Geeks */
/* The graph header is present in the graph.h file */
/* Here we are employing a successive shortest path algorithm to obtain the minimum time */
/* We set the flow to 0 and in each iteration find the shortest path and send the flow in that direction */

#include <string.h>
#include <iostream>
#include <vector>
#include <list>
#include "graph.h"
#include <limits.h>
#include <queue>

using namespace std;

struct edge{
	int 	src;
	int 	dest;
	double 	length;
	int 	flow;
	int 	id;
    int     type; /* Variable to store the type of the road */
};

struct path{
	vector<edge> 	roads;
};

vector<edge> 	edges;

bool BFS(Graph graph, int source_node, int final_node, int * level) 
{ 
	/* Perform BFS to check if a path exists from the source node to the final node */
    for (int i = 0 ; i < graph.no_nodes ; i++) 
        level[i] = -1; 
  
    level[source_node] = 0;  // Level of source vertex 
  
    // Create a queue, enqueue source vertex 
    // and mark source vertex as visited here 
    // level[] array works as visited array also. 
    list< int > queue; 
    queue.push_back(source_node); 

    while (!queue.empty()) 
    { 
        int u = queue.front(); 
        queue.pop_front(); 
        for (int i = 0; i != graph.nodes[u].adj.size(); i++) 
        {  

            if (level[graph.nodes[u].adj[i]->id] < 0  && graph.nodes[u].adj[i]->flow < graph.nodes[u].adj[i]->C) 
            { 
                // Level of current vertex is, 
                // level of parent + 1 
                level[graph.nodes[u].adj[i]->id] = level[u] + 1; 
  
                queue.push_back(graph.nodes[u].adj[i]->id); 
            } 
        } 
    } 
  
    // IF we can not reach to the sink we 
    // return false else true 
    return level[final_node] < 0 ? false : true ; 
} 

// A DFS based function to send flow after BFS has 
// figured out that there is a possible flow and 
// constructed levels. This function called multiple 
// times for a single call of BFS. 
// flow : Current flow send by parent function call 
// start[] : To keep track of next edge to be explored. 
//           start[i] stores  count of edges explored 
//           from i. 
//  u : Current vertex 
//  t : Sink 
int sendFlow(Graph G, int u, int flow, int t, int start[], int * level) 
{ 
    /* If we reached our destination, return the flow */
    if (u == t)  return flow; 
  
    /* We now want to traverse all the adjacent edges */
    for (  ; start[u] < G.nodes[u].adj.size(); start[u]++) 
    { 
        adj_node* e = G.nodes[u].adj[start[u]];  
                                      
        if (level[e->id] == level[u]+1 && e->flow < e->C) 
        { 
            int curr_flow = min(flow, e->C - e->flow); /* Find the minimum flow from the current node to the destination */
            int temp_flow = sendFlow(G, e->id, curr_flow, t, start, level); 
  
            /* If the flow is greater than 0 */
            if (temp_flow > 0) 
            { 
                /* Add flow to the current edge */
                G.nodes[u].adj[start[u]]->flow += temp_flow; 
                edge e_add;
                e_add.src = u;
                e_add.dest = G.nodes[u].adj[start[u]]->id;
                e_add.id = G.nodes[u].adj[start[u]]->edge_id;
                e_add.length = G.nodes[u].adj[start[u]]->dist;
                e_add.flow = temp_flow;
                e_add.type = G.nodes[u].adj[start[u]]->C;

                edges.push_back(e_add); /* Adding the new edge to the existing set of edges */

                /* We now subtract the flow from the reverse edge of the current edge */
                for (int i = 0; i < G.nodes[e->id].adj.size(); ++i)
                {
                    if(u == G.nodes[e->id].adj[i]->id){
                        G.nodes[e->id].adj[i]->flow -= temp_flow;
                        break;
                    }
                }
                return temp_flow; 
            } 
        } 
    } 
  
    return 0; 
} 

/* Return the maximum flow in the graph */
int DinicMaxflow(Graph graph, int start_node, int final_node, int *level) 
{ 
	/* Function has been taken from Geeks for Geeks */
    if (start_node == final_node) 	return -1; /* Handles the corner case */
  
    int total = 0;  // Initialize result 
  
    // Augment the flow while there is path 
    /* Iterate as long as there is a path from the source node to the final node */
    while (BFS(graph, start_node, final_node, level) == true) 
    { 
        // store how many edges are visited 
        // from V { 0 to V } 
        int *start = new int[graph.no_nodes+1]; 
  
        // while flow is not zero in graph from S to D 
        while (int flow = sendFlow(graph, start_node, INT_MAX, final_node, start, level)) 
  
            // Add path flow to overall flow 
            total += flow; 
    } 
  
    // return maximum flow 
    return total; 
} 

vector<path> get_paths(vector<edge> temp_edge, vector<double> &path_len, int source, int destination,vector<int> &bottleneck){
	/* Function to find the individual paths in the residual graph */
    vector<path> ind_paths; /* A vector consisting of the individual paths */
    //vector<int>  bottleneck; /* The bottleneck type of road in every path */
    for (int i = 0; i < temp_edge.size(); ++i)
    {
        int j = i;
        bool min_found = false;
        int road_type = 100; /* Denotes a primary road; 50 -secondary road; 20-tertiary road */
        path temp;
        double size = 0.0;
        /* What we are doing is trying to find the distance of a path. We will then choose the path with minimum distance 
        and then act upon that */
        /* Minimum distance implies the minimum time taken along a path. Here we consider the latency of the path and the 
        time due to the transportation(bottleneck) */
        while(temp_edge[j].src != source){
            temp.roads.push_back(temp_edge[j]);
            /* Here we find the bottleneck on a given path */
            if(temp_edge[j].type==50)  {
                if(min_found==false)    road_type = 50;
            }
            else if(temp_edge[j].type==20) {
                road_type = 20;
                min_found = true;
            }
            if(temp_edge[j].type==100)    size += temp_edge[j].length/60; /* Storing the size of the path based on the type of the path */
            else if(temp_edge[j].type==50) size += temp_edge[j].length/40;
            else if(temp_edge[j].type==20) size += temp_edge[j].length/30;
            j++;
        }
        temp.roads.push_back(temp_edge[j]);
        if(temp_edge[j].type==100)      size += temp_edge[j].length/60; /* Storing the size of the path based on the time */
        else if(temp_edge[j].type==50)  size += temp_edge[j].length/40;
        else if(temp_edge[j].type==20)  size += temp_edge[j].length/30;
        if(temp_edge[j].type==50)  {
                if(min_found==false)    road_type = 50;
        }
        else if(temp_edge[j].type==20) {
            /* If we find minimum road type, that is the bottleneck */
            road_type = 20;
            min_found = true;
        }
        ind_paths.push_back(temp);
        path_len.push_back(size);
        bottleneck.push_back(road_type);
        i = j;
    }
    return ind_paths;
}

void get_min_time(Graph graph,int source_node,int final_node,int X){
    /* Function to supply the trucks thorugh the minimum path */
    /* This is the successive shortest path algorithm */
	int 	trucks = 0;
	vector<int> edgefl(graph.no_edges);
    vector<double> path_size; /* Lengths of the maximum flow paths */
    vector<int> bottleneck; /* Bottleneck type of road in each path */
    vector<path> paths = get_paths(edges, path_size, source_node, final_node, bottleneck);
    double  time = 0;

    for(int i=0;i<path_size.size();++i){
        if(path_size[i]>time)   time = path_size[i];
    }
    cout << time << endl;
    time = 0;

    while(trucks < X){
        /* Algorithm : Choose the minimum path length and send trucks along that path until the number of trucks burn out */
        int min_id = 0;
        double min_val = 999999;
        for (int i = 0; i < path_size.size(); ++i)
        {
            if(path_size[i] < min_val){
                min_val = path_size[i];
                //cout << min_val << endl;
                min_id = i;
            }
        }
        //path_size[min_id] = -1; /* Update the path size of the minimum path back to -1 */
        int flow_amt = paths[min_id].roads[0].flow; /* The amount of flow available on that minimum path */
        if(flow_amt > X-trucks)		flow_amt = X-trucks;  

        /* We now need to update the flow in each edge based on the flow sent through the shortest path */
        for (int i = 0; i < paths[min_id].roads.size(); ++i)
        {
            edgefl[paths[min_id].roads[i].id] += flow_amt;
            graph.edges[paths[min_id].roads[i].id].nodes[0] = paths[min_id].roads[i].src;
            graph.edges[paths[min_id].roads[i].id].nodes[1] = paths[min_id].roads[i].dest;
        }
        trucks += flow_amt;
        path_size[min_id] += (double)flow_amt/bottleneck[min_id]; /* Updating the cost on a path based on the bottleneck faced by the trucks */
    }
    for (int i = 0; i < graph.no_edges; ++i)
    {
        if(edgefl[i] > 0){
            //cout << graph.edges[i].nodes[0] << " " << graph.edges[i].nodes[1] << " " << edgefl[i] << endl;  
        }
    }
    /* Find the minimum time */
    for(int i=0;i<path_size.size();++i){
        cout << path_size[i] << endl;
    }
    //cout << time << endl;
}

void createAdjNode(adj_node* node,int id,int dist,int edge_id,int flow,int rev){
    /* Utility function to create an adjacency list node */
    node->id = id;
    node->dist = dist;
    node ->edge_id = edge_id;
    node->flow = flow;
    node->rev = rev;
}

int main(int argc,char **argv){
	Graph graph;
	graph.read_graph("tn_graph.txt");
	int *level = new int[graph.no_nodes]; /* Dinic's algorithm makes use of a level array which denotes the number of edges of a vertex from the source*/
	/* Generate an adjacency list for the graph. Using an adjacency matrix will be detrimental as it will use up far too much memory */
	for(int i=0;i<graph.no_edges;++i){
			int src = graph.edges[i].nodes[0];
			int dest = graph.edges[i].nodes[1];
			adj_node* a = new adj_node;
			adj_node* b = new adj_node;
            createAdjNode(a,src,graph.edges[i].distance,i,0,dest);
            createAdjNode(b,dest,graph.edges[i].distance,i,0,src);
			/* Now based on the type of the road, we add the capacity */
			if(graph.edges[i].type == 't'){
            	a->C = 20;
            	b->C = 20;
        	}
        	else if(graph.edges[i].type == 's'){
            	a->C = 50;
            	b->C = 50;
        	}
        	else if(graph.edges[i].type == 'p'){
            	a->C = 100;
            	b->C = 100;
        	}
        	graph.nodes[src].adj.push_back(b);
        	graph.nodes[dest].adj.push_back(a);
	}
	int source_node, final_node; /* Source node and the destination node */
    cin >> source_node >> final_node;
    int X; /* Number of supplies to be transported from the source to the destination */
    cin >> X;
    DinicMaxflow(graph, source_node, final_node, level); /* Call the DinicMaxFlow function to the get the maximum flow */
    get_min_time(graph,source_node,final_node,X);
}
