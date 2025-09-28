#include <stdio.h>
#include <time.h>
#include "hash.c"
#include "listaLigada.c"


void HoraData(char* Entrada){
    
    time_t tempo = time(NULL);
    struct tm *tempo_local = localtime(&tempo);
    sprintf(Entrada,"%s",asctime(tempo_local));

}

bloco* CriarBloco(char* estIn, char* hashAnt){
    
    struct b *y = (struct b*)malloc(sizeof(struct b));

    if (y == NULL){
        exit(1);
    }

    strcpy(y->estagio,estIn);
    HoraData(y->dataHoraC);
    y->hashAnterior = hashAnt;
    char var[(strlen(hashAnt))+(strlen(estIn))+(strlen(y->dataHoraC))+1];
    strcpy(var,hashAnt);
    strcat(var,estIn);
    strcat(var,y->dataHoraC);
    hash(var,sizeof(var),y->hashAtual);
    
    return y;
}

void entrada(struct node **cabeca,struct node **ultimoBlcCriado){

    if (*cabeca == NULL){
        char entradaNome[30];
        printf("\nDigite o primeiro estagio: ");
        scanf("%s",entradaNome);
        bloco *primeirobloco = CriarBloco(entradaNome,"0000");
        *cabeca = comecaLista(primeirobloco);
        *ultimoBlcCriado = *cabeca;
    }else{
        char entradaNome[30];
        printf("\nDigite o proximo estagio: ");
        scanf("%s",entradaNome);
        bloco *x = CriarBloco(entradaNome,(*ultimoBlcCriado)->blc->hashAtual);
        addLista(*ultimoBlcCriado,x);
        *ultimoBlcCriado = (*ultimoBlcCriado)->link;
    }

}




int main(void){

    struct node *cabeca = NULL;
    struct node *ultimoBlcCriado = NULL;
    
    entrada(&cabeca,&ultimoBlcCriado);
    entrada(&cabeca,&ultimoBlcCriado);
  

    // Passar por todos os blocos da lista.
    struct node *ponteiro = cabeca;
    for (int i = 0; i <= ultimoBlcCriado->vetor; i++){
        
        printf("%s\n",ponteiro->blc->estagio);
        ponteiro = ponteiro->link;
    }
    
    return 0;
}
