#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    struct timeval start_time, end_time;

    // Create a pipe for communication
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) { // Child process
        // Close the unused read end of the pipe
        close(pipe_fd[READ_END]);

        // Get the starting time and write it to the pipe
        gettimeofday(&start_time, NULL);
        write(pipe_fd[WRITE_END], &start_time, sizeof(start_time));

        // Replace the child process with the specified command
        execlp(argv[1], argv[1], NULL);

        // If execlp fails, print an error message
        perror("exec failed");
        exit(1);    
    } else { // Parent process
        // Close the unused write end of the pipe
        close(pipe_fd[WRITE_END]);

        wait(NULL); // Wait for the child to complete

        // Get the ending time after the child has finished
        gettimeofday(&end_time, NULL);

        // Read the starting time from the pipe
        read(pipe_fd[READ_END], &start_time, sizeof(start_time));

        // Calculate the elapsed time
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                             (end_time.tv_usec - start_time.tv_usec) / 1e6;

        // Print the elapsed time
        printf("Elapsed time: %.5f seconds\n", elapsed_time);
    }

    return 0;
}

// usage: gcc time-pipe.c -o time-pipe then ./time-pipe <command> <args>