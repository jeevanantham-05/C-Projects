/*
	I. JEEVANANTHAM
	26010_108
	STEGANOGRAPHY PROJECT*/


#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

/* Check operation type */
OperationType check_operation_type(char *argv) // decide whether encoding or decoding 
{
	if(strcmp(argv, "-e") == 0)
	{
		return e_encode;
	}
	else if(strcmp(argv, "-d") == 0)
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}

int main(int argc, char* argv[])
{
	if(argc <= 3)
	{
		printf("ERROR: please provide operation (-e or -d),\nsource image (beautiful.bmp), secret file (secret.txt),\nOptional -> destination img file name(new.bmp)\n");
	}
	else
	{
		printf("\nOPERATION: Checking operation type...\n");
		if(check_operation_type(argv[1])== e_encode)
		{
			printf("STATUS: selected encoding\n");
			
			EncodeInfo encInfo;		//declaring the variable name for the struct datatype EncodeInfo which is located in encode.h
				
			printf("OPERATION: Calling read & validate function\n");
			if(read_and_validate_encode_args(argv, &encInfo) == e_failure)//call the read and validate encode arguments function
			{
				printf("ERROR: Read & validate function failed\n");
				return 0;
			}
			else
			{
				//call the next function do_encoding function
				printf("STATUS: Successfull, Calling do encoding function\n");
				do_encoding(&encInfo);
			}
		}
		else if(check_operation_type(argv[1])== e_decode)
		{
			printf("STATUS: Selected decoding\n");
			
			DecodeInfo decInfo;
			//call the read and validate decode arguments function
			//we are passing firstly -d (for decoding) and source file name(output image)
			printf("OPERATION: Calling read & validate function\n");
			if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
			{
				printf("ERROR: Read and validate function failed\n");
				return 0;
			}
			else
			{
				//call the next function do_decoding function
				printf("STATUS: Successfull, Calling do decoding function\n");
				do_decoding(&decInfo);
			}
		}
		else
		{
			printf("ERROR: Invalid operation! mention -e(encoding) or -d(decoding)\n");
			return 0;
		}
	}
}


