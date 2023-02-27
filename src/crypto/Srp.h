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

    char *HexPrivateKey();

    char *DecPublicKey();

    char *DecPrivateKey();

    BIGNUM* ClientSession(
        char* user,
        char* password,
        char* salt,
        char *client_publickey,
        char *server_publickey,
        char *client_privatekey
    );
};

#endif
