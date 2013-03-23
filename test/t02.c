#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Ok. pid = %d\nforever loop\n", getpid());
    while (1);
    return 0;
}
