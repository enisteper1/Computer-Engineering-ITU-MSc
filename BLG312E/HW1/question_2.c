#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void process_tree_M(int M, int ppid)
{
    // If last left child  created return
    if(M == 0)
        return;
    // Generate left child
    int p = fork();
    // If child
    if(p == 0)
    {   
        // Print and generate next left recursively
        printf("Parent: %d, Left Child: %d\n", ppid, getpid());
        process_tree_M(M-1, getpid());
    }
}
void process_tree_N_M(int N, int M, int ppid)
{

    int p = fork();
    if(p == 0)
    {
        printf("Parent: %d, Left Child: %d\n", ppid, getpid()); 
        process_tree_M(M-1, getpid());
    }
    else
    {
        // Wait left childs to be printed
        wait(NULL);
        // To not create last right child
        if(N==1)
            return;
        int p2 = fork();
        if(p2 == 0)
        {
            printf("Parent: %d, Right Child: %d\n", ppid, getpid());
            process_tree_N_M(N-1, M, getpid());
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
    if (argc < 3) {
        printf("N and M value should be passed!\n");
        return -1;
    }
    
    process_tree_N_M(atoi(argv[1])+1, atoi(argv[2]), getpid());
    return 0;
    
}