#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"lexer.h"


static char *keywords[]={"int","float","char","double",
                      "short","long","extern","static",
                      "register","if","else","while",
                      "for","goto","break","return",
                      "sizeof","signed","unsigned","switch",
                      "case","restrict","struct","union",
                      "typedef","void","enum","do",
                      "default","continue","const","auto",
                      "volatile"};
                      
                      
static void identify_word(FILE *fptr, int *ch)
{
    char word[50];
    int i=0;

    while(isalnum(*ch) || *ch=='_')
    {
        word[i++]=*ch;
        *ch=fgetc(fptr);
    }

    word[i]='\0';

    if(*ch!=EOF)
        ungetc(*ch,fptr);

    int flag=0;

    for(int j=0;j<33;j++)
    {
        if(strcmp(word,keywords[j])==0)
        {
            flag=1;
            printf("%s -> Keyword\n",word);
            break;
        }
    }

    if(flag==0)
        printf("%s -> Identifier\n",word);
}

static void identify_number(FILE *fptr, int *ch)
{
    char constant[20];
    int i=0;
    int flag=0;

    while(isdigit(*ch) || *ch=='.')
    {
        if(*ch=='.')
        {
            if(flag==1)
                break;

            flag=1;
        }

        constant[i++]=*ch;
        *ch=fgetc(fptr);
    }

    constant[i]='\0';

    if(*ch!=EOF)
        ungetc(*ch,fptr);

    if(flag==1)
        printf("%s -> float Constant\n",constant);
    else
        printf("%s -> integer Constant\n",constant);
}
static void identify_operator(FILE *fptr, int *ch)
{
    char Operator[3];
    int next;
    int i=0;

    next=fgetc(fptr);

    if((*ch=='=' && next=='=') ||
       (*ch=='!' && next=='=') ||
       (*ch=='<' && next=='=') ||
       (*ch=='>' && next=='=') ||
       (*ch=='+' && next=='+') ||
       (*ch=='-' && next=='-') ||
       (*ch=='&' && next=='&') ||
       (*ch=='|' && next=='|') ||
       (*ch=='-' && next=='>'))
    {
        Operator[i++]=*ch;
        Operator[i++]=next;
        Operator[i]='\0';

        printf("%s -> Operator\n",Operator);
    }
    else
    {
        printf("%c -> Operator\n",*ch);

        if(next!=EOF)
            ungetc(next,fptr);
    }
}
static void skip_comment(FILE *fptr, int *ch)
{
    int next=fgetc(fptr);

    if(next=='/')
    {
        while(*ch!='\n' && *ch!=EOF)
        {
            *ch=fgetc(fptr);
        }
    }
    else if(next=='*')
    {
        while(*ch!=EOF)
        {
            if(*ch=='*')
            {
                next=fgetc(fptr);

                if(next=='/')
                    break;
            }

            *ch=fgetc(fptr);
        }
    }
    else
    {
        printf("%c -> Operator\n",*ch);

        if(next!=EOF)
            ungetc(next,fptr);
    }
}
static void identify_delimiter(int ch)
{
    printf("%c -> Delimiter\n",ch);
}
static void identify_character(FILE *fptr, int *ch)
{
    char character[4];
    int i;

    for(i=0;i<3;i++)
    {
        character[i]=*ch;
        *ch=fgetc(fptr);
    }

    character[i]='\0';

    printf("%s -> character constant\n",character);
}
static void identify_string(FILE *fptr, int *ch)
{
    char string[100];
    int i=0;

    string[i++]=*ch;
    *ch=fgetc(fptr);

    while(*ch!='"' && *ch!=EOF)
    {
        string[i++]=*ch;
        *ch=fgetc(fptr);
    }

    if(*ch=='"')
    {
        string[i++]=*ch;
        string[i]='\0';

        printf("%s -> string constant\n",string);
    }
}
void lexical_analysis(FILE *fptr)
{   
    int ch; 

    while((ch=fgetc(fptr))!=EOF)
    {
        if(isalpha(ch))//if it is alphabet we categorize them as keyword/identifier/string/character
        {
            identify_word(fptr,&ch);
        }
        else if(isdigit(ch))
        {
           identify_number(fptr,&ch);
            
        }
        else if(ch==' ' || ch =='\n' || ch=='\t')
        {
           continue;
        }
        else if(ch=='+' || ch=='-' || ch=='*' || ch=='%' ||
        ch=='&' || ch=='^' || ch=='|' || ch=='<' ||
        ch=='>' || ch=='?' || ch=='!' || ch=='=')
        {
            identify_operator(fptr,&ch);
        }
        else if(ch=='/')
        {
            skip_comment(fptr,&ch);
        }
        else if(ch=='(' || ch==')' || ch=='{' || ch=='}' ||
                ch=='.' || ch==',' || ch==';' ||
                ch=='[' || ch==']')
        {
            identify_delimiter(ch);
        }   
        else if(ch=='\'')
        {
            identify_character(fptr,&ch);   
        }
        else if(ch=='"')
        {
            identify_string(fptr,&ch);
        }
        printf("\n");
    }
}