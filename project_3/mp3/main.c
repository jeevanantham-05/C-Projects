/*
    I. JEEVANANTHAM
    26010_108
    MP3 Tag reader
*/

#include<stdio.h>
#include<string.h>
#include"proto.h"

char *tg[] = {"-t","-a","-A","-y","-m","-c"};
int flag=0;

int main(int count, char *str[])
{
    if(count<3 || (strcmp(str[1],"-h")==0)) 
    {
        help();
        return 1;
    }

//validate command line
    if(strcmp(str[1],"-v")==0)  //view part
    {
        if(strstr(str[2],".mp3")!=NULL)
        {
            vfile();//view file open, call func
        }
        else printf("Error: file exten must \".mp3\"\n");
    }
    else if(strcmp(str[1],"-e")==0)  //edit part, 
    {
        for(int i=0;i<6;i++)
        {
            if(strcmp(str[2],tg[i])==0) //validating while user giving correct keyword cmd or not
            {
                efile(str,i);//passing main arug vector as well as finded index pos
                flag=1;//to know user entered crt input only
                break; // call func to edit in inside file
            }
        } 
        if(flag!=1)
        printf("Error: file tag must: \"-t\\-a\\-A\\-y\\-m\\-c \" \n");//wrng input user given time it shows only this input to enter
    }
    else printf("Error: first arug must \"-v or -e\"\n");

    return 0;
}

//helping to user 
void help()
{
    printf("HELP ! :\n./a.out -v sample.mp3\n");//helping to userr to know the command
}