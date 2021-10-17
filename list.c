#include <stdio.h>
#include <stdlib.h>

struct stru_list
{
  int data;
  struct stru_list *next;
};


void list_print(struct stru_list * head)
{
  struct stru_list *ptr = head;
  printf("n[head] =>");
  while(ptr != NULL)
  {
    printf(" %d =>",ptr->data);
    ptr = ptr->next;
  }
  printf(" [null]n");
}

struct stru_list * list_insert(struct stru_list * head, int data)
{
  struct stru_list *link = (struct stru_list*) malloc(sizeof(struct stru_list));
  link->data = data;
  link->next = head;
  return link;
}

int list_get(struct stru_list * head, int key)
{
  int resp = 0;
  int n = 0;
  struct stru_list *ptr = head;
  while(ptr != NULL)
  {
    if(n==key)
    {
      resp = ptr->data;
      ptr = NULL;
    }
    else 
    {
      ptr = ptr->next;
    }
    n++;
  }
  return resp;

}
