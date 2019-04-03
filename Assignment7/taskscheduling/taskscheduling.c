/* Program : Pre-emptive scheduling of tasks on a processor efficiently
 * Author : Anant Shah
 * Date : 29-10-2018
 * Roll Number : EE16B105
 */

#include<stdio.h>
#include<stdlib.h>
#define INF 999999

struct task{
	int rel_time; /* Release time of the task */
	int ded_time; /* Latest possible finish time of the task */
	int duration; /* Duration of the task */
	int id; /* ID of the task */
};

struct output_f{
	int id;
	int start;
	int end;
};

typedef struct task tk;
typedef struct output_f out;

struct Node{
	tk 			job;
	struct Node *next;
};

struct Node_out{
	out 	output;
	struct Node_out *next;
};

typedef struct Node nd;
typedef struct Node_out ndo;

void add_to_task_list(nd **sptr,tk *newtask){
	/* Add a task to a list of tasks */
	nd *newptr = malloc(sizeof(nd));
	newptr->next = NULL;
	newptr->job.rel_time = newtask->rel_time;
	newptr->job.ded_time = newtask->ded_time;
	newptr->job.duration = newtask->duration;
	newptr->job.id = newtask->id;
	if( *sptr == NULL){
		*sptr = newptr;
		return;
	}
	nd *curptr = *sptr;
	while(curptr->next!=NULL){
		curptr = curptr->next;
	}
	curptr->next = newptr;
}

void add_to_out_list(ndo **sptr,out *disp){
	ndo *newptr = malloc(sizeof(ndo));
	newptr->next = NULL;
	newptr->output.id = disp->id;
	newptr->output.start = disp->start;
	newptr->output.end = disp->end;
	if( *sptr == NULL){
		*sptr = newptr;
		return;
	}
	ndo *curptr = *sptr;
	while(curptr->next!=NULL){
		curptr = curptr->next;
	}
	curptr->next = newptr;
}

int is_task_list_empty(nd **sptr){
	if( *sptr == NULL){
		return 1;
	}
	return 0;
}

void remove_task(nd **sptr,tk *newtask){
	if( *sptr == NULL){
		printf("error : Trying to delete from an empty list");
		return;
	}else{
		nd *curptr = *sptr;
		nd *prevptr = NULL;
		while(curptr->job.id != newtask->id && curptr->next!=NULL){
			prevptr = curptr;
			curptr = curptr->next;
		}
		if((curptr->job.id == newtask->id) && (curptr->next == NULL)){
			prevptr->next = NULL;
		} else if(curptr == *sptr){
			*sptr = curptr->next;
		}else{
			prevptr->next = curptr->next;
		}
	}
}

int comparator(const void *t1,const void *t2){
	/* Function to compare to tasks based on their start times */
	int start_t1 = ((tk *)t1)->rel_time;
	int start_t2 = ((tk *)t2)->rel_time;
	return start_t1 - start_t2;
}

tk* get_urgent_task(nd **stalled_tasks,int cur_time){
	/* Find the task which needs to be executed most urgently */
	nd *curptr = *stalled_tasks;
	int min_slack = INF;
	int slack;
	tk  *urg_task; /* Most urgent task to be executed */
	int valid_task = 0; /* Counter for a valid task */
	while(curptr->next!=NULL){
		slack = curptr->job.ded_time - curptr->job.duration - cur_time;
		if(slack>=0){
			valid_task++;
			if(slack<min_slack){
				min_slack = slack;
				urg_task = &(curptr->job);
			}
		}
		curptr = curptr->next;
	}
	slack = curptr->job.ded_time - curptr->job.duration - cur_time;
	if(slack>=0){
		valid_task++;
		if(slack<min_slack){
			min_slack = slack;
			urg_task = &(curptr->job);
		}
	}
	if(valid_task==0)	return NULL;
	return urg_task;
}

void create_out(out *printo,int id,int start,int end){
	printo->id = id;
	printo->start = start;
	printo->end = end;
}

void print_tasks(ndo *print_out){
	ndo *curptr = print_out;
	int start=0;
	int start_time;
	int end_time;
	while(curptr->next != NULL){
		if(curptr->output.id == curptr->next->output.id){
			if(start==0) {
				start_time = curptr->output.start; 
				start = 1;
			}
		}
		else{
			if(start==1){
				end_time = curptr->output.end;
				printf("%d %d %d\n",start_time,end_time,curptr->output.id);
				start = 0;
			}else{
				printf("%d %d %d\n",curptr->output.start,curptr->output.end,curptr->output.id);
			}
		}
		curptr = curptr->next;
	}
	printf("%d %d %d\n",curptr->output.start,curptr->output.end,curptr->output.id);
}

