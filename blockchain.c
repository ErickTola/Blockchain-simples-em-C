#include <stdio.h>
#include <time.h>
#include "hash.c"
#include "listaLigada.c"


void HoraData(char* Entrada){
    
    time_t tempo = time(NULL);
    struct tm *tempo_local = localtime(&tempo);
    sprintf(Entrada,"%s",asctime(tempo_local));
    Entrada[strlen(Entrada) - 1 ] = '\0';

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
        char entradaNome[76];
        printf("\nDigite o primeiro estagio: ");
        scanf("%s",entradaNome);
        bloco *primeirobloco = CriarBloco(entradaNome,"0000");
        *cabeca = comecaLista(primeirobloco);
        *ultimoBlcCriado = *cabeca;
    }else{
        char entradaNome[76];
        printf("\nDigite o proximo estagio: ");
        scanf("%s",entradaNome);
        bloco *x = CriarBloco(entradaNome,(*ultimoBlcCriado)->blc->hashAtual);
        addLista(*ultimoBlcCriado,x);
        *ultimoBlcCriado = (*ultimoBlcCriado)->link;
    }

}

void verificarChain(struct node **cabeca,struct node **ultimoBlcCriado){
    struct node *pontTemp = (*cabeca);

    for(int i = 0; i <=(*ultimoBlcCriado)->vetor;i++){
        char var[(strlen(pontTemp->blc->hashAnterior))+(strlen(pontTemp->blc->estagio))+(strlen(pontTemp->blc->dataHoraC))+1];
        char TempHash[76];

        strcpy(var, "");

        strcpy(var,pontTemp->blc->hashAnterior);
        strcat(var,pontTemp->blc->estagio);
        strcat(var,pontTemp->blc->dataHoraC);

        hash(var,sizeof(var),TempHash);

        if(!(strncmp(pontTemp->blc->hashAtual,TempHash,76))){
            printf("\nBloco %d Valido!",i);
        }else{
            printf("\nBloco %d Invalido!",i);
        }

        pontTemp = pontTemp->link;
    }

}


int main(void){

    struct node *cabeca = NULL;
    struct node *ultimoBlcCriado = NULL;
    
    
    for (int i = 0; i < 5; i++){
        entrada(&cabeca,&ultimoBlcCriado);
    }      
    
  

    // Passar por todos os blocos da lista.
    struct node *ponteiro = cabeca;
    for (int i = 0; i <= ultimoBlcCriado->vetor; i++){
        
        printf("%s \n",ponteiro->blc->estagio);
        ponteiro = ponteiro->link;
    }
    

   verificarChain(&cabeca,&ultimoBlcCriado);
    return 0;
}
