#include <cheat.h>
#include "Sha1.h"
#include "../utils/Utils.h"

CHEAT_TEST(sha1_encoding,
    Sha1 *message = new Sha1();
    message->update((unsigned char*)&"hello", 5);
    char *message_hash = message->hash();
    cheat_assert(strncmp(
        message_hash,
        "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d",
        40
    ) == 0);

    free(message_hash);

    Sha1 *double_message = new Sha1();
    double_message->update((unsigned char*)&"test", 4);
    double_message->update((unsigned char*)&"hello", 5);
    char *double_message_hash = double_message->hash();
    cheat_assert(strncmp(
        double_message_hash,
        "6d248f78817c2fa03e3046043989d5f078586585",
        40
    ) == 0);
    free(double_message_hash);
)
