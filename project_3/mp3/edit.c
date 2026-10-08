#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"proto.h"


uchar header[10];
uchar tagg[5]={0};
uint sizee=0;
uchar flagg[3];
uchar old_frame[100];
uchar new_frame[100]={'\0'};


uint big_endian=0;
uint lit_endian=0;

char *tagss[] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};//tags
char *cmd[] = {"-t","-a","-A","-y","-m","-c"}; //input in command line


int efile(char **str, int index)
{
    FILE *fr, *fw;

    fr = fopen("song.mp3","r"); //read mode
    if(fr==NULL)
    {
        printf("edit_rd not open\n");
        return 1;
    }

    fw= fopen("temp.mp3","w"); //write mode
    if(fw==NULL)
    {
        printf("edit_wr not open\n");
        return 1;
    }

    //copying header 10byte
    fread(header,1,10,fr);
    fwrite(header,1,10,fw);

    //before finded, which tag frame want to edit, that tag index position stored(in main() )
    int tag_indx= index;

    //if comared tag is matched then do this
        for(int i=0;i<6;i++)
        {

            //read tag from old_file
            fread(tagg,1,4,fr);

            //fetch tag
            if(strcmp(tagg,tagss[tag_indx])==0)
            {
                fwrite(tagg,1,4,fw);
                 //new data size
                strcpy(&new_frame[1],str[3]);
                unsigned long int ln=strlen(str[3])+1;//'+1' because strlen return size without includeing null 
            
                //coert L.E to B.E
                big_endian = convert(ln);
           
                //write B.E size to .mp3 file
                fwrite(&big_endian,1,4,fw); //fwrite(whre to read, how many byte, how many times, whre to store)
                
                //read size from old_file to move pointer 
                fread(&sizee,1,4,fr);
                lit_endian = convert(sizee);
              
                
                //read & write flags
                fread(flagg,1,2,fr);
                fwrite(flagg,1,2,fw);

                //new frame name write here
                fwrite(new_frame,1,ln,fw);

                fseek(fr,lit_endian,SEEK_CUR);

            }
            else
            {
                //not matched previous tag then write old tag itself
                fwrite(tagg,1,4,fw);
                
                //read size from old_file 
                fread(&sizee,1,4,fr);

                //convert B.E to L.E
                lit_endian = convert(sizee);

                //write B.E size to temp.mp3
                fwrite(&sizee,1,4,fw);
                
                //read & write flags
                fread(flagg,1,2,fr);
                fwrite(flagg,1,2,fw);

                //read L.E size data from song.mp3 // reading with '\0' also
                fread(old_frame,1,lit_endian,fr);

                //write L.E size data from temp.mp3
                fwrite(old_frame,1,lit_endian,fw);
            }
        }

    //remaining bytes reading and writing as it is.
    while((fread(old_frame,8,1,fr))!=0)//fread return how many bytes it read successfully so if value read 0 it stop
        fwrite(old_frame,8,1,fw);



    fclose(fr);
    fclose(fw);

    remove("song.mp3");//old file deleted
    rename("temp.mp3",str[4]);//for new file reaname as a old file name
    return 0;

}

