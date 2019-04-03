/* Program : To simulate the execution of events
 * Author : Anant Shah
 * Date : 17-9-2018
 * Roll Number : EE16B105
 **/

#include<stdio.h>
#include<stdlib.h>

struct Node{
	long long int	end_t;
	struct Node	*next;
};

typedef struct Node nd;

void add_to_list(nd **sptr,long long int endtime){
	/* Function to add a value to the list */
	nd *newptr = (nd *)malloc(sizeof(nd));
	nd *curptr;

	newptr->end_t = endtime;
	newptr->next=NULL;

	if(*sptr==NULL){
		*sptr = newptr;
		return;
	}
	curptr = *sptr;
	while(curptr->next!=NULL){
		curptr=curptr->next;
	}
	curptr->next=newptr;
}
 
void delete_from_list(nd **sptr,long long int time){
	/* Function to delete elements <= time from the list */
	nd *curptr = *sptr;
	nd *prevptr = NULL;
	
	if(*sptr==NULL)	return;

	while(curptr->next!=NULL){
		if(curptr->end_t<=time){
			if(prevptr!=NULL){
				prevptr->next = curptr->next;
				curptr = curptr->next;
			}else{
				*sptr = curptr->next;
				curptr = *sptr;
			}
		}else{
			prevptr=curptr;
			curptr=curptr->next;
		}
	}
	if(curptr->end_t<=time){
		if(prevptr!=NULL)	prevptr->next=NULL;
		else			*sptr=NULL;
	}
}

long long int length_of_list(nd **sptr){
	long long int 	length=0; /* Variable to store the length of the list */
	if(*sptr==NULL)		return 0;
	else{
		nd *curptr = *sptr;
		while(curptr->next!=NULL){
			length++;
			curptr=curptr->next;
		}
		length++;
	}
	return length;
}

int main(int argc,char **argv){

	long long int		T; /* Number of tasks to be executed */
	long long int		end_sim_time=0; /* End of simulation time */
	long long int		max_tasks=0; /* Maximum number of tasks running at a given time */
	double			avg_tasks=0; /* Average number of tasks running at a given time  */
	long long int		num_task=0; /* Number of tasks running at a given time */
	long long int		end_time; /* End-time of a task */
	int			counter=0; /* Iterates till the last task */
	long long int		start_t; /* Start time of the task */
	int			task_time; /* Duration of the task */
	nd			*active_tasks=NULL; /* Array to store the active tasks at a given time */
	long long int		start_prev_t=0; /* Start time of the previous task */

	scanf("%lld",&T);
	
	while((counter++)<T){
		scanf("%lld %d",&start_t,&task_time);
		end_time = start_t+task_time;
		if(counter==0||start_prev_t==start_t){
			/* Just add the task to the list in the first iteration */
			add_to_list(&active_tasks,end_time);
			num_task = length_of_list(&active_tasks);
			if(num_task>max_tasks)	max_tasks = num_task;
		}else{
			num_task = length_of_list(&active_tasks);
			if(num_task>max_tasks)	max_tasks = num_task;
			delete_from_list(&active_tasks,start_t);
			add_to_list(&active_tasks,end_time);
		}
		if(end_time>end_sim_time){
			end_sim_time = end_time;
		}
		avg_tasks += task_time;
		start_prev_t = start_t;
	}
	
	num_task = length_of_list(&active_tasks);
	if(num_task>max_tasks)		max_tasks = num_task;

	avg_tasks = avg_tasks/end_sim_time;
	
	printf("%lld\n",end_sim_time);
	printf("%lld\n",max_tasks);
	printf("%.4f\n",avg_tasks);	
}
