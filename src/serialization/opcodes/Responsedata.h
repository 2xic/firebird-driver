#ifndef ResponseData_H
#define ResponseData_H
#include "../MessageDecoder.h"
#include "./Response.h"
#include <openssl/bn.h>

class ResponseData: public Response
{
public:
    bool is_error = false;

    int database_handle = 0;

    ResponseData(MessageDecoder *decoder);

    char * data();
};

#endif
