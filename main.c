#include <stdio.h>
#include <stdlib.h>

struct stru_list
{
  int data;
  struct stru_list *next;
};


void printList(struct stru_list * head)
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

struct stru_list * insert(struct stru_list * head, int data)
{
  struct stru_list *link = (struct stru_list*) malloc(sizeof(struct stru_list));
  link->data = data;
  link->next = head;
  return link;
}

int get(struct stru_list * head, int key)
{
  int resp = NULL;
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

int main()
{

  struct stru_list *head = NULL;
  struct stru_list *current = NULL;

  head = insert(head, 1000);
  head = insert(head, 60);
  head = insert(head, 40);
  printList(head);

  printf("> %d <", get(head, 5));


  return 0;
}
