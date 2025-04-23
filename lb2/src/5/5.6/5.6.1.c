// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include <pthread.h>
// // #include <sched.h>
// // #include <errno.h>
// // #include <string.h>

// // #define NUM_THREADS 3

// // typedef struct {
// //     const char* thread_name;
// //     int scheduling_policy;
// //     int thread_priority;
// // } ThreadConfig;

// // void display_thread_info(const char* thread_name) {
// //     int current_policy;
// //     struct sched_param params;
    
// //     if (pthread_getschedparam(pthread_self(), &current_policy, &params) != 0) {
// //         perror("Failed to get thread scheduling parameters");
// //         return;
// //     }

// //     const char* policy_name;
// //     switch (current_policy) {
// //         case SCHED_FIFO:  policy_name = "FIFO"; break;
// //         case SCHED_RR:    policy_name = "RR"; break;
// //         case SCHED_OTHER: policy_name = "OTHER"; break;
// //         default:          policy_name = "UNKNOWN";
// //     }

// //     printf("[%s] Thread ID: %lu | Policy: %-5s | Priority: %2d\n", 
// //            thread_name, (unsigned long)pthread_self(), policy_name, params.sched_priority);
// //     fflush(stdout);
// // }

// // void* thread_work(void* config_ptr) {
// //     ThreadConfig* config = (ThreadConfig*) config_ptr;
// //     while (1) {
// //         // Имитация работы потока
// //         volatile unsigned long counter = 0;
// //         for (unsigned long j = 0; j < 1000000000UL; j++) {
// //             counter++;
// //         }
// //         display_thread_info(config->thread_name);
// //     }
// //     return NULL;
// // }

// // int main(int argc, char* argv[]) {
// //     pthread_t thread_handles[NUM_THREADS];
// //     ThreadConfig thread_configs[NUM_THREADS];
// //     int status;

// //     // Настройки политик планирования
// //     int policies[NUM_THREADS] = {SCHED_OTHER, SCHED_RR, SCHED_FIFO};
    
// //     // Приоритеты по умолчанию
// //     int fifo_priority = 50;
// //     int rr_priority = 50;

// //     // Обработка аргументов командной строки
// //     if (argc > 2) {
// //         fifo_priority = atoi(argv[1]);
// //         rr_priority = atoi(argv[2]);
// //     } 

// //     // Создание и настройка потоков
// //     for (int i = 0; i < NUM_THREADS; i++) {
// //         // Настройка имени потока
// //         switch(i) {
// //             case 0: thread_configs[i].thread_name = "OTHER"; break;
// //             case 1: thread_configs[i].thread_name = "RR"; break;
// //             case 2: thread_configs[i].thread_name = "FIFO"; break;
// //         }
        
// //         thread_configs[i].scheduling_policy = policies[i];

// //         // Инициализация атрибутов потока
// //         pthread_attr_t thread_attrs;
// //         status = pthread_attr_init(&thread_attrs);
// //         if (status != 0) {
// //             fprintf(stderr, "Thread attr init failed: %s\n", strerror(status));
// //             exit(EXIT_FAILURE);
// //         }

// //         // Установка явного наследования параметров
// //         status = pthread_attr_setinheritsched(&thread_attrs, PTHREAD_EXPLICIT_SCHED);
// //         if (status != 0) {
// //             fprintf(stderr, "Set inherit sched failed: %s\n", strerror(status));
// //             exit(EXIT_FAILURE);
// //         }

// //         // Установка политики планирования
// //         status = pthread_attr_setschedpolicy(&thread_attrs, thread_configs[i].scheduling_policy);
// //         if (status != 0) {
// //             fprintf(stderr, "Set policy failed for %s: %s\n", 
// //                     thread_configs[i].thread_name, strerror(status));
// //         }

// //         // Настройка приоритета
// //         struct sched_param priority_params;
// //         if (thread_configs[i].scheduling_policy == SCHED_FIFO) {
// //             priority_params.sched_priority = fifo_priority;
// //         } else if (thread_configs[i].scheduling_policy == SCHED_RR) {
// //             priority_params.sched_priority = rr_priority;
// //         } else {
// //             priority_params.sched_priority = 0;
// //         }

// //         status = pthread_attr_setschedparam(&thread_attrs, &priority_params);
// //         if (status != 0) {
// //             fprintf(stderr, "Set priority failed for %s: %s\n", 
// //                     thread_configs[i].thread_name, strerror(status));
// //         }
// //         thread_configs[i].thread_priority = priority_params.sched_priority;

