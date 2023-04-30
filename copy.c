#include <stdio.h>
#include <string.h>
FILE* f1;
FILE* f2;

int main(int count, char *argv[])
{

    int overWrite = 0;
    int verbose = 0;
    for (int i = 3; i < count; i++)
    {
        if(strcmp(argv[i],"-v") == 0)
            verbose = 1;
        else if(strcmp(argv[i], "-f") == 0)
            overWrite = 1;
    }

    f1 = fopen(argv[1], "r");

    if((f2 = fopen(argv[2], "r"))){
        if(verbose == 1 )
            printf("target file exist\n");
        if(overWrite == 0){
            fclose(f2);
            fclose(f1);
            return 1;
        }
    }
    f2 = fopen(argv[2], "w");
    
    if(f1 == NULL || f2 == NULL)
    {
        if(verbose == 1 )
            printf("general failure\n");
        fclose(f1);
        fclose(f2);
        return 1;
    }
    
    fseek(f1, 0, SEEK_SET);
    int f1p;
    
    while((f1p = fgetc(f1)) != EOF )
    {
        if(!f1p){
            if(verbose == 1 )
                printf("general failure\n");
            fclose(f1);
            fclose(f2);
            return 1;
        }
        if(!fputc(f1p, f2)){
            if(verbose == 1 )
                printf("general failure\n");
            fclose(f1);
            fclose(f2);
            return 1;
        }
        
    }
    if(verbose)
        printf("success\n");
    fclose(f1);
    fclose(f2);
    return 0;

}