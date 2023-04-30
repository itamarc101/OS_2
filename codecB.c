#include <stdio.h>
#include <dlfcn.h>


void encodeB(char * msg)
{
    int i;
    for (i = 0; i < msg[i] != '\0'; i++)
    {
        msg[i] += 3;
    }

    printf("%s\n", msg);
}

void decodeB(char * msg)
{
    int i;
    for (i = 0; i < msg[i] != '\0'; i++)
    {
        msg[i] -= 3;
    }

    printf("%s\n", msg);
}