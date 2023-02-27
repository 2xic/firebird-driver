#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./Acceptdata.h"
#include "../../utils/Opcodes.h"
#include "../../utils/Config.h"
#include "../../utils/Utils.h"
#include "../../crypto/Srp.h"

AcceptData::AcceptData(){}

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
        MessageDecoder *keys = new MessageDecoder();
        // TODO: Should be output from the array on top
        keys->decode(array, array_size);

        short saltLength = keys->readShortLe();
        unsigned char *salt = keys->readBuffer(saltLength);
        short keyLength = keys->readShortLe();
        unsigned char* keys_data = keys->readBuffer(keyLength);
        printf("%i %i\n", saltLength, keyLength);

  //      printf("salt == %s \n", getAsHex(salt, saltLength));
        printf("salt == %s \n", salt);
        printf("server_keys == %s \n", keys_data);
        printf("pub == %s\n", srp->DecPublicKey());
        printf("priv == %s\n", srp->DecPrivateKey());

        BIGNUM *bn1 = BN_new();
        BN_hex2bn(&bn1, (char*)keys_data);

        printf("keee === %s\n", BN_bn2dec(bn1));

//        HexStringToAscii((char*)keys_data, 256);

        this->client_session = srp->ClientSession(
            (char*)FIREBIRD_USER,
            (char*)FIREBIRD_PASSWORD,
            (char*)salt, //getAsHex(salt, saltLength),
            srp->DecPublicKey(),
            BN_bn2dec(bn1),
//            srp->HexPrivateKey(),
            srp->DecPrivateKey()
        );
    }
    else
    {
        log("Unsupported plugin, throw error");
    }

    free(array);
    free(pluginName);

    log("Hello!\n");
}

char* AcceptData::data(){
    // Return client session
    return BN_bn2hex(this->client_session);
}
