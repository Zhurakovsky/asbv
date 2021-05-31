#include <iostream>
#include <unistd.h>
#include <sys/types.h>


using namespace std;

int main()
{
    while(true)
    {
        cout << "Hello from actuator: " << getpid() << endl;
        sleep(1);
    }
    return 0;
}