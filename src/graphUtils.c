#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
/**********************************/
/*Farhang Rouhi                   */
/*Date: 22 April 2018             */
/*lab#8: part 1                   */
/**********************************/
float* PRhelper(struct graph* g,float* pr, int* outNum,int nodeCount);
/*This struct is for the linked list of nodes used to implement
 *stack and q*/
struct qnode
{
  struct node* node;
  struct qnode* next;
};
/*This is the push function for both stack and Q*/
struct qnode* push (struct qnode* head, struct node* node)
{
  struct qnode* qnode=malloc (sizeof(struct qnode*));
  qnode->node=node;
  qnode->next=head;
  return qnode;
}
/*This is the pop dunction for stack*/
struct node* popStack (struct qnode** head)
{
  struct node* popedNode;
  struct qnode* temp;
  if(*head==NULL) return NULL;
  temp=(*head);
  *head=(*head)->next;
  popedNode=temp->node;
  free(temp);
  return popedNode;
}
/*This is the pop funstion for Q*/
struct node* popQ (struct qnode** head)
{
  struct node* popedNode;
  if(*head==NULL) return NULL;
  if((*head)->next==NULL)
    {
      popedNode=(*head)->node;
      free(*head);
      *head=NULL;
      return popedNode;
    }
  while((((*head)->next)->next)!=NULL)
    {
      *head=(*head)->next;
    }
  popedNode=((*head)->next)->node;
  free((*head)->next);
  (*head)->next=NULL;
  return popedNode;
}
/*This function sets all nodes to unvisited*/
void unvisitify (struct graph* g)
{
  struct node* node=g->firstNode;
  while(node->next !=NULL)
    {
      node->visited=0;
      node=node->next;
    }
}
/* Uses depth first search to find the URL in the graph.
   Returns 1 if url is in the graph. */
int depthFirstSearch(struct graph* g, char* url)
{
  struct node* node;
  struct edge* edge;
  struct qnode* head=NULL;
  head=push(head,g->firstNode);
  while((node=popStack(&head))!=NULL)
    {
      if(!strcmp(node->url,url))
	{
	  unvisitify(g);
	  return 1;
	}
      node->visited=1;
      edge=node->edges;
      while(edge!=NULL)
	{
	  if(!(edge->connectsTo)->visited)
	    {
	      head=push(head,edge->connectsTo);
	    }
	  edge=edge->next;
	}
    }
  unvisitify(g);
  return 0;
}

/* Uses breadth first search to find the URL in the graph.
   Returns 1 if url is in the graph. */
int breadthFirstSearch(struct graph* g, char* url)
{
  struct node* node;
  struct edge* edge;
  struct qnode* head=NULL;
  head=push(head,g->firstNode);
  while((node=popQ(&head))!=NULL)
    {
      if(!strcmp(node->url,url))
	{
	  unvisitify(g);
	  return 1;
	}
      node->visited=1;
      edge=node->edges;
      while(edge!=NULL)
	{
	  if(!((edge->connectsTo)->visited))
	    {
	      head=push(head,edge->connectsTo);
	    }
	  edge=edge->next;
	}
    }
  unvisitify(g);
  return 0;
}
int flag=0;
/* calculates the pagerank of each node in the graph.*/
void calculatePageRank(struct graph* g)
{
  int nodeCount=0;
  struct node* node=g->firstNode;
  struct edge* edge;
  float* temppr;
  float* pr;
  int* outNum;
  int out,counter,i=0;
  while(node!=NULL)
    {
      nodeCount++;
      node=node->next;
    }
  pr=calloc(nodeCount,sizeof(float));
  outNum=calloc(nodeCount,sizeof(int));
  node=g->firstNode;
  counter=0;
  while(node!=NULL)
    {
      out=0;
      edge=node->edges;
      while(edge!=NULL)
	{
	  out++;
	  edge=edge->next;
	}
      outNum[counter]=out;
      counter++;
      node=node->next;
    }
  for(i=0;i<nodeCount;i++)
    {
      pr[i]=1;
    }
  counter=0;
  while(!flag && counter<100)
    {
      temppr=pr;
      pr=PRhelper(g,pr,outNum,nodeCount);
      free(temppr);
      counter++;
    }
  node=g->firstNode;
  counter=0;
  while(node!=NULL)
    {
      node->pagerank=pr[counter];
      counter++;
      node=node->next;
    }
}
/*This is a helper that calculates each round of PR*/
float* PRhelper(struct graph* g,float* pr, int* outNum,int nodeCount)
{
  struct node* node=g->firstNode;
  struct node* tempNode=node;
  struct edge* edge;
  float* newPR=malloc(nodeCount*sizeof(float));
  int counter=0;
  int innerCounter=0;
  float innerpart=0;
  while(node!=NULL)
    {
      innerCounter=0;
      while(tempNode!=NULL)
	{
	  edge=tempNode->edges;
	  while(edge!=NULL)
	    {
	      if(edge->connectsTo==node)
		{
		  innerpart+=pr[innerCounter]/outNum[innerCounter];
		}
	      edge=edge->next;
	    }
	  innerCounter++;
	  tempNode=tempNode->next;
	}
      newPR[counter]=innerpart*0.85+0.15;
      innerpart=0;
      tempNode=g->firstNode;
      counter++;
      node=node->next;
    }
  for(counter=0;counter<nodeCount;counter++)
    {
      if(pr[counter]!=newPR[counter]) return newPR;
    }
  flag=1;
  return newPR;
}
/* prints out each URL and their associated pagerank*/
void printPageRank(struct graph* g)
{
  struct node* node=g->firstNode;
  while(node!=NULL)
    {
      printf("%s,%.2f\n", node->url, node->pagerank);
      node=node->next;
    }
}

