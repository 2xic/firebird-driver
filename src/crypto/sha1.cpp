#include "sha1.h"
#include <openssl/sha.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

Sha1::Sha1()
{
    char output[80];
    SHA1_Init(&this->context);
}

void Sha1::update(unsigned char *data, int length)
{
    SHA1_Update(&context, data, length);
}

unsigned char* Sha1::raw_hash() {
    unsigned char *digest = (unsigned char*)malloc((sizeof(unsigned char) * 20));
    SHA1_Final(digest, &this->context);
    return digest;
}

char* Sha1::hash() {
    unsigned char* digest = this->raw_hash();
    char *results = (char*)malloc(sizeof(char) * (20 * 2 + 1));
    for(int i = 0; i < 20; i++) {
        sprintf(results + 2* i, "%02x", digest[i]);
    }
    free(digest);

    return results;
}
