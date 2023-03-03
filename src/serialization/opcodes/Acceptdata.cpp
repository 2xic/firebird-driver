#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./Acceptdata.h"
#include "../../utils/Opcodes.h"
#include "../../utils/Config.h"
#include "../../utils/Utils.h"
#include "../../crypto/Srp.h"

AcceptData::AcceptData(){}

AcceptData::~AcceptData(){
    BN_free(this->client_session);
}

void AcceptData::decode(MessageDecoder *decoder)
{
    int protocol_version = decoder->readInt();
    protocol_version = (protocol_version & FB_PROTOCOL_MASK) | FB_PROTOCOL_FLAG;

    int protocol_architecture = decoder->readInt();
    int protocol_minimum_type = decoder->readInt() & 0xFF;

    int array_size = decoder->readInt();
    decoder->position -= 4;

    unsigned char *array = decoder->readArray();
    log("%i\n", decoder->position);
    char *pluginName = decoder->readString();
    int is_authenticated = decoder->readInt();

    char *keys = decoder->readString();

    log(
        "Protocol version %i\n", protocol_version);
    log(
        "Protocol architecture %i\n", protocol_architecture);
    log("Name == %s\n", pluginName);

    Srp *srp = new Srp();

    if (is_authenticated == 0 && strncmp(pluginName, "Srp", 3) == 0)
    {
        log("Supported plugin\n");
        // Srp uses sha1
        /*
            Need to use the keys.
        */
        MessageDecoder *message_keys = new MessageDecoder();
        message_keys->decode(array, array_size);

        short saltLength = message_keys->readShortLe();
        unsigned char *salt = message_keys->readBuffer(saltLength);
        short keyLength = message_keys->readShortLe();
        unsigned char* keys_data = message_keys->readBuffer(keyLength);

        BIGNUM *bn1 = BN_new();
        BN_hex2bn(&bn1, (char*)keys_data);

        char *public_key = srp->DecPublicKey();
        char *private_key = srp->DecPrivateKey();
        char *server_key = BN_bn2dec(bn1);

        this->client_session = srp->ClientProof(
            (char*)FIREBIRD_USER,
            (char*)FIREBIRD_PASSWORD,
            (char*)salt, 
            public_key,
            server_key,
            private_key
        );
        BN_free(bn1);

        free(server_key);
        free(public_key);
        free(private_key);

        free(salt);
        free(keys_data);
        delete message_keys;
    }
    else
    {
        log("Unsupported plugin, throw error");
    }

    free(array);
    free(pluginName);
    free(keys);

    delete srp;
}

char* AcceptData::data(){
    // Return client session
    return BN_bn2hex(this->client_session);
}
