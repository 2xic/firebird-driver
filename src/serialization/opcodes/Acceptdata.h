#ifndef AcceptData_H
#define AcceptData_H
#include "../MessageDecoder.h"
#include "./Response.h"
#include <openssl/bn.h>

class AcceptData: public Response
{
    BIGNUM *client_session;

public:
    AcceptData();

    ~AcceptData();

    void decode(MessageDecoder *decoder);

    char * data();
};

#endif
