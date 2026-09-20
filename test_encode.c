#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    if(check_operation_type(argv[1][1]) == e_encode)
    {
        read_and_validate_encode_args(argv, &encInfo);
        if(do_encoding(&encInfo) == e_failure)
        {
            printf("Error...Unable to encoding\n");
            return 0;
        }
        printf("Encoding is done successfully");
    }
    else
    {
        printf("Validation of encode is failed...\n");
        return 0;
    }
}
    
OperationType check_operation_type(char opt)
{
    if(opt == 'e')
    {
        return e_encode;
    }
    elseif(opt == 'd')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
    return 0;
}
