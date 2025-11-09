#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>

int totalArq() {
 //Função que conta quantos arquivos existem no diretorio "/blocos/" e retorna o total.
  int totalArquivos = 0;

 //Abre o diretorio
  struct dirent *dp;
  DIR *dir = opendir("./blocos/");

  if (!dir) 
      return 1;

  while ((dp = readdir(dir)) != NULL)
  {
   //Passa por todos os arquivos e retira os diretórios "." e ".." que são comuns da estrutura de diretorios
    if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
      totalArquivos++;
    }
  }

  closedir(dir);

  return totalArquivos;
}

char **listaArq(int *num) {
 //abre o diretório
  struct dirent *dp;
  DIR *dir = opendir("./blocos/");
//Cria um array de ponteiros para usar como um array de strings
  char **lista = malloc(totalArq() * sizeof(char *));
  if(lista == NULL){
      printf("Não poi possivel alocar lista");
      exit(1);
    }

  int vetor = 0;

  if (!dir) 
      exit(1); 

  while ((dp = readdir(dir)) != NULL)
  {
   //Lê arquivo por aquivo no diretório e associa o nome do arquivo á um vetor da lista
    if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
      char *temp =
          calloc(strlen(dp->d_name) + 1,1);
      if(temp == NULL){
        printf("Não poi possivel alocar temp");
        exit(1);
      } 
      strcpy(temp, dp->d_name);
      lista[vetor] = temp;
      vetor = vetor + 1;
    }
  }

  //Fecha o diretório
  closedir(dir);
 //retorna a lista e o total de arquivos na pasta.
  *num = vetor;
  return lista;
}



