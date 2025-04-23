#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

#define NUM_THREADS 5

// Структура для передачи параметров потока
typedef struct {
    int thread_num;
    int priority;
} thread_params_t;

pthread_barrier_t barrier;

void* thread_function(void* arg) {
    int thread_num = *(int*)arg;
    
    // Ждем, пока все потоки будут созданы
    pthread_barrier_wait(&barrier);
    
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    
    const char* policy_str;
    switch(policy) {
        case SCHED_FIFO: policy_str = "SCHED_FIFO"; break;
        case SCHED_RR: policy_str = "SCHED_RR"; break;
        default: policy_str = "SCHED_OTHER"; break;
    }
    
    printf("Поток %d (%s) начал работу с приоритетом %d\n", thread_num, policy_str, param.sched_priority);
    fflush(stdout);
    for (int i = 0; i < 6; i++) {
        printf("Поток %d (%s) на шаге %d\n", thread_num, policy_str, i+1);
        fflush(stdout);
        for (volatile int j = 0; j < 100000000; j++);
        // sched_yield();
    }
    
    printf("Thread %d completed\n", thread_num);
    fflush(stdout);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_params_t params[NUM_THREADS];
    
    // Инициализируем барьер (потоки + главный поток)
    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);
    
    printf("Creating %d threads with different priorities...\n", NUM_THREADS);
    
    // Создаем все потоки с разными приоритетами
    for (int i = 0; i < NUM_THREADS; i++) {
        params[i].thread_num = i;
        params[i].priority = 50 + i*10; // Приоритеты: 50, 60, 70, 80, 90
        
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        
        // Устанавливаем политику SCHED_FIFO для всех потоков
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        
        struct sched_param param;
        param.sched_priority = params[i].priority;
        pthread_attr_setschedparam(&attr, &param);
        
        if (pthread_create(&threads[i], &attr, thread_function, &params[i])) {
            perror("pthread_create");
            return 1;
        }
        
        pthread_attr_destroy(&attr);
    }
    
    // Разрешаем всем потокам начать работу
    printf("All threads created, releasing barrier...\n");
    pthread_barrier_wait(&barrier);
    
    // Ожидаем завершения
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    printf("All threads completed\n");
    return 0;
}