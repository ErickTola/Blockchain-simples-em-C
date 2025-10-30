#include "filemManip.c"
#include "hash.c"
#include "listaLigada.c"
#include <stdio.h>
#include <time.h>
#include <windows.h>

void HoraData(char *Entrada) {

  time_t tempo = time(NULL);
  struct tm *tempo_local = localtime(&tempo);
  sprintf(Entrada, "%s", asctime(tempo_local));
  Entrada[strlen(Entrada) - 1] = '\0';
}

bloco *CriarBloco(char *estIn, char *hashAnt) {

  struct b *y = (struct b *)malloc(sizeof(struct b));

  if (y == NULL) {
    exit(1);
  }

  strcpy(y->estagio, estIn);
  HoraData(y->dataHoraC);
  y->hashAnterior = hashAnt;
  char var[(strlen(hashAnt)) + (strlen(estIn)) + (strlen(y->dataHoraC)) + 1];
  strcpy(var, hashAnt);
  strcat(var, estIn);
  strcat(var, y->dataHoraC);
  hash(var, sizeof(var), y->hashAtual);

  return y;
}

void entrada(struct node **cabeca, struct node **ultimoBlcCriado) {

  if (*cabeca == NULL) {
    char entradaNome[76];
    printf("\nDigite o primeiro estagio: ");
    scanf("%s", entradaNome);
    bloco *primeirobloco = CriarBloco(entradaNome, "0000");
    *cabeca = comecaLista(primeirobloco);
    *ultimoBlcCriado = *cabeca;
  } else {
    char entradaNome[76];
    printf("\nDigite o proximo estagio: ");
    scanf("%s", entradaNome);
    bloco *x = CriarBloco(entradaNome, (*ultimoBlcCriado)->blc->hashAtual);
    addLista(*ultimoBlcCriado, x);
    *ultimoBlcCriado = (*ultimoBlcCriado)->link;
  }
}

void verificarChain(struct node **cabeca, struct node **ultimoBlcCriado) {
  struct node *pontTemp = (*cabeca);

  for (int i = 0; i <= (*ultimoBlcCriado)->vetor; i++) {
    char var[(strlen(pontTemp->blc->hashAnterior)) +
             (strlen(pontTemp->blc->estagio)) +
             (strlen(pontTemp->blc->dataHoraC)) + 1];
    char TempHash[76];

    strcpy(var, "");

    strcpy(var, pontTemp->blc->hashAnterior);
    strcat(var, pontTemp->blc->estagio);
    strcat(var, pontTemp->blc->dataHoraC);

    hash(var, sizeof(var), TempHash);

    if (!(strncmp(pontTemp->blc->hashAtual, TempHash, 76))) {
      printf("\nBloco %d Valido!", i);
    } else {
      printf("\nBlocos %d ao %d sao invalidos!", i, (*ultimoBlcCriado)->vetor);
      exit(0);
    }

    pontTemp = pontTemp->link;
  }
}

void salvarEmArquivo(struct node **cabeca, struct node **ultimoBlcCriado) {

  struct node *ponteiro = *cabeca;
  for (int i = 0; i <= (*ultimoBlcCriado)->vetor; i++) {
    FILE *fl;
    char *path = (char *)malloc(
        (strlen("./blocos/") + strlen(ponteiro->blc->hashAtual)) *
        sizeof(char));
    strcpy(path, "./blocos/");
    strcat(path, ponteiro->blc->hashAtual);
    fl = fopen(path, "w");
    if (fl == NULL) {
      printf("Não foi possivel abrir o arquivo.");
      exit(1);
    }

    fprintf(fl, "codigo: %s\n", ponteiro->codigo);
    fprintf(fl, "estagio: %s\n", ponteiro->blc->estagio);
    fprintf(fl, "dataHoraC: %s\n", ponteiro->blc->dataHoraC);
    fprintf(fl, "hashAnterior: %s\n", ponteiro->blc->hashAnterior);
    fprintf(fl, "hashAtual: %s\n", ponteiro->blc->hashAtual);

    ponteiro = ponteiro->link;
    fclose(fl);
    free(path);
  }
  
}

bloco *abrirBloco(char **lista) {

  struct b *y = (struct b *)malloc(sizeof(struct b));

  if (y == NULL) {
    exit(1);
  }
  strcpy(y->codigo, lista[0]);
  strcpy(y->estagio, lista[1]);
  strcpy(y->dataHoraC, lista[2]);
  y->hashAnterior = (char *)malloc(strlen(lista[3]) * sizeof(char));
  strcpy(y->hashAnterior, lista[3]);
  strcpy(y->hashAtual, lista[4]);

  return y;
}

struct node * abrirArquivo(char* codigo){
  int totalArq;
  char **lista = listaArq(&totalArq);
  // / sizeof(lista[0])
  struct node *ultimoBlcAberto = NULL;
  struct node *cabeca = NULL;
  for(int i =2;i < totalArq;i++){
    
    char path[strlen(lista[i])+strlen(".\\blocos\\")+1];
    strcpy(path,".\\blocos\\");
    strcat(path,lista[i]);
    FILE *fl;
    char buffer[256];
    fl = fopen(path,"r");
    if (fl==NULL){
      printf("%s",path);
      printf("\nAquivo não foi encontrado.");
      exit(1);
    }

    char *ptr;
    char *leitura[6];
    int linha = 0;

    while(fgets(buffer,256,fl) != NULL){
      ptr = (strchr(buffer,' '));
      leitura[linha] = (char*)malloc(strlen(ptr)*sizeof(char)+1);
      strcpy(leitura[linha],ptr+1);
      leitura[linha][strlen(leitura[linha])-1] = '\0'; 
      linha = linha + 1;
    }
  
    struct b *blc = abrirBloco(leitura);
    if(strcmp(blc->codigo,codigo) == 0){
      ultimoBlcAberto = abrirLista(codigo,blc,ultimoBlcAberto,&cabeca);
    }
    fclose(fl);
  }
  if(cabeca == NULL){printf("Codigo Não encontrado."); exit(0);}

  return cabeca;
}

int main(void) {
   /*
 
  
  struct node *cabeca = NULL;
  struct node *ultimoBlcCriado = NULL;

  for (int i = 0; i < 10; i++) {
    entrada(&cabeca, &ultimoBlcCriado);
  }
  salvarEmArquivo(&cabeca,&ultimoBlcCriado);

*/
  

  struct node *teste = abrirArquivo("sNe4LeuqajMUj31");
  
  arrumalista(&teste);



  
  
  // Passar por todos os blocos da lista.
  printf("%s\n",teste->blc->estagio);
  printf("%s\n",teste->link->blc->estagio);
  printf("%s\n",teste->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->link->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->link->link->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->link->link->link->link->link->blc->estagio);
  printf("%s\n",teste->link->link->link->link->link->link->link->link->link->blc->estagio);


  return 0;
  

  

  
  
  
}
