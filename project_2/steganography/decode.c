#include "decode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if(strstr(argv[2],".bmp") != NULL )
		decInfo->out_image_fname = argv[2];	
	else
		return e_failure;
	
	
	if(argv[3] == NULL)
	{
		printf("ERROR: please provide Output File name eg.'output or any name'...\n");
		decInfo->secret_fname = "decoded";
	}
	else
	{
		decInfo->secret_fname = argv[3];
	}
	
	decInfo->extn_secret_file[0] = '\0';
	return e_success;
}

Status open_output_image_file(DecodeInfo *decInfo)
{
	printf("OPERATION: Opening required files\n");
    decInfo->fptr_out_image = fopen(decInfo->out_image_fname, "rb"); 
    if (decInfo->fptr_out_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->out_image_fname);
    	return e_failure;
    }
	fseek(decInfo->fptr_out_image, 54, SEEK_SET);
	printf("STATUS: Opened '%s'\n",decInfo->out_image_fname);
	return e_success;
}

Status open_decoded_message_file(DecodeInfo* decInfo)
{
	decInfo->secret_file_concat_name = malloc(strlen(decInfo->secret_fname) + strlen(decInfo->extn_secret_file) + 1);
	if(decInfo->secret_file_concat_name == NULL)
		return e_failure;
		
	strcpy(decInfo->secret_file_concat_name, decInfo->secret_fname);
	strcat(decInfo->secret_file_concat_name, decInfo->extn_secret_file);

	decInfo->fptr_secret = fopen(decInfo->secret_file_concat_name, "wb");
	
	if (decInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_file_concat_name);
    	return e_failure;
    }
	printf("STATUS: Opened '%s'\n",decInfo->secret_file_concat_name);
	return e_success;
}
	
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
	printf("OPERATION: Decoding Magic String Signature\n");
	char buffer[8];
	char ch;
	char data[strlen(magic_string) + 1];
	
	for(int i=0; i< strlen(magic_string); i++)
	{
		if(fread(buffer, 8, 1, decInfo->fptr_out_image)!= 1) 
			return e_failure;
		decode_bit_from_lsb(&ch, buffer);
		data[i]= ch;
	}
	
	data[strlen(magic_string)]= '\0';
	printf("DEBUG: Expected '%s', Got '%s'\n", magic_string, data); 
	
	if(strcmp(magic_string, data)==0)
	{
		printf("STATUS: Done\n");
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

Status decode_bit_from_lsb(char* ch, char* image_buffer)
{
	*ch = 0;
	for(int i = 0; i < 8; i++)
	{
	*ch = *ch | ((image_buffer[7-i] & 1) << i); 
	}
	return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo* decInfo)
{
	char buffer[32];
	int size;
	if(fread(buffer, 32, 1, decInfo->fptr_out_image)!= 1) return e_failure;
	decode_int_from_lsb(&size, buffer);
	decInfo->extension_size= size;
	return e_success;
}

Status decode_int_from_lsb(int* size, char* image_buffer)
{
	*size = 0;
	for(int i = 0; i < 32; i++)
	{
		*size = *size | ((image_buffer[31-i] & 1) << i); 
	}
	return e_success;
}

Status decode_secret_file_extn(DecodeInfo* decInfo)
{
	printf("OPERATION: Decoding Output File Extension\n");
	char buffer[8];
	char ch;
	char data[decInfo->extension_size + 1];
	
	for(int i=0; i< decInfo->extension_size; i++)
	{
		if(fread(buffer, 8, 1, decInfo->fptr_out_image)!= 1) return e_failure;
		decode_bit_from_lsb(&ch, buffer);
		data[i]= ch;
	}
	data[decInfo->extension_size]= '\0';
	strcpy(decInfo->extn_secret_file,data);
	printf("STATUS: Done. Extension: %s\n", decInfo->extn_secret_file);
	return e_success;
}

Status decode_secret_file_size(DecodeInfo* decInfo)
{
	printf("OPERATION: Decoding '%s' File Size\n",decInfo->secret_file_concat_name);
	char buffer[32];
	int size;
	if(fread(buffer, 32, 1, decInfo->fptr_out_image)!= 1) return e_failure;
	decode_int_from_lsb(&size, buffer);
	decInfo->size_secret_file= size;
	printf("STATUS: Done. Size: %ld bytes\n", decInfo->size_secret_file);
	return e_success;
}

Status decode_secret_file_data(DecodeInfo* decInfo)
{
	printf("OPERATION: Decoding '%s' File data\n",decInfo->secret_file_concat_name);
	char buffer[8];
	char ch;
	
	for(int i=0; i< decInfo->size_secret_file; i++)
	{
		if(fread(buffer, 8, 1, decInfo->fptr_out_image)!= 1) return e_failure;
		decode_bit_from_lsb(&ch, buffer);
		fwrite(&ch, 1, 1, decInfo->fptr_secret);
	}
	printf("STATUS: Done\n");
	return e_success;
}

Status do_decoding(DecodeInfo* decInfo)
{
	printf("\n....Decoding Procedure Started.....\n\n");
	if(open_output_image_file(decInfo) == e_success)
	{
		if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
		{
			if(decode_secret_file_extn_size(decInfo) == e_success)
			{
				if(decode_secret_file_extn(decInfo) == e_success)
				{
					if(open_decoded_message_file(decInfo) == e_success)
					{
						if(decode_secret_file_size(decInfo) == e_success)
						{
							if(decode_secret_file_data(decInfo) == e_success)
							{
								printf("\n.....Decoding Done Successfully.....\n\n");
								fclose(decInfo->fptr_out_image);
								fclose(decInfo->fptr_secret);
								free(decInfo->secret_file_concat_name);
								return e_success;
							}
						}
					}
				}
			}
		}
	}
	printf("ERROR: Decoding failed\n");
	if(decInfo->fptr_out_image) fclose(decInfo->fptr_out_image);
	if(decInfo->fptr_secret) fclose(decInfo->fptr_secret);
	return e_failure;
}