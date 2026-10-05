#include<stdio.h>
#include"lexer.h"

int main(int argc,char *argv[])
{
    FILE *fptr;
    
    if(argc != 2)
    {
        printf("Usage: %s <filename>\n",argv[0]);
        return 1;
    }

    fptr=fopen(argv[1],"rb");

    if(fptr==NULL)
    {
        printf("Error : Unable to open file\n");
        return 1;
    }

    lexical_analysis(fptr);

    fclose(fptr);

    return 0;
}