/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
using namespace std;
#include <cstdlib>
#include <iostream>

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};

    // TODO: add functionality
    // Create pipe
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    // Create child to run first command
    pid_t pid;
    pid = fork();
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // In child, redirect output to write end of pipe
    // Close the read end of the pipe on the child side.
    // In child, execute the command
    if(pid == 0){
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        execvp(cmd1[0], cmd1);
    }


    // Create another child to run second command
    pid_t pid2 = fork();
    // In child, redirect input to the read end of the pipe
    // Close the write end of the pipe on the child side.
    // Execute the second command.
    if(pid2 == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(pid2 == 0){
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        execvp(cmd2[0], cmd2);
    }

    // Reset the input and output file descriptors of the parent.

    close(pipefd[0]);
    close(pipefd[1]);
}
