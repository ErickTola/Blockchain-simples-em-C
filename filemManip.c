#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int totalArq(char *dir) {
  WIN32_FIND_DATA findData;
  HANDLE hFind = FindFirstFile(dir, &findData);
  int totalArquivos = 0;

  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      totalArquivos = totalArquivos + 1;
    } while (FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);
  } else {
    printf("Ocorreu um erro na verificação de arquivos..\n");
  }

  return totalArquivos;
}

char **listaArq(int *num) {
  WIN32_FIND_DATA findData;
  HANDLE hFind = FindFirstFile(".\\blocos\\*", &findData);

  char **lista = (char **)malloc(totalArq(".\\blocos\\*") * sizeof(char *));
  int vetor = 0;
  if (hFind != INVALID_HANDLE_VALUE) {
    do {

      char *temp =
          (char *)malloc(strlen(findData.cFileName) * sizeof(char) + 1);
      strcpy(temp, findData.cFileName);
      lista[vetor] = temp;
      vetor = vetor + 1;

    } while (FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);
  } else {
    printf("Ocorreu um erro na verificação de arquivos..\n");
  }
  *num = vetor;
  return lista;
}


