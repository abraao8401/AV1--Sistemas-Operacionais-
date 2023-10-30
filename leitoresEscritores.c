
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define LEITORES 10 
#define ESCRITORES 5 

pthread_mutex_t bancoDados; //controla o acesso ao banco de dados
pthread_mutex_t mutex;      // controla o acesso a variávelleitoresAtivos
int leitoresAtivos;           // quantidade de leitores lendo o banco de dados

void *escrita(void *idEscritor);
void *leitura(void *idLeitor);
void lerDoBanco(int idLeitor);
void utilizarDados(int idLeitor);
void pensarEscrita(int idEscritor);
void escreverNoBanco(int idEscritor);



int main(){

    pthread_t threadEscritores[ESCRITORES], threadLeitores[LEITORES];
    int idLeitor[LEITORES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int idEscritor[ESCRITORES] = {1, 2, 3, 4, 5};

    int i;

    // inicializacao dos semaforos...
    pthread_mutex_init(&bancoDados, NULL);
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < ESCRITORES; i++){
        pthread_create(&threadEscritores[i], NULL, escrita, &idEscritor[i]);
    }
    
    for (i = 0; i < LEITORES; i++){
        pthread_create(&threadLeitores[i], NULL, leitura, &idLeitor[i]);
    }

    for (i = 0; i < ESCRITORES; i++){
        pthread_join(threadEscritores[i], NULL);
    }

    for (i = 0; i < LEITORES; i++){
        pthread_join(threadLeitores[i], NULL);
    }

    return 0;
}

void *leitura(void *idLeitor){
    int *id = idLeitor;
    while (1){ 

        pthread_mutex_lock(&mutex);      //acesso exclusivo a variave leitoresAtivos
        leitoresAtivos = leitoresAtivos + 1; // um novo leitor

        if (leitoresAtivos == 1)
            pthread_mutex_lock(&bancoDados); //é o primeiro leitor? 
        pthread_mutex_unlock(&mutex);        //libera o acesso a variavel leitoresAtivos

        lerDoBanco(*id);             
        pthread_mutex_lock(&mutex);      
        leitoresAtivos = leitoresAtivos - 1; // um leitor a menos...

        if (leitoresAtivos == 0)
            pthread_mutex_unlock(&bancoDados); // é ultimo leitor? 
        pthread_mutex_unlock(&mutex);          

        utilizarDados(*id); 
    }
}

void *escrita(void *idEscritor){
    int *id = idEscritor;
    while (1){
        pensarEscrita(*id);              
        pthread_mutex_lock(&bancoDados); //garante acesso exclusivo a banco de dados
        escreverNoBanco(*id);            // escreve no banco de dados

        pthread_mutex_unlock(&bancoDados); // up(&bancoDados); libera o acesso ao bd 
    }
}

void lerDoBanco(int idLeitor){

    printf("Leitor %d lendo banco de dados. Total: %d.\n", idLeitor, leitoresAtivos);
    sleep(3);
}

void utilizarDados(int idLeitor){

    printf("Leitor %d utilizando dados do Banco\n", idLeitor);
    sleep(2);
}

void pensarEscrita(int idEscritor){

    printf("Escritor %d pensando na escrita\n", idEscritor);
    sleep(3);
}

void escreverNoBanco(int idEscritor){

    printf("Escritor %d escrevendo no banco de dados\n", idEscritor);
    sleep(2);
    printf("Escritor %d terminou de escrever\n", idEscritor);
    sleep(1);
}