// //         // Создание потока
// //         status = pthread_create(&thread_handles[i], &thread_attrs, thread_work, &thread_configs[i]);
// //         if (status != 0) {
// //             fprintf(stderr, "Thread creation failed for %s: %s\n", 
// //                     thread_configs[i].thread_name, strerror(status));
// //             exit(EXIT_FAILURE);
// //         }
// //         pthread_attr_destroy(&thread_attrs);
// //     }

// //     // Ожидание завершения потоков 
// //     for (int i = 0; i < NUM_THREADS; i++) {
// //         pthread_join(thread_handles[i], NULL);
// //     }

// //     return 0;
// // }
// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <sched.h>
// #include <time.h>

// #define NUM_THREADS 5
// #define SCHED_THREADS 2

// // Функция, которую выполняют потоки
// void* thread_function(void* arg) {
//     int thread_num = *(int*)arg;
//     struct timespec start, current;
//     clock_gettime(CLOCK_MONOTONIC, &start);
    
//     // Получаем параметры планирования текущего потока
//     int policy;
//     struct sched_param param;
//     pthread_getschedparam(pthread_self(), &policy, &param);
    
//     const char* policy_str;
//     switch(policy) {
//         case SCHED_FIFO: policy_str = "SCHED_FIFO"; break;
//         case SCHED_RR: policy_str = "SCHED_RR"; break;
//         default: policy_str = "SCHED_OTHER"; break;
//     }
    
//     printf("Thread %d started with policy %s and priority %d\n", 
//            thread_num, policy_str, param.sched_priority);
    
//     // Имитация работы потока
//     long counter = 0;
//     // while (1) {
//     //     for (int counter, i = 0; i < 100000000;i++) counter++;
//     // }
//     for (int i = 0; i < 10; i++) {
//         printf("Thread %d (%s) working... step %d\n", 
//                thread_num, policy_str, i+1);
//         // Искусственная задержка для наглядности
//         for (volatile int j = 0; j < 10000000; j++);
//     }
    
//     printf("Thread %d completed %ld iterations\n", thread_num, counter);
//     return NULL;
// }

// int main() {
//     pthread_t threads[NUM_THREADS];
//     int thread_nums[NUM_THREADS];
    
//     // Создаем потоки
//     for (int i = 0; i < NUM_THREADS; i++) {
//         thread_nums[i] = i;
//         pthread_attr_t attr;
//         pthread_attr_init(&attr);
        
//         // Для первых двух потоков устанавливаем SCHED_FIFO с приоритетом 50
//         if (i < SCHED_THREADS) {
//             pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
//             pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
//             struct sched_param param;
//             param.sched_priority = 50; // одинаковый приоритет
//             pthread_attr_setschedparam(&attr, &param);
//         }
        
//         if (pthread_create(&threads[i], (i < SCHED_THREADS) ? &attr : NULL, 
//                           thread_function, &thread_nums[i]) != 0) {
//             perror("pthread_create");
//             return 1;
//         }
        
//         pthread_attr_destroy(&attr);
//     }
    
//     // Ожидаем завершения всех потоков
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }
    
//     printf("All threads completed\n");
//     return 0;
// }

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

#define NUM_THREADS 5
#define SCHED_THREADS 2

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
    
    printf("Поток %d (%s) начал работу\n", thread_num, policy_str);
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
    int thread_nums[NUM_THREADS];
    
    // Инициализируем барьер на NUM_THREADS + 1 (потоки + главный поток)
    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);
    
    printf("Создание %d потоков...\n", NUM_THREADS);
    fflush(stdout);
    // Создаем все потоки
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_nums[i] = i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        
        if (i < SCHED_THREADS) {
            pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
            struct sched_param param;
            param.sched_priority = 99;
            pthread_attr_setschedparam(&attr, &param);
        }
        
        if (pthread_create(&threads[i], (i < SCHED_THREADS) ? &attr : NULL, 
                          thread_function, &thread_nums[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
        
        pthread_attr_destroy(&attr);
    }
    
    // Разрешаем всем потокам начать работу
    printf("Запуск созданных потоков...\n");
    fflush(stdout);
    pthread_barrier_wait(&barrier);
    // Ожидаем завершения
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    printf("Выполнение завершено\n");
    return 0;
}