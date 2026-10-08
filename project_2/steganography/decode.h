#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Output Image info */
    char* out_image_fname;		//storing the beautiful.bmp file name
    FILE* fptr_out_image;		//opening the source file in read mode and the file pointer is stored in fptr 
    uint image_capacity;		//size of the source file storing in the image capacity variable
    //uint bits_per_pixel;		
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
	char* secret_file_concat_name;
    char* secret_fname;			//storing the secret file name
    FILE* fptr_secret;			//storing the file pointer address in fptr and we open it in read mode
    char extn_secret_file[5];			//storing the extension (.txt) in this array of size 4 and it is the maximum size for all the extension types.
    char secret_data[100];			//collecting the content from secret.txt and storing it in secret_array of size 1, because reading one character at a time and decoding it and we do it in a loop to decode all the characters. 
    long size_secret_file;			//storing the secret file size, bcox it is used in the decoding process.
	int extension_size;

    /* Stego Image Info */
    char* stego_image_fname;		//this is for storing the destination file name 
    FILE* fptr_stego_image;			//storing the file pointer address in fpr_stego and open it in write mode.

} DecodeInfo;


/* decoding function prototype */


/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char* argv[], DecodeInfo* decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo* decInfo);

/* Get File pointers for i/p and o/p files */
Status open_output_image_file(DecodeInfo* decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decoded_message_file(DecodeInfo* decInfo);

/* Store Magic String */
Status decode_magic_string(const char* magic_string, DecodeInfo* decInfo);

Status decode_secret_file_extn_size(DecodeInfo* decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo* decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo* decInfo);

/* decode secret file data*/
Status decode_secret_file_data(DecodeInfo* decInfo);

/* decode function, which does the real decoding */
Status decode_int_from_lsb(int* size, char* image_buffer);		//this is to store the 32 bytes of data

/* decode a byte into LSB of image data array */
Status decode_bit_from_lsb(char* ch, char* image_buffer);		//using this function to do the decoding process by taking the data character by character so 8 bytes


#endif
