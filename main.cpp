#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "config_parser/config_parser.hpp"
#include "common/include/types.hpp"

#include <fstream>
#include <iostream>
#include <csignal>

#define MAX_CHILD_ARGS 10

using namespace std;
using namespace poc_autosar;

int child_pids[3];

void signalHandler( int signum )
{
    cout << "Interrupt signal received. Closing child apps\n";
    for (auto pid: child_pids)
    {
        kill(pid, SIGKILL);
    }

    exit(signum);  
}

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int run_child_proc(string proc_name, std::vector<std::string> args_v)
{
    char *args[MAX_CHILD_ARGS];

    if (args_v.size() > MAX_CHILD_ARGS)
    {
        return -1;
    }

    for (int i = 0; i < args_v.size(); i++)
    {
        args[i + 1] = const_cast<char*>(args_v[i].c_str());
    }

    args[0] = const_cast<char*>(proc_name.c_str());
    args[args_v.size() + 1] = NULL;

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork error");
            break;
        case 0:
            if ((execvp (args[0], args)) == -1 )
            {
                perror("execvp");
            }
            break;
    }

    return pid;
}

void print_help_message()
{
    cout << "AutosarPocApp - Self-Balancing vehicle robotics motorcycle POC" << endl << \
    "Usage: ./AutosarPocApp [configFile]" << endl;

}

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandler);

    if (argc != 2)
    {
        cout << "Error parsing arguments" << endl;
        print_help_message();
        return -1;
    }
    else
    {
        if (strlen(argv[1]) == 2 && strstr(argv[1], "-h") == argv[1])
        {
            print_help_message();
            return 0;
        }
        else if (!is_file_exist(argv[1]))
        {
            cout << "config file " << argv[1] << " is not exist" << endl;;
            return -1;
        }
    }

    ConfigParser cfg(argv[1]);
    cfg.parse_config();

    child_pids[0] = run_child_proc("./pidControllerApp"s, cfg.get_pid_config_line());
    child_pids[1] = run_child_proc("./sensorApp"s, cfg.get_sensor_config_line());
    child_pids[2] = run_child_proc("./actuatorApp"s, cfg.get_actuator_config_line());

    while(true)
    {
        sleep(1);
    }

    return 0;
}
