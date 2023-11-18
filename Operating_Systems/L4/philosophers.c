#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 5 // global constant to represent the number of philosophers
#define THINKING 0 // global constant to represent the thinking state
#define HUNGRY 1 // global constant to represent the hungry state
#define EATING 2 // global constant to represent the eating state
#define LEFT (phnum + 4) % N // global constant to calculate the index of the left neighbor of a philosopher
#define RIGHT (phnum + 1) % N // global constant to calculate the index of the right neighbor of a philosopher

int state[N]; // array to store the state of each philosopher
int phil[N] = {0, 1, 2, 3, 4}; // array to identify each philosopher
pthread_mutex_t mutex; // mutex to control access to shared resources
pthread_cond_t condition[N]; // array of condition variables, one for each philosopher

// Function to check if a philosopher can eat based on the states of neighbouring philosophers

void test(int phnum) {
    if (state[phnum] == HUNGRY &&
        state[LEFT] != EATING &&
        state[RIGHT] != EATING) { // If philosopher is hungry and its neighbours are not eating 
        
        state[phnum] = EATING; // changes the state for a particular philosopher to eating to eating

        printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);

        pthread_cond_signal(&condition[phnum]); //signaling the condition variable
    }
}

// function to let a philosopher pick up forks
void take_fork(int phnum) {
    pthread_mutex_lock(&mutex); //acquire the mutex lock
    
    state[phnum] = HUNGRY; //changes the state of philosopher to hungry
    printf("Philosopher %d is Hungry\n", phnum + 1);
    
    test(phnum); // test function call to check if philosopher can eat 
    while (state[phnum] != EATING)
        pthread_cond_wait(&condition[phnum], &mutex);
    pthread_mutex_unlock(&mutex); // releasing mutex lock after philosopher has eaten
}

// function to let a philosopher put down forks
void put_fork(int phnum) {
    pthread_mutex_lock(&mutex); // acquire the mutex lock
    
    state[phnum] = THINKING; // changes the state of philosopher to thinking
    printf("Philosopher %d putting fork %d and %d down\n", phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT); // test function call for its left and right neighbours
    test(RIGHT);

    pthread_mutex_unlock(&mutex); // releasing mutex lock
}

// function to simulate the cycle of thinking, eating and putting down forks for philosophers
void* philosopher(void* num) {
    while (1) {
        int* i = num;
        int thinking_time = 1 + rand() % 3;
        sleep(thinking_time);  // Thinking
        take_fork(*i);
        int eating_time = 1 + rand() % 3;
        sleep(eating_time);  // Eating
        put_fork(*i);
    }
}

// 
int main() {
    int i;
    pthread_t thread_id[N];

    srand(time(NULL)); // seeding random number generator with current time 
    // rand generates number within specified range and srand(time(NULL) provides a different seed value each
    // time the program is run

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (i = 0; i < N; i++) {
        pthread_cond_init(&condition[i], NULL);
    }
    // Creating threads for each philosopher
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }
    // Waiting for all philosophers' threads to finish their work 
    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Clean up by destroying the mutex and condition variables
    pthread_mutex_destroy(&mutex);
    for (i = 0; i < N; i++) {
        pthread_cond_destroy(&condition[i]);
    }
}