void schedule_tasks(tk *ops,int num_tasks){

	nd 		*stalled_tasks = NULL;
	tk* 	cur_task;
	int 	next_cur_time; /* Time when the next set of tasks become available */
	int 	min_end_time; /* Minimum ending time of a set of tasks with the same start time */
	int 	j;
	int 	ideal_end_time; /* Ideal end time of the current task */
	int 	cur_time = 0; /* Clock for the processor */
	int 	task_count =0; /* Number of tasks that have been read */
	int 	exec_time; /* Execution time of the stalled tasks between two tasks being available */
	tk*		temp_task;
	ndo 	*print_out = NULL;
	out 	*printo;
	int 	invalid;

	cur_task = (tk *)malloc(sizeof(tk));
	qsort((void *)ops,num_tasks,sizeof(tk),comparator); /* Sort the tasks based on their starting times */
	do{
		exec_time = 0;
		cur_time = (ops+task_count)->rel_time;
		add_to_task_list(&stalled_tasks,ops+(task_count++));
		while((ops+task_count)->rel_time==cur_time){
			add_to_task_list(&stalled_tasks,(ops+(task_count++)));
		}
		/* All the tasks that are available to execute have been added to the list */
		/* I need to find the task with the minimum slack(most urgent task) */
		if(task_count!=num_tasks){
			next_cur_time = (ops+task_count)->rel_time;
			/* Now i need to execute my stalled tasks till <next_cur_time> */
			while(cur_time<next_cur_time){
				cur_task = get_urgent_task(&stalled_tasks,cur_time); /* Obtained the most urgent task to be executed */
				if(cur_task!=NULL){
					exec_time = cur_task->duration;
					if((exec_time+cur_time)>next_cur_time){
						printo = (out *)malloc(sizeof(out));
						create_out(printo,cur_task->id,cur_time,next_cur_time);
						add_to_out_list(&print_out,printo);
						free(printo);		
						cur_task->duration = cur_task->duration - (next_cur_time - cur_time);
						break;
					}else{
						printo = (out *)malloc(sizeof(out));
						create_out(printo,cur_task->id,cur_time,cur_time+exec_time);
						add_to_out_list(&print_out,printo);
						free(printo);
						remove_task(&stalled_tasks,cur_task);
						cur_time += exec_time;
					}
				}else break;
			}
		}else{
			/* No more task is coming and hence we now just execute all based on urgency*/
			while(stalled_tasks!=NULL){
				cur_task = get_urgent_task(&stalled_tasks,cur_time);
				//printf("Current Task : %p\n",cur_task);
				if(cur_task != NULL){
					exec_time = cur_task->duration;
					printo = (out *)malloc(sizeof(out));
					create_out(printo,cur_task->id,cur_time,cur_time+exec_time);
					add_to_out_list(&print_out,printo);
					free(printo);
					remove_task(&stalled_tasks,cur_task);
					cur_time += exec_time;
				}else{
					nd *curptr = stalled_tasks;
					invalid = 0;
					while(curptr->next!=NULL){
						exec_time = curptr->job.duration;
						printo = (out *)malloc(sizeof(out));
						create_out(printo,curptr->job.id,cur_time,cur_time+exec_time);
						add_to_out_list(&print_out,printo);
						free(printo);
						cur_time += exec_time;
						curptr = curptr->next;
						invalid++;
					}
					exec_time = curptr->job.duration;
					printo = (out *)malloc(sizeof(out));
					create_out(printo,curptr->job.id,cur_time,cur_time+exec_time);
					add_to_out_list(&print_out,printo);
					free(printo);
					invalid++;
					break;
				}
			}
		}
	}while(task_count!=num_tasks && stalled_tasks!=NULL);
	print_tasks(print_out);
	//printf("Number of invalid tasks : %d\n",invalid);
}

int main(int argc,char **argv){

	int 	num_tasks; /* Number of tasksk to be executed */
	tk 		*ops; /* A pointer to an array of tasks */

	scanf("%d",&num_tasks);
	ops = (tk *)malloc(sizeof(tk)*num_tasks);

	for(int i=0;i<num_tasks;i++){
		scanf("%d %d %d",&((ops+i)->rel_time),&((ops+i)->duration),&((ops+i)->ded_time));
		(ops+i)->id = i+1;
	}
	schedule_tasks(ops,num_tasks);
}
