#include <stdio.h>
//Struct que define o formato de um bloco.
typedef struct b {
  char codigo[20];
  char estagio[200];
  char hashAnterior[76];
  char dataHoraC[26];
  char hashAtual[76];

} bloco;
