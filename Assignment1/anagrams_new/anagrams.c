/* Program : To find all the anagrams in a list of words
 * Author : Anant Shah
 * Roll Number : EE16B105
 * Date : 8-8-2018
 **/

#include "include/anagrams.h"
#define MAX_LEN 30

void swapCharacters(int ptr1,int ptr2, char *str){
        int temp;
        temp = str[ptr1];
        str[ptr1] = str[ptr2];
        str[ptr2] = temp;
}

int PartitionCharacters(int l,int r,char *str){
	/* Return the quick sort partition index for the quick sort algorithm */
	int partition = r;
	int lptr = l;
	int rptr = r-1;
	int temp;	

	while(lptr<=rptr)
	{
		while(str[lptr]<str[partition])			lptr++;
		while((str[rptr]>str[partition])&&(rptr>0))	rptr--;
		/* I need to swap these two elements */
		if(lptr==partition)	return partition;
		if(rptr<=lptr){
			swapCharacters(lptr,partition,str);
			return lptr;
		} 
		swapCharacters(lptr,rptr,str);
		lptr++;
		rptr--;
	}

	swapCharacters(lptr,partition,str);
	return lptr;
}


void QuickSortCharacters(int l,int r,char *str){
	/* Sort a string using the quick sort method */
	if(r<=l)	return;
	int p = PartitionCharacters(l,r,str);
	QuickSortCharacters(l,p-1,str);
	QuickSortCharacters(p+1,r,str);
}

int ifAnagramExists(int index,int *index_hits,int size){
	for(int i=0;i<size;i++){
		if(index==index_hits[i])	return 1;
	}
	return 0;
}

int str_length(char *str){
	int length=0;
	int i=0;
	while(*(str+i)!='\0'){
		length++;
		i++;
	}
	return length;
}

int str_compare(char *str1,char *str2){
	int i=0;
	char ch1;
	char ch2;
	while((*(str1+i)!='\0')||(*(str2+i)!='\0')){
		ch1 = *(str1+i);
		ch2 = *(str2+i);
		if((ch1>=65&&ch1<=90)&&(!(ch2>=65&&ch2<=90))){
			if((ch1+32)>ch2)	return 1;
			else if((ch1+32)<ch2)	return -1;
		}
		else if((ch2>=65&&ch2<=90)&&(!(ch1>=65&&ch1<=90))){
			if(ch1>(ch2+32))	return 1;
			else if(ch1<(ch2+32))	return -1;
		}
		else {
			if(ch1>ch2)		return 1;
			else if(ch1<ch2)	return -1;
		}
		i++;
	}
	if((*(str1+i)=='\0')&&(*(str2+i)!='\0'))                        return -1;
        else if((*(str2+i)=='\0')&&(*(str1+i)!='\0'))                   return 1;
        else                                                            return 0;
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
                while(str_compare(*(str+lptr),*(str+partition))<0)                 		lptr++;
                while(str_compare(*(str+rptr),*(str+partition))>0&&rptr>0)     			rptr--;
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
	if(r<=l)	return;
	int p = DictionaryPartition(l,r,str);
	CreateDictionary(l,p-1,str);
	CreateDictionary(p+1,r,str);
}

int main(int argc,char **argv){
	
	int 	N; /* Number of strings in the dictionary */
	
	scanf("%d",&N);	
	
	char 	*str[N]; /* An array which will store all the strings from the input file */
	char 	*str_sort[N]; /* An array which will contain all the strings which are sorted */
	int	num_anagrams=0; /* Number of anagrams found in the list of words */
	int	*index_hits; /* An array which contains all the indices where the word had an anagram, this will be used to avoid further comparisons */
	int 	anagram_word_count; /* Number of anagrams for a given word */	
	int	*anagram_word; /* Array consisting of the number of anagrams for a given word */
	int	index_hit_counter=0;	
	int 	anagram_word_counter=0;
	int 	str_size;

	for(int i=0;i<N;i++){
		str[i] = malloc(MAX_LEN*sizeof(char));
		str_sort[i] = malloc(MAX_LEN*sizeof(char));
		scanf("%s",str[i]);
		str_size = str_length(str[i]);
                for(int j=0;j<str_size;j++){
                        *(str_sort[i]+j) = *(str[i]+j);
                }
                QuickSortCharacters(0,strlen(str_sort[i])-1,str_sort[i]);
	}
	
	/* for(int i=0;i<N;i++){
		str_size = str_length(str[i]);
                for(int j=0;j<str_size;j++){
                    	*(str_sort[i]+j) = *(str[i]+j);
                }
                QuickSortCharacters(0,strlen(str_sort[i])-1,str_sort[i]);
	} */

	/* We have all the sorted strings in an array
	 * We now need to compare them and see which of them are equal 
 	 **/

	index_hits = malloc(N*sizeof(int));
	anagram_word = malloc(N*sizeof(int)); /* At most each word will have 1 anagram (worst case scenario) */

	for(int i=0;i<N;i++){
		if(i!=0&&num_anagrams!=0){
			if(ifAnagramExists(i,index_hits,index_hit_counter)==1)	continue;
		} 
		anagram_word_count=0;
		for(int j=i+1;j<N;j++){
			if(strcmp(str_sort[i],str_sort[j])==0)
			{
				anagram_word_count++;
				if(anagram_word_count==1){
					*(index_hits+index_hit_counter)=i;
					index_hit_counter++;
				}
				*(index_hits+index_hit_counter)=j;
				index_hit_counter++;
			}
		}
		if(anagram_word_count!=0){
			num_anagrams++;
			*(anagram_word+anagram_word_counter)=anagram_word_count+1;
			anagram_word_counter++;
		}
	}

	int 	count=0;
	int 	iter=0;
	char	**anagram_set[num_anagrams];
	int	k=0;

	printf("%d\n",num_anagrams);
	for(int i=0;i<num_anagrams;i++){
		printf("%d\n",*(anagram_word+i));
		anagram_set[i] = malloc(*(anagram_word+i)*sizeof(char *));
		while(iter < *(anagram_word+i)){
			*(anagram_set[i]+iter) = str[*(index_hits+count)];
			count++;
			iter++;
		}	
		
		CreateDictionary(0,*(anagram_word+i)-1,anagram_set[i]);		

		while(k < *(anagram_word+i)){
			printf("%s\n",*(anagram_set[i]+k));
			k++;
		} 
		iter=0;
		k=0; 
	}  	
}
