#ifndef Sha1_test_functions_H
#define Sha1_test_functions_H

#include "Sha1.h"
#include "../utils/Utils.h"

char * sha1_test_encoding()  {
    Sha1 *message = new Sha1();
    message->update((unsigned char*)&"hello", 5);
    char *message_hash = message->hash();

    delete message;
    
    return message_hash;
}

void profile_sha1() {
    char *hash = sha1_test_encoding();
    free(hash);
}

#endif
