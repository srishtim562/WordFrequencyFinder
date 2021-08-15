#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct node
{
	char word[100];
	int freq;
	struct node* next;
};

int hash(char c)
{
	if(islower(c))
	       return c - 97;
	if(isupper(c))
		return c - 65;	
}

void to_lower(char w[])
{
        int i;
        for(i = 0; w[i]!='\0';i++)
        {
                w[i] = tolower(w[i]);
        }
}


struct node* getnode()
{
	struct node* p = (struct node*)malloc(sizeof(struct node));
	if(p == NULL)
	{
		printf("Insufficient Memory\n");
		exit(0);
	}

	p->next = NULL;
	p->freq  = 0;
	return p;
}

struct node *insert(struct node *p, char *w)
{
	struct node *q , *prev = NULL;
	int flag = 0;
	q = getnode();
	to_lower(w);
	strcpy(q->word, w);
	
	if(p == NULL)
	{
		p = q;
		q->freq++;
		return p;
	}

	struct node *r;
	r = p;
	
	while( r != NULL)
	{
		if(strcmp(w,r->word) == 0)
		{
			flag = 1;
			r->freq++;
			free(q);
			break;
		}
		prev = r;
		r = r->next;
	}

	if(flag == 0)
	{
		prev->next = q;
		q->freq++;
	}

	return p;

}

int findfreq(struct node *p, char* w)
{
	if( p == NULL)
	{
		return 0;
	}
	else if(strcmp(w,p->word) == 0)
	{
		return p->freq;
	}
	else
	{
		return findfreq(p->next,w);
	}
}


void display(struct node *p)
{
        if(p!=NULL)
        {
                printf("\n%s: %d", p->word, p->freq);
                display(p->next);
        }
}


void displayall(struct node *list[])
{
	int i;
	for(i=0; i<26; i++)
	{
		display(list[i]);
	}
}


int mostfreq(struct node *p)
{
	int max = 0;
	while(p!=NULL)
	{
		if(p->freq>max)
			max = p->freq;

		p = p->next;
	}

	return max;
}

void disp_mostfreqall(struct node *list[])
{
	struct node *p;
	int i, max_full_list = 0, max;

	for(i=0; i<26; i++)
	{
		max = mostfreq(list[i]);
		if(max > max_full_list)
			max_full_list = max;
	}

	for(i=0; i<26; i++)
	{
		p = list[i];
		while(p!=NULL)
		{
			if(p->freq == max_full_list)
			{
				printf("\n%s: %d",p->word, p->freq);
			}

			p = p->next;
		}
	}
}


int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: ./wff filename\n");
		exit(0);
	}

	int i, choice, j = 0, k;

	struct node *list[26];
	for(i = 0; i < 26; i++)
	{
		list[i] = NULL;
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == (signed int)-1)
	{
		printf("Could not open file\n");
		exit(0);
	}
	char* buff = malloc(sizeof(char));
	int index;
	char word[100], searched[100][100];

	i = 0;

	//creating hash table
	while(read(fd,buff,1))
	{
		if(*buff == ' ' || *buff == ',' || *buff == '.' || *buff == '"' || *buff == ')' || *buff == '\n' || *buff == ':' || *buff == ';' || *buff == ']' || *buff == '}')
		{
			word[i] = '\0';
			to_lower(word);
			if (strcmp(word," ") != 0 && strcmp(word, "")!=0)
			{
				index = hash(word[0]);
				list[index] = insert(list[index], word);
			}
			i = 0;
			strcpy(word,"");
			continue;
		}
		if(*buff == '(' || *buff == '"' || *buff == ' ' || *buff == '[' || *buff == '{')
			continue;

		word[i++] = *buff;	
	}
	while(1)
	{
		printf("\n1. Find frequency of one word\n2. Display frequency of all words\n3. Display frequency of all searched words\n4. Dispay most frequent word\n5. Exit\n\n");	
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:  printf("Enter the word\n");
			         scanf("%s", word);
       				 to_lower(word);
				 strcpy(searched[j++],word);
  			         index = hash(word[0]);
    			         printf("%d\n", findfreq(list[index],word));
				 break;

			case 2:  displayall(list);
				 printf("\n");
				 break;

			case 3:  for(k=0; k<j; k++)
				 {
					 index = hash(searched[k][0]);
					 printf("\n%s: %d",searched[k], findfreq(list[index],searched[k]));
				 }
				 printf("\n");
				 break;

			case 4:  disp_mostfreqall(list);
				 printf("\n");
				 break;

			case 5:  exit(0);

			default: printf("\nInvalid choice\n");

		}
	}

	return 0;
}
