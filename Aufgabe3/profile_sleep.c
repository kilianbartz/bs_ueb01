#include <unistd.h>

#define NUM_ITERATIONS 1000000

int main()
{
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        sleep(0);
    }
    return 0;
}
