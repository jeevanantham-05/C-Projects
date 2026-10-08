#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    // Find the size of secret file data
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // step 1 -> check argv[2] having .bmp
    if(strstr(argv[2],".bmp") != NULL){
        // yes -> encInfo -> src_image_fname = argv[2], goto next step;
        encInfo->src_image_fname = argv[2];
    }
    else return e_failure; // no -> return e_failure;

    // step 2 -> check argv[3] is having extn or not
    if(strstr(argv[3],".txt") != NULL || strstr(argv[3],".csv") !=NULL){

        // yes -> encInfo -> sec_fname = argv[3]; , extn_secret_file and 
        encInfo->sec_fname = argv[3];
        //store secret file extn
        char *extn = strstr(argv[3],".");
        strcpy(encInfo->extn_secret_file,extn);
    }
    else return e_failure;  // no -> return e_failure
        
    
    // step 3 -> check argv[4] != NULL or not
    if(argv[4]!=NULL)
    {
         // yes -> check argv[4] having .bmp`
         if(strstr(argv[4],".bmp")!=NULL)
         {
            // yes -> encInfo -> stego_image_fname = argv[4];
            encInfo -> stego_image_fname = argv[4];

         }
         else return e_failure;      // no -> return e_failure;
    }
    else{
         // no -> encInfo -> stego_image_fname = "stego.bmp"
        encInfo -> stego_image_fname = "stego.bmp";
    }

    return e_success;

}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->sec_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->sec_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // step 1 -> encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    
    // step 2 -> encInfo -> size_secret_file = call get_file_size(encInfo -> fptr_secret);

    //step 3 -> check encInfo -> image_capacity > 16 + 32 + (strlen(encInfo -> extn_secret_file) * 8) + 32 + (encInfo -> size_secret_file * 8)
        // yes -> return e_success;
        // no -> return e_failure;

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    uint required_size = 16 + 32 + (strlen(encInfo->extn_secret_file) * 8) + 32 + (encInfo->size_secret_file * 8);
    return (encInfo->image_capacity > required_size)? e_success : e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // char buffer[54];
    // step 1 -> rewing fptr_src_image to 0th pos
    // step 2 -> read 54 bytes from src file pointer
    // step 3 -> write 45 bytes to dest file
    // return e_success;
    rewind(fptr_src_image);
    char header[54];
    fread(header, 1, 54, fptr_src_image);
    fwrite(header, 1, 54, fptr_dest_image);
    return e_success;

}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // char buffer[8];

    // step 1 -> read 8 bytes from src file
    // step 2 -> encode_byte_to_lsb(magic_string[i], buffer);
    // step 3 -> write 8 bytes to dest file
    // repeat this strlen(magic_string) from step 1
    for(uint i = 0; i < strlen(magic_string); i++)
    {
        char buffer[8];
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    // char buffer[32];
    // step 1 -> read 32 bytes from src file
    // step 2 -> encode_size_to_lsb(size, buffer);
    // step 3 -> write 8 bytes to dest file
    char buffer[32];
    fread(buffer, 1, 32, encInfo->fptr_src_image);
    encode_size_to_lsb(size, buffer);
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    // char buffer[8];

    // step 1 -> read 8 bytes from src file
    // step 2 -> encode_byte_to_lsb(file_extn[i], buffer);
    // step 3 -> write 8 bytes to dest file
    // repeat this strlen(file_extn) from step 1
    for(uint i = 0; i < strlen(file_extn); i++)
    {
        char buffer[8];
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i], buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
   // char buffer[32];
    // step 1 -> read 32 bytes from src file
    // step 2 -> encode_size_to_lsb(file_size, buffer);
    // step 3 -> write 8 bytes to dest file
    char buffer[32];
    fread(buffer, 1, 32, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buffer);
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //store secret data from file to member

    // char buffer[8];

    // step 1 -> read 8 bytes from src file
    // step 2 -> encode_byte_to_lsb(encInfo -> secret_data[i], buffer);
    // step 3 -> write 8 bytes to dest file
    // repeat this encInfo -> size_secret_file from step 1
    char ch;
    for(long i = 0; i < encInfo->size_secret_file; i++)
    {
        fread(&ch, 1, 1, encInfo->fptr_secret);
        char buffer[8];
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(ch, buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // write the logic to copy the data
    char ch;
    while(fread(&ch, 1, 1, fptr_src) > 0)
        fwrite(&ch, 1, 1, fptr_dest);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
   //    write logic to encode data in lsb
    for(int i = 7; i >= 0; i--)
    {
        char bit = (data >> i) & 1;
        image_buffer[7-i] = (image_buffer[7-i] & 0xFE) | bit;
    }
    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)
{
   //write logic to encode data in lsb

   for(int i = 31; i >= 0; i--)
    {
        char bit = (size >> i) & 1;
        imageBuffer[31-i] = (imageBuffer[31-i] & 0xFE) | bit;
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
 // step 1 -> check open_files(encInfo) is returning e_success or not
    if(open_files(encInfo)!= e_success)
    {
        printf("Error: open_files\n");return e_failure;
    } 
    
    printf("Success Files opened\n");  // yes -> print success msg and go to next step
            
// step 2 -> check check_capacity(encInfo) is returning e_success or not
    if(check_capacity(encInfo)!= e_success)
    {   printf("Error: Capacity not have\n");
        return e_failure; 
    } 
    printf("Success Capacity ok\n");  

// step 3 -> check copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) success or not
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)!= e_success)
    {   printf("Error: Copy_bmp_header\n");
        return e_failure; 
    }  
    printf("Success copy_bmp_header\n");// yes -> print success msg goto next step


// step 4 -> call encode_magic_string(MAGIC_STRING, encInfo)
    if(encode_magic_string(MAGIC_STRING, encInfo)!= e_success)
     return e_failure;

    printf("Success encode_magic string\n");

// step 5 -> call encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo)!= e_success) return e_failure;
    printf("Success encode_secret_file_extn_size\n");

// step 6 -> call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)!= e_success) return e_failure;
    printf("Success encode_secret_file_extn\n");

// step 7 -> call encode_secret_file_size(encInfo -> size_secret_file, encInfo)
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo)!= e_success) return e_failure;
    printf("Success encode_secret_file_size\n");

// step 8 -> encode_secret_file_data(encInfo);
    if(encode_secret_file_data(encInfo)!= e_success) return e_failure;
    printf("Success encode_secret_file_data\n");

 // step 9 -> copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image)!= e_success) return e_failure;
    printf("\n.....SUCCESS Encoding Done.....\n");

    return e_success;

}
