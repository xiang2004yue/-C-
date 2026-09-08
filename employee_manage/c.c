#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct tree
{
    int data;
    struct tree *right;
    struct tree *light;
}Tree;

Tree *creatnode(int value)
{
    Tree *head=(Tree  *)malloc(sizeof(Tree));
    head->data=value;
    head->light=NULL;
    head->right=NULL;
    return head;
}

void perorder(Tree *head)
{
    if(head==NULL) return ;
    printf("%d ",head->data);
    perorder(head->light);
    perorder(head->right);
}

void order(Tree *head)
{
    if(head==NULL) return ;
    order(head->light);
    printf("%d ",head->data);
    order(head->right);
}

void lestorder(Tree *head)
{
    if(head==NULL) return ;
    lestorder(head->light);
    lestorder(head->right);
    printf("%d ",head->data);
}

int getnode(Tree  *head)
{
    if(head==NULL)
    return 0;
    return 1+getnode(head->light)+getnode(head->right);
}

int seekheight(Tree *head)
{
    if(head==NULL)
    return 0;
    int left=seekheight(head->light);
    int right=seekheight(head->right);
    return left>right?left+1:right+1;
}

Tree *find(Tree *head,int value)
{
    if(head==NULL) return NULL;
    if(head->data==value)
    return head;
    Tree *a=find(head->light,value);
    if(a!=NULL) return a;
    return find(head->right,value);
}

int  main()
{
    Tree *p=creatnode(1);
    p->light=creatnode(2);
    p->right=creatnode(3);
    p->light->light=creatnode(4);
    perorder(p);
    printf("\n");
    order(p);
    printf("\n");
    lestorder(p);
    printf("\n");
    int a=getnode(p);
    printf("%d",a);
    printf("\n");
    printf("%d\n",seekheight(p));
    return 0;
}