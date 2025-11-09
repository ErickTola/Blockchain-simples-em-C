#include "filemManip.c"
#include "hash.c"
#include "listaLigada.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void HoraData(char *Entrada) {
//Função basica que captura a hora/data do sistema e transforma em string
  time_t tempo = time(NULL);
  struct tm *tempo_local = localtime(&tempo);
  sprintf(Entrada, "%s", asctime(tempo_local));
  Entrada[strlen(Entrada) - 1] = '\0';
}

bloco *CriarBloco(char *estIn, char *hashAnt,char* codigo) {
//Função para gerar um bloco e retornar um ponteiro para ele.

//Aloca memoria para o bloco e verifica se a mesma foi sucedida.
  struct b *y = malloc(sizeof(struct b));

  if (y == NULL) {
    printf("Não poi possivel alocar y");
    exit(1);
  }
  //Insere as informações passadas na função para o bloco
  strcpy(y->codigo,codigo);
  strcpy(y->estagio, estIn);
  HoraData(y->dataHoraC);
  strcpy(y->hashAnterior, hashAnt);
  //Mescla todos os dados do bloco, calcula o hash e o insere no bloco.
  size_t needed = strlen(hashAnt) + strlen(estIn) + strlen(y->dataHoraC) + 1;
  char *var = malloc(needed);
  if (!var) { printf("Não foi possivel alocar var"); exit(1); }
  snprintf(var, needed, "%s%s%s", hashAnt, estIn, y->dataHoraC);

  hash(var, strlen(var), y->hashAtual);

  free(var);
  //Devolve um ponteiro do bloco.
  return y;
}



void salvarEmArquivo(struct node *no) {
//Função para salvar um bloco passado para a função em arquivo.
  struct node *ponteiro = no;
  //Gera uma variavel com o nome do diretório dos blocos junto com o hash do bloco
    FILE *fl;
    char *path = calloc(
        (strlen("./blocos/") + strlen(ponteiro->blc->hashAtual) + 1),1);
    
    if(path == NULL){
      printf("Não poi possivel alocar path");
      exit(1);
    }
    strcpy(path, "./blocos/");
    strcat(path, ponteiro->blc->hashAtual);

    //Abre um arquivo a partir da variavel criada
    fl = fopen(path, "w");
    
    if (fl == NULL) {
      printf("Não foi possivel abrir o arquivo.");
      exit(1);
    }
    //Insere as informações do bloco no arquivo para serem salvas.
    fprintf(fl, "codigo: %s\n", ponteiro->codigo);
    fprintf(fl, "estagio: %s\n", ponteiro->blc->estagio);
    fprintf(fl, "dataHoraC: %s\n", ponteiro->blc->dataHoraC);
    fprintf(fl, "hashAnterior: %s\n", ponteiro->blc->hashAnterior);
    fprintf(fl, "hashAtual: %s\n", ponteiro->blc->hashAtual); 

    fclose(fl);
    free(path);
  
}

int verificarChain(struct node **cabeca, struct node **ultimoBlcCriado) {
//Função para verificar todos os blocos presentes na cadeia. Retorna um número que indica o vetor de um bloco invalido ou um valor
//que representa que toda a cadeia é valida.

  //Gera variaveis para navegarmos pela cadeia
  struct node *ponteiro = (*cabeca);
  struct node *anterior = NULL;
  int validez = 0;

  //Loop utilizado para analizar bloco por bloco da cadeia até chegar no ultimo
  for (int i = 0; i <= (*ultimoBlcCriado)->vetor; i++) {

    //Gera uma variavel igual a da função "CriarBloco" a partir dos dados do bloco sendo analizado para recalcular
    //Seu hash e compara-lo com o hash presente no bloco.

    size_t needed = strlen(ponteiro->blc->hashAnterior) + strlen(ponteiro->blc->estagio) + strlen(ponteiro->blc->dataHoraC) + 1;
    char *var = malloc(needed);
    if (!var) { printf("Não foi possivel alocar var"); exit(1); }
    snprintf(var, needed, "%s%s%s", ponteiro->blc->hashAnterior, ponteiro->blc->estagio, ponteiro->blc->dataHoraC);
    char TempHash[76];

    hash(var, strlen(var), TempHash);
    free(var);


    if(anterior != NULL){
      //Verifica se o hash calculado bate com o do bloco e se o hash anterior do bloco bate com o bloco anterior.
        if ((strcmp(ponteiro->blc->hashAtual, TempHash) == 0) && (strcmp(anterior->blc->hashAtual, ponteiro->blc->hashAnterior) == 0) ) {
          //Variavel para determinar a validade de uma cadeia. -1 significa que a cadeia é valida, qualquer valor acima disso representa
          //em qual vetor da cadeia os blocos começam a ser invalidos.
          //Ex: Validez = 5 significa que todos os blocos a partir do cinco são invalidos e devem ser desconsiderados.
          validez = -1;
        
      } else {
          validez = i;
          return validez;
      }
    }else{
      //Caso não haja anterior, verifica se o hash anterior bate com o valor de um bloco genesis, além de comparar o hash recalculado. 
      if ((strcmp(ponteiro->blc->hashAtual, TempHash) == 0) && (strcmp("0000", ponteiro->blc->hashAnterior) == 0) ) {
          validez = -1;
        
      } else {
          validez = i;
          return validez;
      }
    }
    
    //Define o bloco anterior como o bloco que acabou de ser analizado e passa o nosso cursor para o proximo bloco da lista.
    anterior = ponteiro;
    ponteiro = ponteiro->link;
   
    }
  return validez;
}

