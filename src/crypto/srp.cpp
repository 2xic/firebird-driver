
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include "srp.h"
#include "../config.h"

// https://github.com/nakagami/firebirdsql/blob/master/srp.go
const char G[] = "02";
const char N[] = "E67D2E994B2F900C3F41F08F5BB2627ED0D49EE1FE767A52EFCD565CD6E768812C3E1E9CE8F0A8BEA6CB13CD29DDEBF7A96D4A93B55D488DF099A15C89DCB0640738EB2CBDD9A8F7BAB561AB1B0DC1C6CDABF303264A08D1BCA932D1F1EE428B619D970F342ABA9A65793B8B2F041AE5364350C16F735F56ECBCA87BD57B29E7";
const char K[] = "1277432915985975349439481660349303019122249719989";
const int PRIVATE_KEY_SIZE = 128;

BIGNUM *randomBigNum(int length);
BIGNUM *fromHex(const char n[]);

Srp::Srp()
{
    BN_CTX *bnctx = BN_CTX_new();
    this->privatekey = randomBigNum(PRIVATE_KEY_SIZE);
    BIGNUM *g = fromHex(G);
    BIGNUM *n = fromHex(N);
    BIGNUM *publicFromPrivate = BN_new();

    BN_mod_exp(publicFromPrivate, g, this->privatekey, n, bnctx);
    this->publickey = publicFromPrivate;

    BN_free(g);
    BN_free(n);
    BN_CTX_free(bnctx);

    if (DEBUG){
        log("Private => %s\n", BN_bn2hex(publicFromPrivate));
        BN_print_fp(stdout, g);
        log("\n");
        BN_print_fp(stdout, n);

        log("\n");
        log("Public => %s\n", this->HexPublicKey());
    }
}

char* Srp::HexPublicKey() {
    return BN_bn2hex(this->publickey);
}

BIGNUM *randomBigNum(int length) {
    int hex_length = (2 * length);
    char bytes[hex_length + 1];
    BIGNUM *bn1 = BN_new();
    for (int i = 0; i < hex_length; i += 2) {
        // Static for now while debugging

        bytes[i] = '1';// rand() & 0xFF;
        if ((i + 1) < (hex_length - 1)) {
            bytes[i + 1] = '0';
        }
    }
    BN_hex2bn(&bn1, bytes);
    return bn1;
}

BIGNUM *fromHex(const char n[]) {
    BIGNUM *bn1 = BN_new();
    BN_hex2bn(&bn1, n);
    return bn1;
}
