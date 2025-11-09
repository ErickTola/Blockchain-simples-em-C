#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

//inicia um grupo de constantes usadas no SHA-256, que são os primeiros 32 bits dos decimais das raizes cúbicas dos primeiros 64 números primos
static const uint32_t K[64] = {0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};

//Define uma função de rotação de bits
static inline uint32_t rotacionar_direita(uint32_t n, unsigned int d) {
    //Rotaciona para direita os bits inceridos (n) pelo número de vezes (d) 
    return (n >> d) | (n << (32 - d));
}


void hash(const uint8_t *mensagem, size_t tamanho, char *saidastr) {
    // Valores inicias do HASH, usamos uma constante do SHA-256 que são os primerios 32 bits dos decimais das raizes dos primeiros 8 números primos
    uint32_t H[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};

    //8ULL garante que o resultado seja em 64bits, já que o C assume que apenas "8" seja um número em 32bits,
    //Mesmo quando multipilcado por um número de 64bits
    uint64_t tamanhoBit = (uint64_t)tamanho * 8ULL ; 
    
    size_t tamanhoCerto = tamanho + 1 + 8; 

    //Garante que o o tamanho do bloco seja 512 bits, ou 64 bytes
    while (tamanhoCerto % 64 != 0) tamanhoCerto++;

    //Calloc aloca a memória utilizada o bloco e garante que caso o conteúdo não atinja 512 bits
    //o resto da informação será preenchida com zeros.
    uint8_t *bloco = calloc(1, tamanhoCerto); 
    if (bloco == NULL) {
        printf("Falha ao tentar alocar bloco do hash\n");
        exit(1);
    }
    //Copia as informações de entrada para a memoria alocada.
    memcpy(bloco, mensagem, tamanho);
    //Adiciona um separador entre as informações do bloco e os zeros adicionados,
    //Para não contaminarmos as informações originais caso a informações não seja exatamente 512 bits
    bloco[tamanho] = 0x80; 

    
    for (int i = 0; i < 8; ++i) {
        //Além do separador, devemos armazenar o tamanho original da mensagem no bloco
        //Para podermos separar dos zeros
        bloco[tamanhoCerto - 1 - i] = (uint8_t)((tamanhoBit >> (8 * i)) & 0xff);
    }

    for (size_t i = 0; i < tamanhoCerto; i += 64) {
        uint32_t w[64];
     
        for (int t = 0; t < 16; ++t) {
            //Quebra o bloco em 16 linhas de 32 bits
            size_t j = i + t*4;
            w[t] = ((uint32_t)bloco[j] << 24) | ((uint32_t)bloco[j+1] << 16) | ((uint32_t)bloco[j+2] << 8)  | ((uint32_t)bloco[j+3]);
        }
     
        for (int t = 16; t < 64; ++t) {
            //Cria mais 48 linhas de 32 bits a partir das 16 linhas iniciais
            //Embaralhando, cortando e realizando "XOR"s para modificar a informação.
            uint32_t s0 = rotacionar_direita(w[t-15], 7) ^ rotacionar_direita(w[t-15], 18) ^ (w[t-15] >> 3);
            uint32_t s1 = rotacionar_direita(w[t-2], 17) ^ rotacionar_direita(w[t-2], 19) ^ (w[t-2] >> 10);
            w[t] = w[t-16] + s0 + w[t-7] + s1;
        }
        //Inicializa nossas informações finais e atribui os valores da nossa constante H á ela.
        uint32_t a,b,c,d,e,f,g,h;

        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];
        
        for (int t = 0; t < 64; ++t) {
            //Começa o calculo principal do Hash até acabar as informações do bloco.
            uint32_t S1 = rotacionar_direita(e,6) ^ rotacionar_direita(e,11) ^ rotacionar_direita(e,25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + K[t] + w[t];
            uint32_t S0 = rotacionar_direita(a,2) ^ rotacionar_direita(a,13) ^ rotacionar_direita(a,22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        //Soma as informações processadas com as constantes originais
        H[0] += a;
        H[1] += b;
        H[2] += c;
        H[3] += d;
        H[4] += e;
        H[5] += f;
        H[6] += g;
        H[7] += h;
    }


    char *p = saidastr;
    for (int i = 0; i < 8; ++i) {
        //Converte as informaçõe de Bytes para caractére e as adicionas na string de saida
        sprintf(p, "%08x", H[i]);
        p += 8;
    }
    //Valida a string á terminando com um caractére núlo
    *p = '\0';

    //Libera a memória do bloco agora que já não está em uso.
    free(bloco);
}
