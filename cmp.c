#include <stdio.h>
#include <string.h>
FILE* f1;
FILE* f2;
int lower(int letter){
    if( letter >= 65 && letter <= 90)
        letter += 32;
    return letter;
}
int main(int count, char *argv[])
{

    int ignoreCase = 0;
    int verbose = 0;
    for (int i = 3; i < count; i++)
    {
        if(strcmp(argv[i],"-v") == 0)
            verbose = 1;
        else if(strcmp(argv[i], "-i") == 0)
            ignoreCase = 1;
    }
    f1 = fopen(argv[1], "r");
    f2 = fopen(argv[2], "r");
    
    if(f1 == NULL || f2 == NULL)
    {
        printf("Error with open file\n");
        fclose(f1);
        fclose(f2);
        return 1;
    }
    
    fseek(f1, 0, SEEK_SET);
    fseek(f2, 0, SEEK_SET);
    int f1p, f2p;
    if(ignoreCase == 1){
        f1p = lower(fgetc(f1));
        f2p = lower(fgetc(f2));
    }
    else{
        f1p = fgetc(f1);
        f2p = fgetc(f2);
    }
    while(f1p != EOF && f2p != EOF )
    {
        if(f1p != f2p)
        {
            if(verbose == 1)
                printf("distinct\n");
            fclose(f1);
            fclose(f2);
            return 1;
        }
        if(ignoreCase == 1){
            f1p = lower(fgetc(f1));
            f2p = lower(fgetc(f2));
        }
        else{
            f1p = fgetc(f1);
            f2p = fgetc(f2);
        }
    }
    if(!(f1p == EOF && f2p == EOF))
    {   
        if(verbose)
            printf("distinct\n");
        fclose(f1);
        fclose(f2);
        return 1;
    }
    if(verbose == 1)
        printf("equal\n");
    fclose(f1);
    fclose(f2);
    return 0;
}