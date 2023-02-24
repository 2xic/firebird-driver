#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./Acceptdata.h"
#include "../../opcodes.h"

AcceptData::AcceptData()
{
}

void AcceptData::decode(MessageDecoder *decoder)
{
    int protocol_version = decoder->readInt();
    protocol_version = (protocol_version & FB_PROTOCOL_MASK) | FB_PROTOCOL_FLAG;

    int protocol_aritecture = decoder->readInt();
    int protocol_minimum_type = decoder->readInt() & 0xFF;

    unsigned char *array = decoder->readArray();
    printf("%i\n", decoder->position);
    char *pluginName = decoder->readString();
    int is_authenticated = decoder->readInt();

    char *keys = decoder->readString();

    printf(
        "Protocol version %i\n", protocol_version);
    printf(
        "Protocol aritecture %i\n", protocol_aritecture);
    printf("Name == %s\n", pluginName);

    if (is_authenticated == 0 && strncmp(pluginName, "Srp", 3) == 0)
    {
        printf("Supported plugin\n");
        // Srp uses sha1
        /*
            Need to use the keys.
        */
        MessageDecoder *keys = new MessageDecoder();
        // TODO: Should be output from the array on top
        keys->decode(array, 324);

        short saltLength = keys->readShortLe();
        keys->readBuffer(saltLength);
        short keyLength = keys->readShortLe();
        keys->readBuffer(keyLength);
        printf("%i %i\n", saltLength, keyLength);
    }
    else
    {
        printf("Unsupported plugin, throw error");
    }

    free(array);
    free(pluginName);

    printf("Helo!\n");
    exit(0);
}
