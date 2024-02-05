//Name: Xiyao Xu
//Class: CS6013
//Date: 2/5/2024
//Overview：This program is a basic example of IPC programming, showing how to safely pass information between parent and child processes. Through pipes, simple data transmission is achieved, but it is important to note that when using pipes for inter-process communication, ensure data synchronization and correct pipe end closing to avoid deadlock or data loss.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pipefd[2]; // Array to hold the file descriptors for the pipe
    pid_t pid; // Variable to store the process ID
    int status; // Variable to store the status of the child process

    // Check if the command line argument is less than 2. If so, print usage message and exit
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        exit(1);
    }

    // Create a pipe and check for errors
    if (pipe(pipefd) == -1) {
        perror("pipe error");
        exit(1);
    }

    // Create a child process and check for errors
    pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    if (pid > 0) {
        // Parent process
        printf("parent\n");
        close(pipefd[0]); // Close the read-end of the pipe

        // Write the length of the message to the pipe
        int len = strlen(argv[1]) + 1; // Include null terminator
        if (write(pipefd[1], &len, sizeof(len)) != sizeof(len)) {
            perror("write error");
            exit(1);
        }

        // Write the message to the pipe
        if (write(pipefd[1], argv[1], len) != len) {
            perror("write error");
            exit(1);
        }

        close(pipefd[1]); // Close the write-end of the pipe
        waitpid(pid, &status, 0); // Wait for the child process to terminate
    } else {
        // Child process
        close(pipefd[1]); // Close the write-end of the pipe

        // Read the length of the message from the pipe
        int len;
        if (read(pipefd[0], &len, sizeof(len)) != sizeof(len)) {
            perror("read error");
            exit(1);
        }

        // Allocate memory for the message and check for errors
        char* message = static_cast<char *>(malloc(len));
        if (message == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        // Read the message from the pipe
        if (read(pipefd[0], message, len) != len) {
            perror("read error");
            free(message); // Free the allocated memory
            exit(1);
        }

        // Print the received message
        printf("child received: %s\n", message);
        free(message); // Free the allocated memory
        close(pipefd[0]); // Close the read-end of the pipe
        exit(0);
    }
    return 0;
}
