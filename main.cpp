#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    int status;
    char* args[2];
    string child_app = "./sensorApp";
    args[0] = (char*)child_app.c_str();
    args[1] = NULL;

    pid_t pid = fork();
    
    switch(pid)
    {
        case -1:
            perror("Scanner fork");
            break;

        case 0:
            if ((execvp (args[0], args)) == -1 )
            {
                perror("execvp");
            }
            break;

        default:
            wait(0);
    }

    return 0;
}