/* reads in the graph from file and stores it in a graph object */
struct graph* readGraph(char* fileName)
{
  struct graph* g=malloc(sizeof(struct graph*));
  FILE* in = fopen(fileName,"r");
  /*char* node=realloc(NULL, sizeof(char)*20);
    char* edge=realloc(NULL, sizeof(char)*20);
    int sizeNode=20;
    int sizeEdge=20;*/
 
  struct node* gnode;
  int c,counter;
  g->firstNode=NULL;
  while((c=getc(in))!=EOF)
    {
       char* node=malloc(sizeof(char)*50);
       char* edge=malloc(sizeof(char)*50);
      node[0]=c;
      counter=1;
      while((c=getc(in))!=',')
	{
	  /*if(counter==sizeNode-1)
	    {
	      sizeNode+=10;
	      node=realloc(node,sizeof(char)*(sizeNode));
	    }*/
	  node[counter]=c;
	  counter++;
	}
      node[counter]='\0';
      counter=0;
      while((c=getc(in))!='\n' && c!=EOF)
	{
	  /*if(counter==sizeEdge-1)
	    {
	      sizeEdge+=10;
	      edge=realloc(edge,sizeof(char)*(sizeEdge));
	      }*/
	  edge[counter]=c;
	  counter++;
	}
	edge[counter]='\0';
      if(g==NULL)
	{
	  insertNode(g,node);
	  insertNode(g,edge);
	  insertEdge(g,node,edge);
	}
      else
	{
	  gnode=g->firstNode;
	  counter=0;
	  while(gnode!=NULL)
	    {
	      if(!strcmp(node,gnode->url))
		{
		  counter+=1;
		}
	      if(!strcmp(edge,gnode->url))
		{
		  counter+=2;
		}
	      gnode=gnode->next;
	    }
	  if(counter==2 || counter==0)
	    {
	      insertNode(g,node);
	    }
	  if(counter==1 || counter==0)
	    {
	      insertNode(g,edge);
	    }
	  insertEdge(g,node,edge);
	}
      /*node=realloc(NULL, sizeof(char)*20);
      edge=realloc(NULL, sizeof(char)*20);
      sizeNode=20;
      sizeEdge=20;printf("%s,,,%s", node, edge);
        free(node);
	free(edge);*/
    }
  
  fclose(in);
  return g;
}
/*My main function is only used to test and show how the code works*/
/*int main(void)
{
  struct graph* g=readGraph("simplegraph.txt");
  calculatePageRank(g);
  printPageRank(g);
  printf("depthFirstSearch for H: %d\n",depthFirstSearch(g,"H"));
  printf("depthFirstSearch for D: %d\n",depthFirstSearch(g,"D"));
  printf("depthFirstSearch for B: %d\n",depthFirstSearch(g,"B"));
  printf("breadthFirstSearch for H: %d\n",breadthFirstSearch(g,"H"));
  printf("breadthFirstSearch for D: %d\n",breadthFirstSearch(g,"D"));
  printf("breadthFirstSearch for B: %d\n",breadthFirstSearch(g,"B"));
  return 0;
}*/


