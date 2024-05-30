#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int x = 0;
sem_t sync1, sync2;

void a1(){
  printf("a1()\n");
  sleep(1);
}

void a2(){
  printf("a2()\n");
}

void* thread1(void* arg) {
    a1();
    sem_post(&sync1);
    sem_wait(&sync2);
    a2();
    return NULL;
}

void b1(){
  printf("b1()\n");
  sleep(1);
}

void b2(){
  printf("b2()\n");
}

void* thread2(void* arg) {
    b1();
    sem_post(&sync1);
    sem_wait(&sync2);
    b2();
    return NULL;
}

int main() {
    pthread_t t1, t2;

    if (sem_init(&sync1, 0, 0) == -1) {
        perror("Could not initialize sync1 semaphore");
        exit(2);
    }
    if (sem_init(&sync2, 0, 0) == -1) {
        perror("Could not initialize sync2 semaphore");
        exit(2);
    }

    if (pthread_create(&t1, NULL, thread1, NULL) < 0) {
        perror("Error: thread 1 cannot be created");
        exit(1);
    }
    if (pthread_create(&t2, NULL, thread2, NULL) < 0) {
        perror("Error: thread 2 cannot be created");
        exit(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sync1);
    sem_destroy(&sync2);

    exit(0);
}
