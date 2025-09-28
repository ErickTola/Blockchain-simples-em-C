#include <stdio.h>
#include <stdlib.h>
#include "bloco.c"

struct node {

    int vetor;
    struct b *blc;
    struct node *link;
};



struct node *comecaLista(struct b *x){

    struct node *cabeca = NULL;
    cabeca = (struct node*)malloc(sizeof(struct node));

    if (cabeca == NULL){
        exit(1);
    }

    cabeca->vetor = 0;
    cabeca->blc = x;
    cabeca->link = NULL;

    return cabeca;
}

void addLista(struct node *anterior,bloco *x){

    struct node *no = NULL;
    no = (struct node*)malloc(sizeof(struct node));

    if (no == NULL){
        exit(1);
    }

    no->vetor = anterior->vetor + 1;
    no->blc = x;
    no->link = NULL;
    anterior->link = no;

}