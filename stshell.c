#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 10

void sigint_handler(int signum)
{
    printf("\n"); // newline
}

void execute_command(char **argv, int input_fd, int output_fd, int *pipe_fds)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // child process
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        if (pipe_fds != NULL)
        {
            close(pipe_fds[0]); // close read end of pipe
            dup2(pipe_fds[1], STDOUT_FILENO);
            close(pipe_fds[1]);
        }
        execvp(argv[0], argv);
        perror("execvp failed");
        exit(1);
    }
    else
    {
        // parent process
        if (pipe_fds != NULL)
        {
            close(pipe_fds[1]); // close write end of pipe
        }
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
        do
        {
            waitpid(pid, &status, 0);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));   
    }
}

void parse_command(char *command, char **argv, int *num_args, int *input_fd, int *output_fd, int *pipe_fds)
{
    *num_args = 0;
    *input_fd = STDIN_FILENO;
    *output_fd = STDOUT_FILENO;
    *pipe_fds = -1;

    // token command string by spaces
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        if (strcmp(token, ">") == 0)
        {
            // output
            token = strtok(NULL, " ");
            *output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*output_fd == -1)
            {
                perror("open failed");
                return; // return without executing the command
            }
        }
        else if (strcmp(token, ">>") == 0)
        {
            // output (append)
            token = strtok(NULL, " ");
            *output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (*output_fd == -1)
            {
                perror("open failed");
                return; // return without executing the command
            }
        }
        else if (strcmp(token, "|") == 0)
        {
            // pipe
            int pipefds[2];
            if (pipe(pipefds) == -1)
            {
                perror("pipe failed");
                return; // return without executing the command
            }
            *pipe_fds = pipefds[0];  // read end of pipe
            *output_fd = pipefds[1]; // write end of pipe
            argv[*num_args] = NULL;         
            execute_command(argv, *input_fd, *output_fd, pipefds);
            *num_args = 0;         // reset argument count
            *input_fd = *pipe_fds; // set input to read end of pipe
            *pipe_fds = -1;        // reset pipe
        }
        else
        {
            // regular argument
            argv[*num_args] = token;
            (*num_args)++;
        }
        token = strtok(NULL, " ");
    }
    argv[*num_args] = NULL; 
    execute_command(argv, *input_fd, *output_fd, pipe_fds);
}

int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *argv[MAX_ARGUMENTS];
    int num_args, input_fd, output_fd, pipe_fds;
    
    signal(SIGINT, sigint_handler);

    while (1)
    {
        printf("stshell : ");
	    fgets(command, 1024, stdin);
	    command[strlen(command) - 1] = '\0'; // replace \n with \0

        if(strcmp(command, "exit") == 0) break;

        // parse command
        parse_command(command, argv, &num_args, &input_fd, &output_fd, &pipe_fds);
    }

    return 0;
}