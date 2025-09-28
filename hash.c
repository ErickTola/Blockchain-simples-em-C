#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

const uint32_t H[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
const uint32_t K[] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

uint32_t rotacionar_direita(uint32_t n,unsigned int d){

    return (n >> d) | (n << (32 - d));
}

void hash(char *str,unsigned long long tamanho, char *strOut){
    
     if ((strlen(str) + 1) < tamanho){
        
        for (size_t i = strlen(str) - 1; i < tamanho-2; i++){
            strcat(str," ");
        }
        
    }
    uint32_t HashFinal[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    char HashFinalStr[65];

    // definição de tamanhos e acerto de tamanhos maiores e menores
    size_t tamanho_bit = strlen(str) * 8;
    size_t tamanho_total = strlen(str) + 1;
    size_t adiciona_zeros;

    if ((tamanho_total % 64) <= 56){
        adiciona_zeros = 56 - (tamanho_total % 64);
    } else {
        adiciona_zeros = 64 + 56 - (tamanho_total % 64);
    }

    uint8_t* msg_tamanho_corrigido = (uint8_t*) malloc (tamanho_total + adiciona_zeros + 8);

    if(!adiciona_zeros) {exit(1);}

    memcpy(msg_tamanho_corrigido,str,strlen(str));

    msg_tamanho_corrigido[strlen(str)] = 0x80;

    memset(msg_tamanho_corrigido + strlen(str) + 1, 0x00, adiciona_zeros);

    for(int i; i < 8; i++){
        msg_tamanho_corrigido[(tamanho_total + adiciona_zeros + 8) - 1 - i] = (tamanho_bit >> (8 * i)) & 0xff;
    }


    // começa processamento de blocos
    for (size_t i = 0; i < strlen(str); i += 64){

        char bloco[64];

        strncpy(bloco,&str[i],sizeof(bloco)-2);
        bloco[64] = '\0';
        

        if ((strlen(bloco) + 1) < sizeof(bloco)){
        
        for (size_t i = strlen(bloco) - 1; i < sizeof(bloco)-1 ; i++){
            strcat(bloco," ");
        }
        }

        uint32_t w[64];
        for (int j = 0; j < 64; j += 4) {
        w[j / 4] = (bloco[j]) << 24 | (bloco[j + 1]) << 16 | (bloco[j + 2]) << 8 | (bloco[j + 3]);
        }

        for (int j = 16; j < 64; j++){

            uint32_t s0 = rotacionar_direita(w[j-15], 7) ^ rotacionar_direita(w[j-15], 18) ^ (w[j-15] >> 3);
            uint32_t s1 = rotacionar_direita(w[j-2], 17) ^ rotacionar_direita(w[j-2], 19) ^ (w[j-2] >> 10);
            w[j] = (w[j-16] + s0 + w[j-7] + s1);
        }
    
        uint32_t a,b,c,d,e,f,g,h;

        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];

        for (int j = 0; j < 64; j++){

            uint32_t S1 = rotacionar_direita(e, 6) ^ rotacionar_direita(e, 11) ^ rotacionar_direita(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t numero_temp1 = (h + S1 + ch + K[j] + w[j]);
            uint32_t S0 = rotacionar_direita(a, 2) ^ rotacionar_direita(a, 13) ^ rotacionar_direita(a, 22);
            uint32_t maioria = (a & b) ^ (a & c) ^ (b & c);
            uint32_t numero_temp2 = (S0 + maioria);
            
            h = g;
            g = f;
            f = e;
            e = (d + numero_temp1);
            d = c;
            c = b;
            b = a;
            a = (numero_temp1 + numero_temp2);
        }

        HashFinal[0] = HashFinal[0] + a;
        HashFinal[1] = HashFinal[1] + b;
        HashFinal[2] = HashFinal[2] + c;
        HashFinal[3] = HashFinal[3] + d;
        HashFinal[4] = HashFinal[4] + e;
        HashFinal[5] = HashFinal[5] + f;
        HashFinal[6] = HashFinal[6] + g;
        HashFinal[7] = HashFinal[7] + h;
        
    }
    char temp1[17];
    sprintf(temp1, "%08x" PRId32, HashFinal[0]);
    strcpy(HashFinalStr,temp1);

    for(int i = 1; i < 8; i++){
        sprintf(temp1,"%08x" PRId32, HashFinal[i]);
        strcat(HashFinalStr, temp1);
    }
    strcat(HashFinalStr,"\0");
    strcpy(strOut,HashFinalStr);
}
