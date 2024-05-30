#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int x = 0;
sem_t sync_;

void a() {
    sleep(10);
    printf("X = %d\n", x);
    x = 55;
}

void* p1(void* arg) {
    a();
    sem_post(&sync_);
    return NULL;
}

void b() {
    printf("X = %d\n", x);
}

void* p2(void* arg) {
    sem_wait(&sync_);
    b();
    return NULL;
}

void main() {
    pthread_t thread1, thread2;

    if (sem_init(&sync_, 0, 0) == -1) {
        perror("Could not initialize mylock semaphore");
        exit(2);
    }
    if (pthread_create(&thread1, NULL, p1, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, p2, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sem_destroy(&sync_);
    exit(0);
}
