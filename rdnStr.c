#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



void geraNome(char* alvo,int tamanho){
    // Gera uma string aleatória com o tamanho definido.

    if(tamanho < 1){
    	printf("\nTamanho inválido para string aleatória\n");
	exit(1);
    }
	//Utiliza a data/hora do sistema para gerar uma "Semente" da função rand, garantindo que o mesmo código não se repita toda vez
    srand(time(NULL));

	//Disponibiliza os caracteres possiveis para a geração da string e aloca memória para a string.
    char charsPos[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    char* rndStr = malloc((tamanho+1));

    if (rndStr == NULL){
        printf("Não poi possivel alocar rndSTR");
    	exit(1);
    }

	//Gera caractére por caractére aleatório na string e o associa a variavel final.
    for (int i = 0; i < tamanho; i++){
     rndStr[i] = charsPos[rand() % strlen(charsPos)];
    }
	//Define o ultimo caractere da string como nulo para valida-la
    rndStr[tamanho] = '\0';
	//Copia a string aleatória para a variavel alvo.
    strcpy(alvo,rndStr);
}

