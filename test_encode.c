#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if(argc < 2)
    {
        printf("Invalid input arguments....\n");
        return 0;
    }
    if(check_operation_type(argv[1][1]) == e_encode)
    {
        if(argc < 4)
        {
            printf("Encoding : Please enter ./a.out -e beautiful.bmp secret.txt\n");
            return 0;
        }
        if(read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            printf("Invalid input arguments....\n");
            return 0;
        }
        if(do_encoding(&encInfo) == e_failure)
        {
            printf("Error...Unable to encoding\n");
            return 0;
        }
        printf("Encoding is done successfully\n");
        return 0;
    }
    else if(check_operation_type(argv[1][1]) == e_decode)
    {
        if(argc < 3)
        {
            printf("Decoding: Please enter ./a.out -d output.bmp\n");
            return 0;
        }
        if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
        {
            printf("Invalid input arguments....\n");
            return 0;
        }
        if(do_decoding(&decInfo) == e_failure)
        {
            printf("Error...Unable to decoding\n");
            return 0;
        }
        printf("Decoding is done successfully\n");
        return 0;
    }
    else
    {
        printf("Invalid operation...\n");
        return 0;
    }
    return 0;
}
    
OperationType check_operation_type(char opt)
{
    if(opt == 'e')
    {
        return e_encode;
    }
    else if(opt == 'd')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}