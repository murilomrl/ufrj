/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 2 */
/* Codigo: Multiplica uma matriz por um vetor */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//variaveis globais
//float *matA; //matriz de entrada
//float *vetX; //vetor de entrada
//float *vetB; //vetor de saida
int nthreads;
float *matA; //matriz de entrada
float *matB; //matriz de entrada
float *matC; //matriz de saida
FILE *arqA, *arqB, *arqC; //arquivos dos dados de entrada e saida
int linhas, colunas; //dimensoes da matriz de entrada
int linhas2, colunas2; //dimensoes da matriz de entrada
//funcao que multiplica matriz por vetor (A * X = B)
//entrada: matriz de entrada, vetor de entrada, vetor de saida, dimensoes da matriz
//requisito 1: o numero de colunas da matriz eh igual ao numero de elementos do vetor de entrada
//requisito 2: o numero de linhas da matriz eh igual ao numero de elementos do vetor de saida


void *multiplicaMatrizes(void *tid) {
   int inicio, fim;
   int i = * (int *) tid;
   int j,k;
   int tam_bloco = linhas/nthreads;
   //int z = 0;
   inicio = i * tam_bloco;
   if (i<nthreads-1) fim = inicio + tam_bloco;
   else fim = linhas;
   for (i=inicio;i<fim; i++){
     for (j=0; j<colunas; j++){
       for (k=0; k<colunas; k++){
       	// if(z > 34122750)
       	// {
       	//        	printf("1:  %d\n", i*colunas+j);
       	//        	printf("2:  %d\n", i*colunas+k);
       	//        	printf("3:  %d\n", k*colunas+j);
       	//        	printf("i:  %d\n", i);
       	//        	printf("j:  %d\n", j);
       	//        	printf("k:  %d\n", k);
       	//        	printf("Z:  %d\n", z);
       	//        	printf("%f\n", matC[i*colunas+j]);
       	// }
       	// z += 1;

       	matC[i*colunas+j] += (matA[i*colunas+k] * matB[k*colunas+j]);


       }
     }
   }
}

//funcao que aloca espaco para uma matriz e preenche seus valores
//entrada: matriz de entrada, dimensoes da matriz
//saida: retorna 1 se a matriz foi preenchida com sucesso e 0 caso contrario
int preencheMatriz(float **mat, int linhas, int colunas, FILE *arq) {
   int i, j;
   //aloca espaco de memoria para a matriz
   *mat = (float*) malloc(sizeof(float) * linhas * colunas);
   if (mat == NULL) return 0;
   //preenche o vetor
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         //fscanf(arq, "%f", *( (*mat) + (i*colunas+j) ) );
      	
        fscanf(arq, "%f", (*mat) + (i*colunas+j));        

      }
   }
   return 1;
}

int preencheMatrizSaida(int linhas, int colunas) {
   int i, j;
   //aloca espaco de memoria para a matriz
   matC = (float*) malloc(sizeof(float) * linhas * colunas);
   if (matC == NULL) return 0;
   //preenche o vetor
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         //fscanf(arq, "%f", *( (*mat) + (i*colunas+j) ) );
      	matC[i*colunas+j] = 0;        

      }
   }
   return 1;
}

//funcao que imprime uma matriz
//entrada: matriz de entrada, dimensoes da matriz
//saida: matriz impressa na tela
void imprimeMatriz(float *mat, FILE *arq) {
   int i, j;
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         fprintf(arq, "%.1f ", mat[i*colunas+j]);
      }
      fprintf(arq, "\n");
   }
}


//funcao principal
int main(int argc, char *argv[]) {

	// matC = (float *) malloc(sizeof(float)*1024*1024);
	// int icarus;
	// for(icarus = 0; icarus < 1024*1024; icarus++)
	// {
	// 	matC[icarus] = 0;
	// }

   //le e valida os parametros de entrada
   //o arquivo da matriz de entrada deve conter na primeira linha as dimensoes da matriz (linha coluna) seguido dos elementos da matriz separados por espaco
   //o arquivo do vetor de entrada deve conter na primeira linha a dimensao do vetor seguido dos elementos separados por espaco
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo vetor X> <arquivo vetor B>.\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   //abre o arquivo da matriz de entrada
   arqA = fopen(argv[1], "r");
   if(arqA == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da matriz de entrada.\n");
      exit(EXIT_FAILURE);
   }
   //le as dimensoes da matriz de entrada
   fscanf(arqA, "%d", &linhas);
   fscanf(arqA, "%d", &colunas);

   //abre o arquivo do vetor de entrada
   arqB = fopen(argv[2], "r");
   if(arqB == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da matriz de entrada.\n");
      exit(EXIT_FAILURE);
   }
   //le a dimensao do vetor de entrada
   fscanf(arqB, "%d", &linhas2);
   fscanf(arqB, "%d", &colunas2);

   //valida as dimensoes das matrizes de entrada
   if(colunas != linhas2) {
      fprintf(stderr, "Erro: as dimensoes das matrizes de entrada nao sao compativeis.\n");
      exit(EXIT_FAILURE);
   }

   //abre o arquivo do vetor de saida
   arqC = fopen(argv[3], "w");
   if(arqC == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da matriz de saida.\n");
      exit(EXIT_FAILURE);
   }

   //aloca e preenche a matriz de entrada
   if(preencheMatriz(&matA, linhas, colunas, arqA) == 0) {
      fprintf(stderr, "Erro de preenchimento da matriz de entrada\n");
      exit(EXIT_FAILURE);
   }
   if(preencheMatriz(&matB, linhas, colunas, arqB) == 0) {
      fprintf(stderr, "Erro de preenchimento da matriz de entrada\n");
      exit(EXIT_FAILURE);
   }
   if(preencheMatrizSaida(linhas, colunas) == 0) {
      fprintf(stderr, "Erro de preenchimento da matriz de saida\n");
      exit(EXIT_FAILURE);
   }

   /*
   //imprime a matriz de entrada
   printf("Matriz de entrada:\n");
   imprimeMatriz(matA, linhas, colunas, stdout);
   //imprime o vetor de entrada
   printf("Vetor de entrada:\n");
   imprimeVetor(vetX, colunas, stdout);
   */
   nthreads = atoi(argv[4]); //numero de threads
   pthread_t *tid_sistema; //vetor de identificadores das threads no sistema
   int *tid; //identificadores das threads no programa
   int t; //variavel contadora
   //aloca espaco para o vetor de identificadores das threads no sistema
   tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid_sistema==NULL) {
     printf("--ERRO: malloc()\n"); exit(-1);
   }
   for(t=0; t<nthreads; t++) {
     tid = malloc(sizeof(int)); if(tid==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
     *tid = t;

     if (pthread_create(&tid_sistema[t], NULL, multiplicaMatrizes, (void*) tid)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }
   //espera todas as threads terminarem e imprime o vetor de saída
   for(t=0; t<nthreads; t++) {
     if (pthread_join(tid_sistema[t], NULL)) {
       printf("--ERRO: pthread_join()\n"); exit(-1);
     }
   }
   //multiplica a matriz de entrada pelo vetor de entrada
   //multiplicaMatrizVetor(matA, vetX, vetB, linhas, colunas);

   //imprime o vetor de saida no arquivo de saida
   imprimeMatriz(matC, arqC);

   //libera os espacos de memoria alocados
   free(matA);
   free(matB);
   free(matC);

   return 0;
}
