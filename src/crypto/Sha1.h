#ifndef Sha1_H
#define Sha1_H
#include <openssl/sha.h>

class Sha1
{
SHA_CTX context;

public:
    Sha1();
    
    ~Sha1();

    void update(unsigned char *data, int length);

    unsigned char* raw_hash();

    char* hash();
};

#endif
