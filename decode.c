#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
    for(int i = 2; i < 3; i++)
    {
        if(argv[i] == NULL)
        {
            printf("Invalid input\n");
            return e_failure;
        }
    }
    char *dot = strrchr(argv[2], '.');
    if(dot == NULL || strcmp(dot, ".bmp") != 0)
    {
        printf("Error extension must be '.bmp' only\n");
        return e_failure;
    }
    encInfo -> stego_image_fname = argv[2]; 
    
    //Check argv[3] == NULL
    if(argv[3] == NULL)
    {
        encInfo -> stego_image_fname = "decoded_op.bmp";
    }
    else
    {
        char output[20];
        //Collect upto th '.' character
        sscanf(argv[3], "[^.]", output);
        //Concatenate output[] and ".txt"
        strcat(output, ".txt");
        encInfo -> stego_image_fname = output;
    }
    // Call open_files(encInfo)
    if(open_decoded_files(encInfo) == e_failure)
    {
        printf("Error extension must be '.bmp' onlFile doesnot opened\n");
        return e_failure;
    }
    return e_success;
}

Status do_decoding(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_src_image,54,SEEK_SET);
    printf("\n%lu\n",ftell(encInfo->fptr_src_image));

}

Status open_decoded_files(EncodeInfo *encInfo)
{
    //Open output file in read 'r' mode
    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname, "r");
    //Validate file is opening  or not
    if(encInfo -> fptr_src_image == NULL)
    {
        printf("Output file is not opened.\n");
        return e_failure;
    }
    printf("File is opened successfully\n");

    //Open decoded output file in read 'r' mode
    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "w");
    //Validate file is opening  or not
    if(encInfo -> fptr_stego_image == NULL)
    {
        printf("Source file is not opened.\n");
        return e_failure;
    } 
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;
    for(int i = 0; i < 8; i++)
    {
        *data = *data << 1;
        //Get the ith bit is set or not
        if(image_buffer[i] & 1)
        {
            //If set, Set the LSB of image_buffer[]
            *data = *data | 1; 
        }
    }
    return e_success;
}
