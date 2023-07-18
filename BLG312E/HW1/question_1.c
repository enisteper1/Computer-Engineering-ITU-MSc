#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Recursive tree based fork structure generation
void process_tree(int N, int ppid)
{
    
    // Multiply the process
    int p = fork();
    if(p == -1)
        printf("Fork operation failed!\n");
    // If child process assign  it as a left child
    else if(p == 0)
        printf("Parent: %d Left Child: %d\n", ppid, getpid());
    // If parent
    else
    {
        // Wait left child to be printed
        wait(NULL);
        // To not create last right child
        if(N==1)
            return;
        int p2 = fork();
        if(p2 == 0)
        {
            printf("Parent: %d Right Child: %d\n", ppid, getpid());
            process_tree(N-1, getpid());
            // Terminate parent
            exit(0);
        }
        // If parent
        else
        {
            // Wait right child to be printed
            wait(NULL);
            // Terminate parent
            exit(0);
        }
    }
    // Terminate remaining child process
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("N value should be passed!");
        return -1;
    }
    process_tree(atoi(argv[1])+1, getpid());
    return 0;
}