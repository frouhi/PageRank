#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "indexFiles.h"
float pageRank=0;
void readInFileAndInsert(float pr, char* fn, struct HTable* table)
{
  pageRank=pr;
  readInFile(fn,table);
}
/* Reads in a file and stores each element in the hashtable */
/* Ensure that a file name 'fn' is in the current working directory */
void readInFile(char* fn, struct HTable* table)
{
  FILE* in = fopen(fn,"r");
  char* line;
  char* token;
  char c;
  int counter=0;
  int size;
  /*extern struct graph* graph;
  if(graph!=NULL)
    {
      struct node* node=graph->firstNode;
      while(node!=NULL)
	{
	  if(strcmp(fn,node->url)==0)
	    {
	      pageRank=node->pageRank;
	}
    }*/
  if(in==NULL)
    {
      printf("%s is not in the current directory!\n", fn);
      return;
    }
  while((c=getc(in))!=EOF)
    {
      line=realloc(NULL, sizeof(char)*20);
      size=20;
      while(c=='\n')
	{
	  c=getc(in);
	}
      if(c>='A' && c<='Z')
	{
	  c=c-('A'-'a');
	}
      line[0]=c;
      counter=1;
      while((c=getc(in))!='\n' && c!=EOF)
	{
	  if(counter==size-1)
	    {
	      size+=10;
	      line=realloc(line,sizeof(char)*(size));
	    }
	  if(c>='A' && c<='Z')
	    {
	      c=c-('A'-'a');
	    }
	  line[counter]=c;
	  counter++;
	}
      line[counter]='\0';
      token=strtok(line, " .?!@$*/-+,<>~#%^&(){}[]\"\'\\");
      while (token != NULL)
	{
	  struct HashElt* elt=malloc(sizeof(struct HashElt));
	  elt->url=malloc(sizeof(char)*(strlen(fn)+1));
	  strcpy(elt->url,fn);
	  elt->token=malloc(sizeof(char)*(strlen(token)+1));
	  strcpy(elt->token,token);
	  /*elt->url=fn;
	    elt->token=token;*/
       	  elt->next=NULL;
	  elt->pageRank=pageRank;
	  insert(elt, table);
	  token = strtok(NULL, " .?!@$*/-+,<>~#%^&(){}[]\"\'\\");
	  /*free(elt);*/
	}
    }
  free(line);
  fclose(in);
}
/*int main()
{
  struct HTable* table=malloc(sizeof(struct HTable));
  struct HashElt* elt=malloc(sizeof(struct HashElt));
  int i=0;
  table->size=2000;
  table->n=0;
  for(i=0;i<2000;i++)
    {
      (table->table)[i]=NULL;
    }
  
  readInFile("Computer", table);
  elt=lookup("smartphones", table);
  printf("%s,,,,%s)",elt->token, elt->url);
  if(lookup("farhang",table)==NULL) printf("farhang == NULL\n");
  return 0;
}*/
