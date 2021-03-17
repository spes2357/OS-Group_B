#include "michael_PR.c"
#include "parker.c"

// void main(){
//     LLTest();
//     printf("\n\n\n");
//     SSThreadsTest();
// }

int main()
{
    pid_t pid;
    pid = fork(); // creates a child process

    if (pid == 0) // child process
    {
        receiveQueryFromAssistant(); // starts server and begins listening

        exit(0);
    }
    else if (pid > 0) // parent process
    {
        runClient(); // runs manager and assistant functions
    }
    return 0;
}