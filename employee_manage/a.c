#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct a
{
    int data;
    struct a *next;
}list;

list* creat_list(int data)
{
    list *q=(list *)malloc(sizeof(list));
    if (q==NULL)
    {
        return NULL;
    }
    q->data=data;
    q->next=NULL;
    return q;
}


list* init_list()
{
    list *head=(list *)malloc(sizeof(list));
    if (head==NULL)
    {
        return NULL;
    }
    head->next=NULL;
    return head;
}

void insert(list *head,int data)
{
    if (head==NULL)
    {
        return ;
    }
    list *p=head;
    list *new_node=creat_list(data);
    while(p->next!=NULL)
    {
        p=p->next;
    }
    p->next=new_node;
}

void print_list(list *head)
{
    if(head->next==NULL)
    {
        printf("Пе\n");
        return;
    }
    if(head==NULL) return;
    list *p=head->next;
    while(p!=NULL)
    {
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");
}

list* reverse_list(list *head)
{
    if(head == NULL || head->next == NULL)
    {
        return NULL;
    }
    list *pre = NULL;
    list *cur = head->next;  
    list *next = NULL;

    while(cur != NULL)
    {
        next = cur->next; 
        cur->next = pre;  
        pre = cur;
        cur = next;
    }
    return pre;
}

int main()
{
    list  *p=init_list();
    insert(p,1);
    insert(p,2);
    insert(p,3);
    insert(p,4);
    insert(p,5);
    print_list(p);
    p->next=reverse_list(p);
    print_list(p);


}