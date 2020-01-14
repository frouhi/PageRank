#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "indexFiles.h"
/**************************************/
/*Farhang Rouhi                       */
/*Date: 3 May 2018                    */
/*Lab#8                               */
/**************************************/
float pageRank=0;
/*This is what is called from outside to also get page rank*/
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
    token=strtok(line, " .?!@$*/-+,<>~#%^&(){}[]\"\'\\_–1234567890");
    while (token != NULL)
    {
      struct HashElt* elt=malloc(sizeof(struct HashElt));
      elt->url=malloc(sizeof(char)*(strlen(fn)+1));
      strcpy(elt->url,fn);
      elt->token=malloc(sizeof(char)*(strlen(token)+1));
      strcpy(elt->token,token);
      elt->next=NULL;
      elt->pageRank=pageRank;
      insert(elt, table);
      token = strtok(NULL, " .?!@$*/-+,<>~#%^&(){}[]\"\'\\_–1234567890");
    }
  }
  free(line);
  fclose(in);
}
