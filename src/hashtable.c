#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
void freeElt(struct HashElt* elt);
/* Turns the token from the HashElt into a number by adding the ascii values of each char in the token string*/
int hash(struct HashElt* elt)
{
  char* token=elt->token;
  int result=0;
  int i=0;
  for(i=0;token[i];i++)
    {
      result+=token[i];
    }
  return result;
}
/* Hash the element and put it in the hashtable. If the element is already in the hashtable, do nothing.*/
void insert(struct HashElt* elt, struct HTable* table)
{
  struct HashElt* head=(table->table)[hash(elt)%(table->size)];
  if(head==NULL)
    {
      (table->table)[hash(elt)%(table->size)]=elt;
      (table->n)++;
      return;
    }
  /*printf("%s\n", head->token);*/
  while(head->next!=NULL)
    {
      if(strcmp(elt->url,head->url)==0 && strcmp(elt->token,head->token)==0)
	{
	  freeElt(elt);
	  return;
	}
      head=head->next;
    }
  if(strcmp(elt->url,head->url)!=0 || strcmp(elt->token,head->token)!=0)
    {
      head->next=elt;
      /*insertNewElt(head,elt);*/
      (table->n)++;
      return;
    }
  freeElt(elt);
  return;
}
/*void insertNewElt(struct HashElt* head, struct HashElt* newElt)
{
  struct HashElt* elt=malloc(sizeof(struct HashElt*));
  elt->url=malloc(sizeof(char)*(strlen(newElt->url)+1));
  strcpy(elt->url,newElt->url);
  elt->token=malloc(sizeof(char)*(strlen(newElt->token)+1));
  strcpy(elt->token,newElt->token);
  elt->pageRank=newElt->pageRank;
  head->next=elt;
  }*/
void freeElt(struct HashElt* elt)
{
  free(elt->token);
  free(elt->url);
  free(elt);
}
/* returns a linked list of everything that's token hashes to the same value as key */
struct HashElt* lookup(char* key, struct HTable* table)
{
  int result=0;
  int i=0;
  struct HashElt* head;
  for(i=0;key[i];i++)
    {
      result+=key[i];
    }
  head=(table->table)[result%(table->size)];
  while(head!=NULL)
    {
      if(strcmp(head->token,key)==0)
	{
	  return head;
	}
      head=head->next;
    }
  return NULL;
}
