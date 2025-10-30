#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



void geraNome(char* alvo,int tamanho){
    

    if(tamanho < 1){
    	printf("\nTamanho inválido para string aleatór\n");
	exit(1);
    }
    srand(time(NULL));

    char charsPos[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    char* rndStr = (char*) malloc((tamanho+1)*sizeof(char));

    if (rndStr == NULL){
    	exit(1);
    }

    for (size_t i = 0; i < tamanho; i++){
     rndStr[i] = charsPos[rand() % strlen(charsPos)];
    }
    rndStr[tamanho] = '\0';
    strcpy(alvo,rndStr);
}

