#include "bloco.c"
#include "rdnStr.c"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

bool ArqExiste(const char *filename) {
  FILE *fp = fopen(filename, "r");
  bool existe = false;
  if (fp != NULL) {
    existe = true;
    fclose(fp);
  }
  return existe;
}

struct node {

  char codigo[15];
  int vetor;
  struct b *blc;
  struct node *link;
};

struct node *comecaLista(struct b *x) {

  struct node *cabeca = NULL;
  cabeca = (struct node *)malloc(sizeof(struct node));

  if (cabeca == NULL) {
    exit(1);
  }
  char nome[15];
  do {
    geraNome(nome, 15);
  } while (ArqExiste(nome) == true);

  strcpy(cabeca->codigo, nome);
  cabeca->vetor = 0;
  cabeca->blc = x;
  cabeca->link = NULL;

  return cabeca;
}

void addLista(struct node *anterior, bloco *x) {

  struct node *no = NULL;
  no = (struct node *)malloc(sizeof(struct node));

  if (no == NULL) {
    exit(1);
  }
  strcpy(no->codigo, anterior->codigo);
  no->vetor = anterior->vetor + 1;
  no->blc = x;
  no->link = NULL;
  anterior->link = no;
}

struct node *abrirLista(char *codigo,bloco *bloco,struct node *ultimoBlcAberto,struct node **cabeca) {

  struct node *no = NULL;
  no = (struct node *)malloc(sizeof(struct node));

  if (no == NULL) {
    exit(1);
  }

  if (ultimoBlcAberto == NULL){
    strcpy(no->codigo,codigo);
    no->vetor = 0;
    no->blc = bloco;
    no->link = NULL;
    *cabeca = no;
  }else{
    strcpy(no->codigo,codigo);
    no->vetor = ultimoBlcAberto->vetor + 1;
    no->blc = bloco;
    no->link = NULL;
    ultimoBlcAberto->link = no;
  }
  return no;
}

void arrumalista(struct node **cabeca){

  int tamanhoLista = 0;
  struct node *ptr = *cabeca;
  struct node *anterior;

  while(ptr->link != NULL){
    ptr = ptr->link;
    tamanhoLista++;
  }
  ptr = *cabeca;

  while(ptr->link != NULL){
    struct b * temp;
    if(strcmp(ptr->blc->hashAnterior,"0000") == 0){
        anterior = (*cabeca);
        temp = (*cabeca)->blc;
        (*cabeca)->blc = ptr->blc;
        ptr->blc = temp;
      }
      ptr = ptr->link;
  }
  ptr = *cabeca;
  for(int i = 0; i <= tamanhoLista; i++){
    ptr = *cabeca;
    struct b * temp = NULL;

    do{
      if(strcmp(ptr->blc->hashAnterior,anterior->blc->hashAtual) == 0){
        temp = ptr->blc;
        ptr->blc = anterior->link->blc;
        anterior->link->blc = temp;
        anterior = anterior->link;
        ptr = ptr->link;
      }
      if(i==6){
      printf("%s--%s\n",ptr->blc->hashAnterior,anterior->blc->hashAtual);
      }
      ptr = ptr->link;
    }while(ptr->link != NULL);
      
    
    }

}
