/* Program : To build a binary search tree based on the values of a list
 * Author : Anant Shah
 * Date : 26-8-2018
 **/

#include<stdio.h>
#include<stdlib.h>

struct Node{
	int		data; /* The data a node stores */
	struct Node	*lptr; /* Pointer to another node to the left of the current node */
	struct Node	*rptr; /* Pointer to another node to the right of the current node */
};

typedef struct Node nd;

void insert(nd **root,int num){
	nd *newptr = malloc(sizeof(nd));
	nd *curptr;
	newptr->data = num;
	newptr->lptr = NULL;
	newptr->rptr = NULL;
	
	if(*root == NULL){
		*root = newptr;
		return;
	}
	curptr = *root;
	while(curptr->lptr!=NULL||curptr->rptr!=NULL){
		if(curptr->data<num){		
			if(curptr->rptr!=NULL)		curptr = curptr->rptr;
			else	{
				curptr->rptr = newptr;
				return;
			}
		}
		else if(curptr->data>=num){
			/* If the number is less than or equal to the node value, this tree will add it to the left of the node */
			if(curptr->lptr!=NULL)		curptr = curptr->lptr;
			else	{
				curptr->lptr = newptr;
				return;
			}
		}
	}
	if(curptr->data<num)	curptr->rptr = newptr;
	else			curptr->lptr = newptr;
}

int search(nd *root,int num){
	/* Function will return 1 if the element is found in the tree otherwise will return 0 */
	nd *curptr = root;
	int found = -1;
	if(num==curptr->data)	return 1;
	if(num>curptr->data){
		if(curptr->rptr!=NULL)	found = search(curptr->rptr,num);
		else			return -1;
	}
	if(num<curptr->data){
		if(curptr->lptr!=NULL)	found = search(curptr->lptr,num);
		else			return -1;
	}
	return found;
}

void inorder(nd *root){
	nd *curptr = root;
	if(curptr->lptr!=NULL)				inorder(curptr->lptr);
	printf("%d\n",curptr->data);
	if(curptr->rptr!=NULL)				inorder(curptr->rptr);
}

void preorder(nd *root){
	nd *curptr = root;
	printf("%d\n",curptr->data);
	if(curptr->lptr!=NULL)				preorder(curptr->lptr);
	if(curptr->rptr!=NULL)				preorder(curptr->rptr);
}

void postorder(nd *root){
	nd *curptr = root;
	if(curptr->lptr!=NULL)				postorder(curptr->lptr);
	if(curptr->rptr!=NULL)				postorder(curptr->rptr);
	printf("%d\n",curptr->data);
}

int main(int argc,char **argv){
	if(argc!=1){
		printf("error : Invalid number of arguments\n");
		exit(EXIT_FAILURE);
	}

	nd	*root=NULL; /* Initialize the root pointer to NULL */
	int	num; /* Number entered from stdin to be inserted in the BST */
	int	N; /* Number of values to be added in the BST */	

	scanf("%d",&N);
	while((N--)!=0){
		scanf("%d",&num);
		insert(&root,num);
	}
	inorder(root);
	preorder(root);
	postorder(root);
}
