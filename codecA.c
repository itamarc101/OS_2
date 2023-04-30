#include <stdio.h>
#include <string.h>


void encodeA(char * msg)
{
    int i;
    for (i = 0; msg[i] != '0'; i++)
    {
        if(msg[i] >= 65 && msg[i] <= 90)
        msg[i] += 32;
        else if(msg[i] >= 97 && msg[i] <= 122)
        msg[i] -= 32;
    }

    printf("%s\n", msg);
    
}

void decodeA(char * msg)
{
    encodeA(msg);
}