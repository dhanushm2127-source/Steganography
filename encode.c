#include <stdio.h>
#include "encode.h"
#include "types.h"

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

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
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
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

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

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *dot = strrchr(argv[2], '.');
    if(dot == NULL || strcmp(dot, ".bmp") != 0)
    {
        printf("Error extension must be '.bmp' only\n");
        return e_failure;
    }
    encInfo -> src_image_fname = argv[2]; 
    
    encInfo -> secret_fname = argv[3];

    //Check argv[4] == NULL
    if(argv[4] == NULL)
    {
        encInfo -> stego_image_fname = "output.bmp";
    }
    else
    {
        char *out = strrchr(argv[4], '.');
        //Validate last 4 character is ".bmp" or not
        if(out == NULL || strcmp(out, ".bmp") != 0)
        {
            printf("Error extension must be '.bmp' only\n");
            return e_failure;
        }
        encInfo -> stego_image_fname = argv[4];
    }
    // Call open_files(encInfo)
    if(open_files(encInfo) == e_failure)
    {
        printf("Error extension must be '.bmp' only\n");
        return e_failure;
    }
    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    //Open source file in read 'r' mode
    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname, "r");
    //Validate file is opening  or not
    if(encInfo -> fptr_src_image == NULL)
    {
        printf("Source file is not opened.\n");
        return e_failure;
    } 

    //Open secret file in read 'r' mode
    encInfo -> fptr_secret = fopen(encInfo -> secret_fname, "r");
    //Validate file is opening  or not
    if(encInfo -> fptr_secret == NULL)
    {
        printf("Secret file is not opened.\n");
        return e_failure;
    }

    //Open output file in read 'r' mode
    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "r");
    //Validate file is opening  or not
    if(encInfo -> fptr_stego_image == NULL)
    {
        printf("Output file is not opened.\n");
        return e_failure;
    }
    printf("File is opened successfully\n");
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    /*
        Call copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image) == e_failure
            print error msg, return e_failure

        Call encode_magic_string(const char *magic_string, EncodeInfo *encInfo) == e_failure
            print error msg, return e_failure
    */


    //Call check_capacity(encInfo) == e_failure
    if(encInfo -> image_capacity == e_failure)
    {
        printf("Error.... Insufficient image capacity\n");
        return e_failure;
    }
}

Status check_capacity(EncodeInfo *encInfo)
{
    //Get image capacity
    encInfo -> image_capacity = get_image_size_of_bmp(encInfo -> fptr_src_image);

    //Get secret file size
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    //Check ((14 + size_secret_file) * 8) > image_capacity
    if(((14 + encInfo -> size_secret_file) * 8) > encInfo -> image_capacity)
    {
        return e_failure;
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    //Move the offset to last pos
    fseek(fptr, 0, SEEK_END);

    //return ftell()
    printf("File size = %lu\n",ftell(fptr));
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
        -> Move the file pointer to the SEEK_SET
        -> Declare the buff[54]
        -> Read 54 bytes from src file
        -> Write 54 bytes to dest file

        -> return e_success
    */
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        Declare the buff of 8 bytes
        Read 8 byte from src_file into buff
        Encode_byte_to_lsb(magic_string[1], buff)
    */
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
    for(i = 7; i >= 0; i--)
    {
        -> Get the ith bit is set or not
            => If set, Set the LSB of image_buffer[]
            => if clear, Clear the LSB of image_buffer[] 
    }
    */
}

Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    /* 
    -> char *dot = strchr(secret_file_name, ".")
    -> strcpy(extn_secret_file,dot);


    -> Declare a buff[32]
    -> Read 32 bytes from src file into buff
    -> call encode_size_to_lsb(strlen(extn_secret_file), buff)

    */
}

Status encode_size_to_lsb(int size,char *Image_buff)
{
    /*
    for(int i=31;i>=0;i--)
    {
        -> get the ith bit is or not
          => if set ,set the LSB of image_buffer[]
          => else clear the LSB of image_buffer[] 

          return e_success
    */
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        Declare buffer[8]

        -> Read 8 bytes from src_image
        -> encode_byte_to_lsb(file_extn[], buff)
        -> Write the 8 bytes of buff to output_file

        return e_success
    */
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        -> Declare the buff[32]
        -> Read the 32 bytes from src_image
        -> Call encode_size_to_lsb(file_size, buff)
        -> Write the 32 bytes of buff to output_file

        return e_success
    */
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        Declare the buffer[8], data
        =>Loop till EOF of secret_file
            -> Read 8 bytes from src_file
            -> Read 1 byte from secret_file
            -> encode_byte_to_lsb(data, buff)
    */
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        Declare a char as data

        => Loop till EOF of src_file
            -> Read a char from src_file
            -> Write thebdata to dest_file


        return e_success
    */
}