bloco *abrirBloco(char **lista) {
  //Tem como entrada uma lista de informações e as usa para gerar um bloco na cadeia. retorna um ponteiro para o bloco aberto.
  struct b *y = malloc(sizeof(struct b));

  if (y == NULL) {
    printf("Não poi possivel alocar y");
    exit(1);
  }
  //Mesmo processo da criação do bloco, porém a partir das informações da lista passada.
  strcpy(y->codigo, lista[0]);
  strcpy(y->estagio, lista[1]);
  strcpy(y->dataHoraC, lista[2]);
  strcpy(y->hashAnterior, lista[3]);
  strcpy(y->hashAtual, lista[4]);
  for(int i = 0;i<5;i++){
    free(lista[i]);
  }
  
  
  return y;
}

struct node * abrirArquivo(char* cod,struct node** anterior){
  //Função que abre todos os arquivos da pasta "/blocos/" e todos que tiverem o código infomado são colocados em uma cadeia.
  //Caso já exista uma cadeia na memoria do programa, a função a remove da memoria para poder abrir uma nova cadeia.
  if (anterior != NULL){
    limpaLista(*anterior);
    *anterior = NULL;
  }

  //Chama uma função que lista todos os arquivos da pasta "/blocos/" e conta sua totalidade.
    int totalArq;
    char **lista = listaArq(&totalArq);
    struct node *ultimoBlcAberto = NULL;
    struct node *cabeca = NULL;

  //Um loop que passa por cada arquivo presente na pasta e o abre como bloco
    for(int i =0;i < totalArq;i++){
      
      //Gera uma variavel temporaria que contem o nome do diretorio junto ao nome do arquivo, para poder abri-lo
      char *path= malloc((strlen(lista[i])+strlen("./blocos/")+1));
      if(path ==NULL){
        printf("Não foi possivel alocar path!");
        exit(1);
      }
      strcpy(path,"./blocos/");
      strcat(path,lista[i]);
      FILE *fl;
      char buffer[256];
      //Abre o arquivo e gera um cursor para poder navega-lo
      fl = fopen(path,"r");
     
      if (fl==NULL){
        printf("%s\n",path);
        printf("\nAquivo não foi encontrado.");
        exit(1);
      }
      free(path);
      char *ptr;
      char *leitura[6];
      int linha = 0;
      //Passa por todas as linhas do arquivo e as coloca em uma lista de informações, para serem processadas pela função abrirBloco
      while(fgets(buffer,256,fl) != NULL){
        if (linha >= 5){
          printf("teste");
          break;
        }
        ptr = (strchr(buffer,' '));
        leitura[linha] = malloc(strlen(ptr)+1);
        if(leitura[linha] == NULL){
          printf("Não poi possivel alocar leitura[linha]");
          exit(1);
        }
        strcpy(leitura[linha],ptr+1);
        

        //Coloca uma terminação nula na string e remove o caractere de quebra de linha
        leitura[linha][(strchr(leitura[linha],'\n') - leitura[linha])] = '\0'; 
        
        linha = linha + 1;
      }
      
      //abre o bloco a partir da lista de informações obtida do arquivo
      struct b *blc = abrirBloco(leitura);
      if(strcmp(blc->codigo,cod) == 0){
        //Associa o bloco na cadeia caso seu código coincida com o código informado na entrada da função
        ultimoBlcAberto = abrirLista(blc,ultimoBlcAberto,&cabeca);
        
      }
      fclose(fl);
    }
    //Caso nenhum bloco seja associado á lista, devolve nulo para informar que a lista não foi encontrada.
    if(cabeca == NULL){printf("Codigo Não encontrado."); return NULL;}

   if(cabeca->link != NULL){
     //Quando o arquivo é aberto ele vem fora de ordem, então chamamos uma função para coloca-los na ordem correta.
      arrumalista(&cabeca);
    }
  //Retorna a cabeça da cadeia para podermos navega-la
    return cabeca;
}

