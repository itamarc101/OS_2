#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

char*(*decodeP)(char *);

int main(int args, char * argv[])
{
    if(args != 3)
    {
        printf("Usage: %s decode <codec> <message> \n", argv[0]);
        exit(0);
    }   

    char * codec = argv[1];
    char * msg = argv[2];
    void* mylib = NULL;

    if(strcmp(codec, "codecA") == 0)
    {
        mylib = dlopen("./libcodecA.so", RTLD_LAZY);
        decodeP = dlsym(mylib, "decodeA");
    } 

    else if(strcmp(codec, "codecB") == 0)
    {
        mylib = dlopen("./libcodecB.so", RTLD_LAZY);
        decodeP = dlsym(mylib, "decodeB");
    }

    else
    {
        printf("No such codec: %s\n", codec);
        return 1;
    }

    if(!mylib)
    {
        printf("error opening lib\n");
        return 1;
    }
    
    
    if(!decodeP)
    {
        printf("Error with function\n");
        dlclose(mylib);
        return 1;
    }

    decodeP(msg);
    dlclose(mylib);

    return 0;

}