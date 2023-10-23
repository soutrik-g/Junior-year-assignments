#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const int SIZE = 4096;

    struct timeval *ptr = (struct timeval *)malloc(sizeof(struct timeval));

    // Declare time variables for tracking start and end times
    struct timeval start_time, end_time, elapsed_time;

    // Define a name for the shared memory region
    const char *name = "Shared memory";

    int fd;

    // Create a shared memory region
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    // Configure the size of the shared memory object
    ftruncate(fd, SIZE);

    // Memory map the shared memory object
    ptr = (struct timeval *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork error\n");
    }
    else if (pid == 0)
    {
        // Child process
        struct timeval start_time;
        gettimeofday(&start_time, 0);

        // Store the start time in shared memory as seconds and microseconds
        ptr += sprintf(ptr, "%ld %ld\n", start_time.tv_sec, start_time.tv_usec);

        // Execute the specified command
        execvp(argv[1], &argv[1]);

        // Exit the child process
        exit(0);
    }
    else{
        // Parent process
        wait(NULL);

        struct timeval end_time;
        gettimeofday(&end_time, 0);

        // Calculate final end time in microseconds
        long int finalendmicro = end_time.tv_usec + end_time.tv_sec * 1000000;

        long int startsec = 0;
        long int startmicro = 0;

        // Parse the start time from shared memory
        sscanf(ptr, "%ld %ld", &startsec, &startmicro);

        // Calculate final start time in microseconds
        long int finalstartmicro = startmicro + startsec * 1000000;

        // Calculate elapsed time in microseconds
        long int elapsedmicro = finalendmicro - finalstartmicro;

        // Print the elapsed time in seconds
        printf("Elapsed time: %lf seconds\n", (float)elapsedmicro / 1000000.0);

        // Unlink and close the shared memory region
        shm_unlink(name);
    }

    return 0;
}

// usage: gcc time-shm.c -o time-shm -lrt then ./time-shm <command> <args>