char* passarLista(struct node* cabeca){
//Função utilizada para capturar todos os dados de uma cadeia e retorna-os num bloco em formato de string, para que possa ser
//processado posteriormente.
  
  if(cabeca != NULL){
    struct node *ultimoBlcCriado = NULL;
    bloco b;

    //Garante que a lista está em ondem antes de envia-la.
    arrumalista(&cabeca);
    //Cria uma variavel com o tamanho individual de um nó da cadeia.
    int tamanhoSingu = sizeof(b.codigo) + sizeof(b.estagio) + sizeof(b.hashAtual) + sizeof(b.dataHoraC) + sizeof(b.hashAtual) * 9;
    int tamanhoTotal;
    struct node *ponteiro = cabeca;

    if(cabeca->link != NULL){
      //Garante que a lista está em ondem antes de envia-la.
      arrumalista(&cabeca);
    }

    int conta = 1;

    //Navega por todos os nós da lista, descobre o ultimo nó e o tamanho total da lista
    while (ponteiro != NULL){
        if (ponteiro->link == NULL){
          ultimoBlcCriado = ponteiro;
          tamanhoTotal = tamanhoSingu * (conta);
          
          break;
        }
        conta++;
        ponteiro = ponteiro->link;
        
    };
    //Aloca uma string com espaço o suficiente para guardar todas as informações da cadeia.
    char *stringApassar =malloc(tamanhoTotal);

    if(stringApassar == NULL){
      printf("Não poi possivel alocar stringApassar");
      exit(1);
    }

    ponteiro = cabeca;

    //Pega a informação da validez da cadeia para passar na string

    int validez = verificarChain(&cabeca,&ultimoBlcCriado);
    while (ponteiro != NULL){
      //variaveis temporarias com valores que serão utilizados para formatar a string.
        char ch[] = ".\0";
        char ch1[] = ";\0";
        char temp[] = "\0\0\0\0\0";
    
        if(ponteiro->vetor == 0){
          //Verifica se é o primeiro indice para colocar duas informações adicionais no começo da string,
          //sendo elas o código da cadeia e sua validez
          strcpy(stringApassar,ponteiro->codigo);
          strcat(stringApassar,ch);
          sprintf(temp,"%d",validez);
          strcat(stringApassar,temp);
          strcat(stringApassar,ch);
          sprintf(temp,"%d",ponteiro->vetor);
          strcat(stringApassar,temp);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->estagio);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->dataHoraC);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->hashAnterior);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->hashAtual);
          strcat(stringApassar,ch1);
        }else{
          //Coloca o resto das informações da cadeia na string.
          sprintf(temp,"%d",ponteiro->vetor);
          strcat(stringApassar,temp);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->estagio);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->dataHoraC);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->hashAnterior);
          strcat(stringApassar,ch);
          strcat(stringApassar,ponteiro->blc->hashAtual);
          strcat(stringApassar,ch1);
        }
        ponteiro = ponteiro->link;
        
    }
    
    return stringApassar;
}else{
  //Caso a cadeia seja nula, devolve uma string informando que a cadeia com aquele código não existe.
  return "Codigo inexistente!";
}
  
}

struct node* entrada(struct node *cabeca,char* ent) {
//Função que recebe a entrada do usuário e o primeiro nó da cadeia para criar e associar um bloco novo e retorna o primeiro nó da cadeia.
  if (cabeca == NULL) {
    //Caso a cadeia não exista, gera um bloco gênesis e um código aleatório.
    char temp[20];
    geraNome(temp,15);

    bloco *primeirobloco = CriarBloco(ent, "0000",temp);
    cabeca = comecaLista(primeirobloco);
    salvarEmArquivo(cabeca);

    return cabeca;
  } else {
//Gera o bloco a partir da entrada do usuário e descobre o ultimo nó da cadeia.
    struct node* ponteiro = cabeca;
    struct node* ultimoBlcCriado;
    while (ponteiro != NULL){
            if (ponteiro->link == NULL){
              ultimoBlcCriado = ponteiro;
              break;
            }
            ponteiro = ponteiro->link;
    }

    //Associa o bloco na cadeia e o salva em arquivo.
    bloco *x = CriarBloco(ent, ultimoBlcCriado->blc->hashAtual,ultimoBlcCriado->codigo);
    addLista(ultimoBlcCriado, x,cabeca->codigo);
    ultimoBlcCriado = ultimoBlcCriado->link;
    strcpy(ultimoBlcCriado->codigo,cabeca->codigo);
    salvarEmArquivo(ultimoBlcCriado);
    return cabeca;
  }
}
