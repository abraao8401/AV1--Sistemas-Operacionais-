#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define N 5
#define PENSANDO 0
#define COMFOME 1
#define COMENDO 2
#define ESQUERDA (idFilosofo + 4) % N
#define DIREITA (idFilosofo + 1) % N


void nome_Filosofos();  //inicializa o nome dos filosfos
void *filosofo(void *num); //funcao principal das threads
void pegar_Garfos(int); 
void deixar_Garfos(int);
void teste(int); //verifica se é possivel pegar os garfos nescessarios e comer 

sem_t mutex;
sem_t S[N];

int estado[N];
int idFilosofo[N] = {0, 1, 2, 3, 4};
char **nomes;


int main()
{
    int i;
    nome_Filosofos();
    pthread_t identificadorThread[N]; // identificadores das
                                      // threads
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++){
        sem_init(&S[i], 0, 0);
    }
    for (i = 0; i < N; i++){
        pthread_create(&identificadorThread[i], NULL, filosofo, &idFilosofo[i]);
        // criar as threads
        printf("Filosofo %d (%s) esta a pensar.\n", i + 1, nomes[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(identificadorThread[i], NULL); // para
                                                    // fazer a junção das threads
    return (0);
}

void nome_Filosofos()
{
    nomes = (char **)malloc(5 * sizeof(char *));
    if (nomes == NULL){
        printf("Falha na alocação de memória.\n");
        return;
    }
    nomes[0] = "Socrates";
    nomes[1] = "Aristoteles";
    nomes[2] = "Platao";
    nomes[3] = "Hipatia";
    nomes[4] = "Democrito";
}

void *filosofo(void *num)
{
    while (1)
    {
        int *nFilosofo = num;
        sleep(1);
        pegar_Garfos(*nFilosofo);
        sleep(1);
        deixar_Garfos(*nFilosofo);
    }
}

void pegar_Garfos(int idFilosofo)
{
    sem_wait(&mutex);
    estado[idFilosofo] = COMFOME;
    printf("Filosofo %d (%s) esta com fome.\n", idFilosofo + 1, nomes[idFilosofo]);
    teste(idFilosofo);
    sem_post(&mutex);
    sem_wait(&S[idFilosofo]);
    sleep(3);
}

void teste(int idFilosofo)
{
    if (estado[idFilosofo] == COMFOME && estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO)
    {
        estado[idFilosofo] = COMENDO;
        sleep(2);
        printf("Filosofo %d (%s) pegou os garfos %d e %d.\n", idFilosofo + 1, nomes[idFilosofo], ESQUERDA + 1, idFilosofo + 1);
        printf("Filosofo %d (%s) esta comendo.\n", idFilosofo + 1, nomes[idFilosofo]);
        sem_post(&S[idFilosofo]);
    }
}

void deixar_Garfos(int idFilosofo)
{
    sem_wait(&mutex);
    estado[idFilosofo] = PENSANDO;
    printf("Filosofo %d (%s) deixou os garfos %d e %d.\n", idFilosofo + 1, nomes[idFilosofo], ESQUERDA + 1, idFilosofo + 1);
    printf("Filosofo %d (%s) esta pensando.\n", idFilosofo + 1, nomes[idFilosofo]);
    teste(ESQUERDA);
    teste(DIREITA);
    sem_post(&mutex);
}

