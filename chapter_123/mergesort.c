#include <stdio.h>
#include <stdlib.h>

typedef struct Lnode{
  int data;
  Lnode *next;
}Lnode, *Linklist;

void mergesort(Linklist ,Linklist ,Linklist );
void init_Linklist(Linklist, int , int );
void traverse(Linklist );

int main(void)
{
  Linklist La = (struct Lnode *)malloc(sizeof(Lnode));
  La->next = NULL;
  Linklist Lb = (struct Lnode *)malloc(sizeof(Lnode));
  Lb->next = NULL;
  Linklist Lc = (struct Lnode *)malloc(sizeof(Lnode));
  Lc->next = NULL;

  init_Linklist(La,4,2);
  traverse(La);
  init_Linklist(Lb,6,1);
  traverse(Lb);
  
  mergesort(La,Lb,Lc);
  traverse(Lc);

  traverse(La);
  traverse(Lb);
  return 0;
}

void traverse(Linklist L)
{
  Linklist p;
  p = L->next;
  while(p)
  {
    printf("%d ",p->data);
    p = p->next;
  }
  printf("\n");
}

void init_Linklist(Linklist L,int m,int n)
{
  int i;
  Linklist q = L;
  for(i=0;i<m;i++)
  {
    Linklist p = (struct Lnode *)malloc(sizeof(Lnode));
    p->next = NULL;
    p->data = i*n;
    q->next = p;
    q = p;
  }
  
}

void mergesort(Linklist La,Linklist Lb,Linklist Lc)
{
  Linklist pa,pb,pc;
  pa = La->next;
  pb = Lb->next;
  Lc = pc = La;
  while(pa && pb)
  {
    if(pa->data <= pb->data)
    {     
      pc->next = pa;
      pc = pa;
      pa = pa->next;
    }
    else{    
      pc->next = pb;
      pc = pb;
      pb = pb->next;
    }
  }  
  pc->next = pa?pa:pb;
  free(Lb);
}






