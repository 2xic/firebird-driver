#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <ctype.h>

#ifndef SRP_H
#define SRP_H

class Srp
{
BIGNUM *publickey;
BIGNUM *privatekey;

public:
    Srp();

    char *HexPublicKey();
};

#endif
