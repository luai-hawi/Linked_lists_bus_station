#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node* Next;
};

void DeleteList (struct node* L){
    struct node* P,*temp;
    P=L->Next;
    L->Next=NULL;
    while(P!=NULL){
        temp=P->Next;
        free(P);
        P=temp;
    }
}
struct node* MakeEmpty(struct node* L){
    if(L != NULL)
        DeleteList(L);
    L = (struct node*)malloc(sizeof(struct node));
    if(L == NULL)
        printf("Out of memory!\n");
    L->Next = NULL;
    return L;
}
int IsEmpty(struct node* L){
    return L->Next==NULL;
}
int IsLast(struct node* P,struct node* L){
    return P->Next==NULL;
}
struct node* Find(int x,struct node* L){
    struct node* P=L->Next;
    while(P!=NULL&&P->data!=x){
        P=P->Next;
    }
    return P;
}
struct node* FindPrevious(int x,struct node* L){
    struct node* P=L;
    while(P->Next!=NULL&&P->Next->data!=x){
        P=P->Next;
    }
    return P;
}
void Delete(int x,struct node* L){
    struct node* P= FindPrevious(x,L),*temp;
    if(!IsLast(P,L)){
        temp = P->Next;
        P->Next=temp->Next;
        free(temp);
    }

}
void Insert(int x, struct node* P, struct node* L){
    struct node* temp;
    temp=(struct node*)malloc(sizeof(struct node));
    temp->data=x;
    temp->Next=P->Next;
    P->Next=temp;
}
void PrintList(struct node* L){
    struct node* P=L;
    if(IsEmpty(L)){
        printf("Empty list \n");
    }
    else {
        do {
            P = P->Next;
            printf("%d\t", P->data);
        } while(!IsLast(P,L));
            printf("\n");



    }
}

int size(struct node* L){
    struct node* P=L->Next;
            int count=0;
            while(P!=NULL){
                count++;
                P=P->Next;
            }
            return count;
}