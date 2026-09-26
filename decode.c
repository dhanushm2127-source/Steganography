#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
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
    decInfo -> src_image_fname = argv[2]; 
    
    //Check argv[3] == NULL
    if(argv[3] == NULL)
    {
        decInfo -> stego_image_fname = "decoded_op.txt";
    }
    else
    {
        char output[20];
        //Collect upto th '.' character
        sscanf(argv[3], "%[^.]", output);
        //Concatenate output[] and ".txt"
        strcat(output, ".txt");
        decInfo -> stego_image_fname = output;
    }
    // Call open_files(encInfo)
    if(open_decoded_files(decInfo) == e_failure)
    {
        printf("Error extension must be '.txt' so File doesnot opened\n");
        return e_failure;
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image,54,SEEK_SET);
    //printf("%lu\n",ftell(decInfo->fptr_src_image));

    //Call decode_magic_string
    if(decode_magic_string(MAGIC_STRING, decInfo) == e_failure)
    {
        printf("Error.... Unable to decode the magic string\n");
        return e_failure;
    }

    //Call decode secret file extention size
    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Error.... Unable to decode the secret file extention size\n");
        return e_failure;
    }

    //Call decode secret file extension
    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("Error.... Unable to decode the secret file extention\n");
        return e_failure;
    }

    //Call decode secret file size
    if(decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Error.... Unable to decode secret file size\n");
        return e_failure;
    }

    //Call decode secret file data
    if(decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Error.... Unable to decode the secret file data\n");
        return e_failure;
    }

    return e_success;

}

Status open_decoded_files(DecodeInfo *decInfo)
{
    //Open output file in read 'r' mode
    decInfo -> fptr_src_image = fopen(decInfo -> src_image_fname, "r");
    //Validate file is opening  or not
    if(decInfo -> fptr_src_image == NULL)
    {
        printf("Output file is not opened.\n");
        return e_failure;
    }

    //Open decoded output file in read 'r' mode
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "w");
    //Validate file is opening  or not
    if(decInfo -> fptr_stego_image == NULL)
    {
        printf("Source file is not opened.\n");
        return e_failure;
    } 
    printf("Decoded output file is opened successfully\n");
    return e_success;
}

Status decode_byte_from_lsb(unsigned char *data, char *image_buffer)
{
    if(image_buffer == NULL)
    {
        return e_failure;
    }

    *data = 0;
    int j = 7;
    for(int i = 0; i < 8; i++)
    {
        //Get the ith bit is set or not
        if(image_buffer[i] & 1)
        {
            //If set, Set the LSB of image_buffer[]
            *data = (*data) | (1 << j); 
        }
        j--;
    }
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char decode_magic_buff[3];
    char buff[8];
    for(int i = 0; magic_string[i] != '\0'; i++)
    {
        char data = 0;
        if(fread(buff, 8, 1, decInfo -> fptr_src_image) != 1)
        {
            printf("Error.... Failed to read data\n");
            return e_failure;
        }
        if(decode_byte_from_lsb(&data, buff) == e_failure)
        {
            return e_failure;
        }
        decode_magic_buff[i] = data;
    }
    decode_magic_buff[2] = '\0'; 
    //Compare magic string with decode magic string
    if(strcmp(magic_string, decode_magic_buff) != 0)
    {
        printf("Magic string doesnot match with decode magic string.\n");
        return e_failure;
    }
    return e_success;
}

Status decode_size_from_lsb(int *data, char *image_buffer)
{
    if(image_buffer == NULL)
    {
        return e_failure;
    }

    int j = 31;
    *data = 0;
    for(int i = 0; i < 32; i++)
    {
        //Get the ith bit is set or not
        if(image_buffer[i] & 1)
        {
            *data = (*data) | (1 << j);
        }
        j--;
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    //Declare a buff[32]
    char buff[32];
    int extn_size = 0;
    //Read 32 bytes from src file into buff
    if(fread(buff, 32, 1, decInfo -> fptr_src_image) != 1)
    {
        printf("Error.... Failed to read image data\n");
        return e_failure;
    }
    //call decode_size_to_lsb(strlen(extn_secret_file), buff)
    if(decode_size_from_lsb(&extn_size, buff) == e_failure)
    {
        return e_failure;
    }
    if(extn_size != 4)
    {
        printf("Secret file extension doesnot match\n");
        return e_failure;
    }
    printf("Secret file extension size is decoded successfully\n");
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    //Declare buffer[8]
    char buff[8];
    char extn[4];
    for(int i =0; i < 4; i++)
    {
        char data = 0;
        //Read 8 bytes from src_image
        if(fread(buff, 8, 1, decInfo -> fptr_src_image) != 1)
        {
            printf("Error.... Failed to read image data\n");
            return e_failure;
        }
        if(decode_byte_from_lsb(&data, buff) == e_failure)
        {
            return e_failure;
        }
        extn[i] = data;
    }
    extn[4] = '\0';
    //printf("%s\n",extn);
    if(strcmp(extn, ".txt") != 0)
    {
        printf("Secret filr extension doesnot match\n");
        return e_failure;
    }
    printf("Secret file extension is decoded successfully\n");
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    //Declare the buff[32]
    char buff[32];
    int data = 0;
    //Read the 32 bytes from src_image
    if(fread(buff, 32, 1, decInfo -> fptr_src_image) != 1)
    {
        printf("Error.... Failed to read image data\n");
        return e_failure;
    }
    if(decode_size_from_lsb(&data, buff) == e_failure)
    {
        return e_failure;
    }
    decInfo -> size_secret_file = data;
    printf("Secret file size decoded successfully.\n");
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    //Declare the buffer[8], data
    char buffer[8];
    //Declare the output[50]
    char output[50];
    char data;
    int i = 0;
    for(int i = 0; i < decInfo -> size_secret_file; i++)
    {
        //Read 8 bytes from src_file
        if(fread(buffer, 8, 1, decInfo -> fptr_src_image) != 1)
        {
            printf("Error.... Failed to read image data\n");
            return e_failure;
        }
        //Read 1 byte from secret_file
        if(decode_byte_from_lsb(&data, buffer) == e_failure)
        {
            return e_failure;
        }
        output[i] = data;
        //write 8 bytes to output file
        if(fwrite(&data, 1, 1, decInfo -> fptr_stego_image) != 1)
        {
            printf("Error.... Failed to write image data\n");
            return e_failure;
        }
    }
    output[i+1] = '\0';
    printf("Secret file data is decoded successfully\n");
    return e_success;
}