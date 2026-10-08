#include<stdio.h>
#include<string.h>
#include"proto.h"
#include<stdlib.h>
#include"conv.h"



char tag[5]={0};
uint size=0;
char frame_tag[100]; //initial with 0 its equal to NULL //we stored char after it have null, consider as string 

//Convert Big endian to little endian
uint little_endian=0;

char *tags[] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};
char *names[] = {"Title","Artist","Album","Year","content_type","Composer"};


int vfile()
{
    FILE *fp;

    fp=fopen("song.mp3","r");//file open in read mode because only read and print o/p
    if(fp==NULL)
    {
        printf("Error: not exist file name\n");
        return 1;//return 1 to terminate program, & telling to OS, program was unsuccefull or failed
    }

//skip 10bytes for header
    fseek(fp,10,SEEK_SET); 

    //totally 6 tags so loop 6 times
    for(int i=0;i<6;i++)
    {
        //reading 4byte of tag
        fread(tag,1,4,fp);// fread(whre to store, how many byte, how many times, whre to read)
        
    //read 4bytes
        fread(&size,1,4,fp);//size in mp3 file big endian format so, to covert to L.E

    //Convert Big endian to little endian
        little_endian = convert(size);

    //skip 3 bytes of flag, 2 have empty space 1 have '\0' it store like 1st null nxt framename thats why we skip 3 bytes
        fseek(fp,3,SEEK_CUR);

    //reading_frame_data with size  of we converted L.E (size -1), siz -1 ->because it count incude null so , 
        fread(frame_tag,1,(little_endian-1),fp);
        frame_tag[little_endian -1]='\0';

    // compering tags and prining if it matches only
        if((strcmp(tag,tags[i]))==0){
            printf("%-13s: ",names[i]);
            printf("%s\n",frame_tag);
        }
    }
    
    return 0;
}

  

