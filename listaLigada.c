#include "bloco.c"
#include "rdnStr.c"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int ArqExiste(const char *entrada) {
  int totalArq;

  //Recebe uma lista com o nome de todos os arquivos presentes em "./blocos"
  char **lista = listaArq(&totalArq);

  for(int i =2;i < totalArq;i++){
    //Abre arquivo por arquivo presente na pasta "./blocos"
    //Verifica se existe algum arquivo que contenha o código de entrada
    char *path= malloc((strlen(lista[i])+strlen("./blocos/")+1));
      if(path ==NULL){
        printf("Não foi possivel alocar path!");
        exit(1);
      }
    strcpy(path,"./blocos/");
    strcat(path,lista[i]);
    FILE *fl;
    char buffer[256];
    char codigo[20];
    fl = fopen(path,"r");
    if (fl==NULL){
      printf("%s\n",path);
      printf("\nAquivo não foi encontrado.");
      exit(1);
    }
    fgets(buffer,256,fl);
    strncpy(codigo,buffer+8,15);
    codigo[16] = '\0';
    free(path);
    if(strcmp(codigo,entrada) == 0)
    {
      entrada = codigo;
      return 1;
    }
  
}
  return 0;
}

struct node {
  //Struct que define o formato de um nó.
  char codigo[20];
  int vetor;
  struct b *blc;
  struct node *link;
};

struct node *comecaLista(struct b *x) {
  //Função utilizada quando ainda não há uma cadeia

  //Cria e aloca memoria para o primeiro nó
  struct node *cabeca = NULL;
  cabeca = malloc(sizeof(struct node));

  if (cabeca == NULL) {
    printf("Não foi possivel alocar cabeca");
    exit(1);
  }
 
  // Insere todas as informações no nó, como código, bloco que ele aponta para, vetor e o proximo nó
  strncpy(cabeca->codigo, x->codigo, sizeof(cabeca->codigo)-1);
  cabeca->codigo[sizeof(cabeca->codigo)-1] = '\0';
  cabeca->vetor = 0;
  cabeca->blc = x;
  cabeca->link = NULL;

  //Retorna um ponteiro para o primeiro nó
  return cabeca;
}

void addLista(struct node *anterior, bloco *x,char* codigo) {
  //Função utilizada quando há ao menos 1 nó na cadeia

  //Cria e aloca um nó
  struct node *no = malloc(sizeof(struct node));

  if (no == NULL) {
    printf("Não foi possivel alocar no");
    exit(1);
  }
  
  // Insere todas as informações no nó, como código, bloco que ele aponta para, vetor e o proximo nó
  // Coloca um ponteiro para sí mesmo no campo de "Proximo nó" do nó anterior
  strncpy(no->codigo, x->codigo, sizeof(no->codigo)-1);
  no->codigo[sizeof(no->codigo)-1] = '\0';
  no->vetor = anterior->vetor + 1;
  no->blc = x;
  no->link = NULL;
  anterior->link = no;
}

struct node *abrirLista(bloco *bloco,struct node *ultimoBlcAberto,struct node **cabeca) {
  //Função utilizada gerar uma cadeia na memória do programa enquanto estiver abrindo arquivos
  struct node *no = NULL;
  no = malloc(sizeof(struct node));
  

  if (no == NULL) {
    printf("Não foi possivel alocar no\n");
    exit(1);
  }
  //Funciona similar ás funções "comecaLista" e "addLista"
  if (ultimoBlcAberto == NULL){
    strncpy(no->codigo, bloco->codigo, sizeof(no->codigo)-1);
    no->codigo[sizeof(no->codigo)-1] = '\0';
    no->vetor = 0;
    no->blc = bloco;
    no->link = NULL;
    *cabeca = no;
  }else{
    strncpy(no->codigo, bloco->codigo, sizeof(no->codigo)-1);
    no->codigo[sizeof(no->codigo)-1] = '\0';
    no->vetor = ultimoBlcAberto->vetor + 1;
    no->blc = bloco;
    no->link = NULL;
    ultimoBlcAberto->link = no;
  }
  return no;
}

void arrumalista(struct node **cabeca){
  //Função para organizar a lista a partir do hash de cada bloco

  int tamanhoLista = 0;
  //Cria um "Cursor" para podermos navegar pela cadeia.
  struct node *ptr = *cabeca;
  struct node *anterior;

  if (ptr->link ==NULL){
    //Verifica se a cadeia não contém apenas um nó, e caso tenha, sai da função
    return;
  }

  while(ptr->link != NULL){
    //Conta quantos nós há na cadeia
    ptr = ptr->link;
    tamanhoLista++;
  }
  //Redefine o cursor para o começo da cadeia
  ptr = *cabeca;
  
  while(ptr != NULL){
    //Procura em todos os nós qual bloco contem o hash genesis, no caso "0000"
    //Depois de encontrar, troca o bloco do nó encontrado pelo bloco do inicio da lista e vice-versa
  
    struct b * temp;
    if(strcmp(ptr->blc->hashAnterior,"0000") == 0){
        anterior = (*cabeca);
        temp = (*cabeca)->blc;
        (*cabeca)->blc = ptr->blc;
        ptr->blc = temp;
        break;
      }
      ptr = ptr->link;
  }
   //Redefine o cursor para o começo da cadeia
  ptr = *cabeca;
  for(int i = 0; i <= tamanhoLista; i++){
    //Redefine o cursor para o começo da cadeia para cada iteração
    ptr = *cabeca;
    struct b * temp = NULL;
    

    do{
      //Procura bloco por bloco em cada nó para ver qual contem o hash do bloco genesis como o seu hash Anterior
      /*
        Ex:
        bloco1:                 bloco2:
        Hash Ant: "0000"    ↗➡➡➡Hash Ant: "1234"
        hash Atual: "1234"↗     Hash atual "5678"
      */
      //Depois de encontrar, troca o bloco do nó encontrado pelo bloco que está sendo analizado e vice-versa
      if (ptr==NULL){
        break;
      }
      if(strcmp(ptr->blc->hashAnterior,anterior->blc->hashAtual) == 0){
        temp = ptr->blc;
        ptr->blc = anterior->link->blc;
        anterior->link->blc = temp;
        anterior = anterior->link;
        break;
      }
      
      ptr = ptr->link;
    }
    while(true);
    
    }

}


void limpaLista(struct node *cabeca){
  //Libera toda a memória da lista
  if(cabeca == NULL){ 
    //Caso a lista já não tenha informações, sai da função
    return;
  }


  struct node *ponteiro = cabeca;
  struct node *temp;
        while (ponteiro != NULL){
        //Libera a memória do bloco
        free(ponteiro->blc);
        //Define o ponteiro como nulo para não ocorrer de tentarmos acessar memoria liberada
        ponteiro->blc = NULL; 
        //Define uma variavel temporária para não perdermos aonde estamos na cadeia
        temp = ponteiro;
        //Passa o cursor para o próximo nó
        ponteiro = ponteiro->link;
        //Libera a memória do nó anterior.
        free(temp);
        
      }
  //Define a cabeça como nula para não ocorrer de tentarmos acessar memoria liberada
  cabeca = NULL;
}