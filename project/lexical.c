#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


int main(int argc,char *argv[])
{
    FILE *fptr=fopen(argv[1],"rb");

    if(fptr==NULL)
    {
        printf("Error : Unable to open file\n");
        return 1;
    }

    fseek(fptr,0,SEEK_SET);
    int ch; 

    char constant[20]; //collecting nums
    char word[50];  //keyword/identifier/string/character
    char Operator[3];//operators =,>=

    char *keywords[]={"int","float","char","double",
                      "short","long","extern","static",
                      "register","if","else","while",
                      "for","goto","break","return",
                      "sizeof","signed","unsigned","switch",
                      "case","restrict","struct","union",
                      "typedef","void","enum","do",
                      "default","continue","const","auto",
                      "volatile"};
    
    while((ch=fgetc(fptr))!=EOF)
    {
        if(isalpha(ch))//if it is alphabet we categorize them as keyword/identifier/string/character
        {
            int i=0;
            while(isalnum(ch) || ch=='_') //sum_123/sum123/sum
            {
                word[i++]=ch;
                ch=fgetc(fptr);

            }
            word[i]='\0';
            if(ch!=EOF)
            {
                ungetc(ch,fptr);//remove space/nullchar/operator/deliminator/other than alpha/alphanum
            }

            int flag=0;
            for(int j=0;j<33;j++)
            {
                if(strcmp(word,keywords[j])==0)//comparing them with the keywords
                {
                    flag=1;
                    printf("%s -> Keyword\n",word);
                    break;
                }
            }
            if(flag==0)//if no then it is a identifier
            {
                printf("%s -> Identifier\n",word);
               
            }
            
        }
        else if(isdigit(ch))
        {
            int i=0;
            int flag=0;
            while(isdigit(ch) || ch=='.')
            { 
                if(ch=='.')
                {
                    if(flag==1) break;
                    flag=1;
                }
                constant[i++]=ch;
                ch=fgetc(fptr);
            }
            constant[i]='\0';
            if(ch!=EOF)
            {
                ungetc(ch,fptr);
            }

            if(flag==1) printf("%s -> float Constant\n",constant);
            else  printf("%s -> integer Constant\n",constant);

            
        }
        else if(ch==' ' || ch =='\n' || ch=='\t')
        {
           continue;
        }
        else if(ch=='+' || ch=='-' || ch=='*' || ch=='%' || ch=='&' || ch=='^' || ch=='|' || ch=='<' || ch=='>' || ch=='?' || ch=='!' || ch=='=')
        {
            int next=fgetc(fptr);
            int i=0;
            if((ch=='=' && next=='=') || (ch=='!' && next=='=')||
                 (ch=='<' && next=='=') || (ch=='>' && next=='=') ||
                  (ch=='+' && next=='+') || (ch=='-' && next=='-') || 
                  (ch=='&' && next=='&') || (ch=='|' && next=='|') ||
                   (ch=='-' && next=='>'))
            {
                Operator[i++]=ch;
                Operator[i++]=next;
                Operator[i]='\0';
                printf("%s -> Operator\n",Operator);
            }
            else 
            {
                printf("%c -> Operator\n",ch);
                if(next!=EOF) ungetc(next,fptr);
            }
        }
        else if(ch=='/')
        {
            int next=fgetc(fptr);

            if( next=='/')
            {
                while(ch!='\n' && ch!=EOF)
                {
                    ch=fgetc(fptr);
                }
                
            }
            else if(next=='*')
            {
                while(ch!=EOF)
                {
                    if(ch=='*')
                    {
                        next=fgetc(fptr);
                        if(next=='/') break;
                    }
                    ch=fgetc(fptr);
                }
            }
            else 
            {
                printf("%c -> Operator\n",ch);
                if(next!=EOF) ungetc(next,fptr);
            }
        }
        else if(ch=='(' || ch==')' || ch=='{' || ch=='}' || ch=='.' || ch==',' || ch==';'||ch=='['||ch==']')
        {
            printf("%c -> Delimiter\n",ch);
        }
        else if(ch=='\'')
        {
            char character[4];

            int i;
            for(i=0;i<3;i++)
            {
              character[i]=ch;
              ch=fgetc(fptr);
            }

            character[i]='\0';
            printf("%s -> character constant\n",character);
        }
        else if(ch=='"')
        {
            char string[100];
            int i=0;

            string[i++]=ch;
            ch=fgetc(fptr);

            while(ch!='"' && ch!=EOF)
            {
                string[i++]=ch;
                ch=fgetc(fptr);
            }

            if(ch=='"')
            {
                string[i++]=ch;
                string[i]='\0';
                printf("%s -> string constant\n",string);
            }
        }
       

    }
    printf("\n");

    fclose(fptr);
}