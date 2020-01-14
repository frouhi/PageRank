#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "indexFiles.h"
#include "graph.h"
/**************************************/
/*Farhang Rouhi                       */
/*Date: 3 May 2018                    */
/*Lab#8                               */
/**************************************/
void printPageRank(struct graph* g);
void calculatePageRank(struct graph* g);
struct graph* readGraph(char* fileName);
void readInFileAndInsert(float pr, char* fn, struct HTable* table);
/*My main function conects the dots and uses all other files and functions
 *to each other to perform the search*/
int main(int argc, char **argv)
{
  struct graph* graph=malloc(sizeof(struct graph));
  struct HTable* table=(struct HTable*) malloc(sizeof(struct HTable));
  struct HashElt** elt=malloc(sizeof(struct HashElt)*5);
  struct HashElt* eltTemp;
  int i=0;
  struct node* node;
  for(i=0;i<strlen(argv[2]);i++)
  {
    if(argv[2][i]>='A'&&argv[2][i]<='Z')
      {
        argv[2][i]=argv[2][i]-('A'-'a');
      }
  }
  for(i=0;i<5;i++)
  {
    elt[i]=NULL;
  }
  graph=readGraph(argv[1]);
  calculatePageRank(graph);
  node=graph->firstNode;
  table->size=2000;
  table->n=0;
  for(i=0;i<2000;i++)
  {
    (table->table)[i]=NULL;
  }
  while(node!=NULL)
  {
    readInFileAndInsert(node->pagerank,node->url, table);
    node=node->next;
  }
  eltTemp=lookup(argv[2],table);
  while(eltTemp!=NULL)
  {
    if(strcmp(eltTemp->token,argv[2])==0)
    {
      for(i=0;i<5;i++)
      {
        if(elt[i]==NULL || (eltTemp->pageRank)>(elt[i]->pageRank))
        {
          struct HashElt* temp1=elt[i];
          elt[i]=eltTemp;
          for(i=i+1;i<5;i++)
          {
            struct HashElt* temp2=elt[i];
            elt[i]=temp1;
            temp1=temp2;
          }
        }
      }
    }
    eltTemp=eltTemp->next;
  }
  for(i=0;i<5;i++)
  {
    if(elt[i]!=NULL)
    {
      printf("%s\n", elt[i]->url);
    }
  }
  free(table);
  free(graph);
  free(elt);
  return 0;
}
