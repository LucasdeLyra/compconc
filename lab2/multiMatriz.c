#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
//#include "leMatrizBinario.c"

typedef struct vetor{
    int linhas, colunas;
    float *vetores;
} vetor;

int escreveMatrizBinario(vetor matrizsaida, char *saida){
    long long int tam; //qtde de elementos na matriz
    FILE * descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida
    tam = matrizsaida.linhas * matrizsaida.colunas;
    //escreve a matriz no arquivo
    //abre o arquivo para escrita binaria
    descritorArquivo = fopen(saida, "wb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
    //escreve numero de linhas e de colunas
    ret = fwrite(&matrizsaida.linhas, sizeof(int), 1, descritorArquivo);
    ret = fwrite(&matrizsaida.colunas, sizeof(int), 1, descritorArquivo);
    //escreve os elementos da matriz
    ret = fwrite(matrizsaida.vetores, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
        return 4;
    }
    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return 0;
}

int leMatrizBinario(char* arquivo, vetor* entrada) {
    int linhas, colunas; //dimensoes da matriz
    long long int tam; //qtde de elementos na matriz
    FILE * descritorArquivo; //descritor do arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada

    //abre o arquivo para leitura binaria
    descritorArquivo = fopen(arquivo, "rb");
    if(!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    //le as dimensoes da matriz
    ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    tam = linhas * colunas; //calcula a qtde de elementos da matriz
    //aloca memoria para a matriz
    entrada->linhas = linhas;
    entrada->colunas = colunas;
    entrada->vetores = (float*)malloc(entrada->linhas*entrada->colunas*sizeof(float));
    if (entrada->vetores == NULL){
        printf("--ERRO: malloc()\n"); exit(-1);
    }
    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(entrada->vetores, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }

    //imprime a matriz na saida padrao
    /*for(int i=0; i<linhas; i++) { 
        for(int j=0; j<colunas; j++)
            fprintf(stdout, "%.6f ", entrada->vetores[i*colunas+j]);
        fprintf(stdout, "\n");
    }*/

    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return 0;
}


int main(int argc, char*argv[]){
    vetor matriz1, matriz2, matrizsaida;
    int m1index, m2index, msaidaindex;
    double inicio, fim, delta;


    if(argc < 4) {
        fprintf(stderr, "Digite: %s <arquivo entrada1> <arquivo entrada2> <arquivo saida>\n", argv[0]);
        return 1;
    }

    //inicialização
    GET_TIME(inicio);
    leMatrizBinario(argv[1], &matriz1);
    leMatrizBinario(argv[2], &matriz2);

    if (matriz1.colunas != matriz2.linhas){
        fprintf(stderr, "As matrizes de entrada nao tem dimensões compativeis\n");
        return 5;
    }

    matrizsaida.linhas = matriz1.linhas;
    matrizsaida.colunas = matriz2.colunas;
    matrizsaida.vetores = (float*)malloc(matriz1.linhas*matriz2.colunas*sizeof(float));
    
    if (matrizsaida.vetores == NULL){
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    for (int i = 0; i < matriz1.linhas*matriz2.colunas; i++){
        matrizsaida.vetores[i] = 0;
    }

    GET_TIME(fim);
    delta = fim-inicio;
    printf("Tempo de inicialização %lf\n", delta);

    //processamento
    GET_TIME(inicio);
    for (int i=0; i<matriz1.linhas; i++){
        for (int j=0; j<matriz2.colunas; j++){
            msaidaindex = i*matriz1.colunas+j;
            for (int k=0; k<matriz1.colunas; k++){
                m1index = i*matriz1.colunas + k;
                m2index = k*matriz2.linhas + j;
                matrizsaida.vetores[msaidaindex] += matriz1.vetores[m1index]*matriz2.vetores[m2index];
            }
        }
    }
    escreveMatrizBinario(matrizsaida, argv[3]);

    /*for (int i = 0; i<matrizsaida.linhas; i++){
        for (int j = 0; j<matrizsaida.colunas; j++){
            msaidaindex = i*matrizsaida.colunas+j;
            printf("%f ", matrizsaida.vetores[msaidaindex]);
        }
        puts("");
    }*/

    GET_TIME(fim);
    delta = fim-inicio;
    printf("Tempo de processamento %lf\n", delta);

    //finalização
    GET_TIME(inicio);
    free(matriz1.vetores);
    free(matriz2.vetores);
    free(matrizsaida.vetores);

    GET_TIME(fim);
    delta = fim-inicio;
    printf("Tempo de finalização %lf\n", delta);

    return 0;
}