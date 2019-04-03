/* Program : To solve an equation in the Reverse Polish notation
 * Author : Anant Shah
 * Roll Number : EE16B105
 **/

#include<stdio.h>
#include<stdlib.h>

struct Node{
	double 		num; /* The value stored in the struct */
	struct Node 	*next; /* A pointer to data-type Node */
};

typedef struct Node nd;

int sizeofStack(nd **sptr){
	/* Function to return the size of the stack */
        if(*sptr==NULL) return 0;
        nd      *curptr; /* Pointer to a type Node to iterate through the stack */
        int     size = 0; /* Variable to store the size of the stack */

        curptr = *sptr;
        while(curptr->next!=NULL){
                size++;
                curptr = curptr->next;
        }
        size++;
        return size;
}

double 	pop(nd **sptr){
	/* Pop an element from the stack */
	if(sizeofStack(sptr)==0){
		printf("error : tried to pop() in an empty stack \n");
		exit(EXIT_FAILURE);
	}
	nd *oldptr = *sptr;
	*sptr = oldptr->next;
	return oldptr->num;
}

void push(nd **sptr,double val){
	/*Push an element onto the stack*/
	nd *newptr = malloc(sizeof(nd));
	nd *oldsptr;
	newptr->num = val;
	newptr->next=NULL;

	if(sizeofStack(sptr)==0){
		/* If the stack is empty, assign the start pointer as the new data element */
		*sptr = newptr;
		return;
	}
	oldsptr = *sptr;
	*sptr = newptr;
	newptr->next = oldsptr;
}

void printStack(nd *sptr){
	if(sizeofStack(&sptr)==0){
		printf("error : cannot print as stack is empty");
	}

	nd *curptr = sptr;
	while(curptr->next!=NULL){
		printf("%.4f->",curptr->num);
		curptr = curptr->next;
	}
	printf("%.4f->NULL\n",curptr->num);
}

int power(int base,int exp){
	/* Return the value of (base^exp) */
	if(base==0)	return 1;
	int prod=1;
	for(int i=0;i<exp;i++){
		prod = prod*base;
	}
	return prod;
}

double getNum(int *a,int size){
	/* Generate a decimal number from an integer array consisting of the digits, '-' sign or a decimal point */
	double num=0.0;
	int decimal = 0; /* At which index the decimal point was found */
	/* To generate the decimal number, we are finding the integer corresponding to the digits and then dividing it by power(10,decimal) */
	if(*a!=45){
		/* Handles a positive decimal number */
		for(int j=0;j<size;j++){
			if(*(a+j) == -2){
				/* If a decimal point was found, move to the next element */
				decimal = j;
				continue;
			}
			if(decimal!=0){
				num += *(a+j) * power(10,size-j); 
			}else{
				num += *(a+j) * power(10,size-1-j);
			}
		}
		if(decimal!=0)	num = (double)num/power(10,size-decimal);
	}else{
		/* Handles the case for a '-' sign */
		for(int j=1;j<size;j++){
			if(*(a+j) == -2){
				/* If a decimal point was found, move to the next element */
                                decimal = j;
                                continue;
                        }
                        if(decimal!=0){
                                num += *(a+j) * power(10,size-j);  
                        }else{
                                num += *(a+j) * power(10,size-1-j);
                        }	
		}
		if(decimal!=0)	num = (double)num/power(10,size-decimal);
		num = -1*num;
	}
	return num;
}

int main(int argc,char **argv){
	char		op_val;
	int		count=0;
	char		operation;
	double		operand1;
	double		operand2;
	int		invalid_char; /* Checks if the input element is invalid so that the rest of the inputs are not read */	
	char		a[256]; /* String to hold the input for each line */
	int		dig[20]; /* Array to hold the digits found in the string */
	int		char_counter=0; /* Counter   */
	int		dig_count=0; /* Counter to keep a track of the number of consecutive digits */
	double		num; /* Number to be added to the stack */
	int		sub_op=0; /* Identifier to check if the character was a subtraction operation */

	//dig = (int *)malloc(20*sizeof(int));

	while(fgets(a,256,stdin)){
		nd 	*sptr = NULL;
		char_counter = 0;
		invalid_char = 0;
		op_val = a[char_counter++];
		/* Keep obtaining characters till we do not reach the end of the line */
		while(op_val!='\n'){
			if(op_val==' '){
				op_val = a[char_counter++];
				continue;
			}
		
			if(sizeofStack(&sptr)<2){
				if((op_val=='+')||(op_val=='*')||(op_val=='/')){
					printf("ERROR\n");
					invalid_char=1;
					break;
				}else{
					/* Handles the case when a negative real number is encountered in the expression */
					while(op_val!=' '&&op_val!='\n'){
                                		/* While loop to create a number from the charcters read */
						if(op_val=='-'){
							dig[dig_count++] = op_val;
							//*(dig+(dig_count++)) = op_val;
							op_val = a[char_counter++];
							sub_op = 1;
						} else{
                                			dig[dig_count++] = op_val-'0';
							//*(dig+(dig_count++)) = op_val-'0';
                                			op_val = a[char_counter++];
						}
                        		}
					if(sub_op==1&&dig_count==1){
						/* To check if the character was just a '-' sign */
						printf("ERROR\n");
						invalid_char=1;
						dig_count = 0;
						sub_op = 0;
						break;
					}else{
						/* The string of characters is a negative real number */
                        			num = getNum(dig,dig_count);
						push(&sptr,num);
						dig_count = 0;
						sub_op = 0;
					}
				}
			}
			else{
				if(op_val=='+'){
					operand1 = pop(&sptr);
					operand2 = pop(&sptr);
					push(&sptr,operand1+operand2);
					op_val = a[char_counter++];
				} else if(op_val=='*'){
					operand1 = pop(&sptr);
					operand2 = pop(&sptr);
					push(&sptr,operand1*operand2);
					op_val = a[char_counter++];
				} else if(op_val=='/'){
					operand2 = pop(&sptr);
					operand1 = pop(&sptr);
					if(operand2==0){
						/* */
						printf("ERROR\n");
						invalid_char = 1;
						break;
					}
					push(&sptr,operand1/operand2);
					op_val = a[char_counter++];
				} else{
					while(op_val!=' '&&op_val!='\n'){
                                                /* While loop to create a number from the charcters read */
                                                if(op_val=='-'){
                                                        dig[dig_count++] = op_val;
                                                        op_val = a[char_counter++];
                                                        sub_op = 1;
                                                } else{
                                                        dig[dig_count++] = op_val-'0';
                                                        op_val = a[char_counter++];
                                                }
                                        }
                                        if(sub_op==1&&dig_count==1){
						operand2 = pop(&sptr);
                                        	operand1 = pop(&sptr);
                                        	push(&sptr,operand1-operand2);
                                        } else{
                                                num = getNum(dig,dig_count);
                                                push(&sptr,num);
                                        }
					dig_count = 0;
					sub_op = 0;
                                }
			}
		}
		if(invalid_char!=1){
			if(sizeofStack(&sptr)!=1)	printf("ERROR\n");
			else				printf("%.4f\n",pop(&sptr));
		}
	}
}
