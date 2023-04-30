#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

char*(*encodeP)(char *);

int main(int args, char * argv[])
{
    if(args != 3)
    {
        printf("Usage: %s encode <codec> <message> \n", argv[0]);
        exit(0);
    }   

    char * codec = argv[1];
    char * msg = argv[2];
    void* mylib = NULL;

    if(strcmp(codec, "codecA") == 0)
    {
        mylib = dlopen("./libcodecA.so", RTLD_LAZY);
        encodeP = dlsym(mylib, "encodeA");
    } 

    else if(strcmp(codec, "codecB") == 0)
    {
        mylib = dlopen("./libcodecB.so", RTLD_LAZY);
        encodeP = dlsym(mylib, "encodeB");
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
    
    
    if(!encodeP)
    {
        printf("Error with function\n");
        dlclose(mylib);
        return 1;
    }

    encodeP(msg);
    dlclose(mylib);

    return 0;

}