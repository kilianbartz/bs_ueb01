#include <unistd.h>

#define NUM_ITERATIONS 100000

int main()
{
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        sleep(0);
    }
    return 0;
}
