#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    long long int num_syscalls = 100000000;

    for (long long int i = 0; i < num_syscalls; i++)
    {
        getpid();
    }

    return 0;
}