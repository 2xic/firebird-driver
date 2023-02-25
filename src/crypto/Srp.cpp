#include <openssl/bn.h>
#include <openssl/crypto.h>
#include "Srp.h"
#include "../utils/Config.h"
#include "Sha1.h"
#include <cstring>

// https://github.com/nakagami/firebirdsql/blob/master/srp.go
const char G[] = "02";
const char N[] = "E67D2E994B2F900C3F41F08F5BB2627ED0D49EE1FE767A52EFCD565CD6E768812C3E1E9CE8F0A8BEA6CB13CD29DDEBF7A96D4A93B55D488DF099A15C89DCB0640738EB2CBDD9A8F7BAB561AB1B0DC1C6CDABF303264A08D1BCA932D1F1EE428B619D970F342ABA9A65793B8B2F041AE5364350C16F735F56ECBCA87BD57B29E7";
const char K[] = "1277432915985975349439481660349303019122249719989";
const int PRIVATE_KEY_SIZE = 128;

BIGNUM *combined(
    char *A,
    char *B
);
BIGNUM *authHash(
    char *username,
    char *password,
    char *salt
);
BIGNUM *randomBigNum(int length);
BIGNUM *fromHex(const char n[]);
BIGNUM *fromDecimals(const char n[]);

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

    if (DEBUG)
    {
        log("Private => %s\n", BN_bn2hex(publicFromPrivate));
        BN_print_fp(stdout, g);
        log("\n");
        BN_print_fp(stdout, n);

        log("\n");
        log("Public => %s\n", this->HexPublicKey());
    }
}

char *Srp::HexPublicKey()
{
    return BN_bn2hex(this->publickey);
}

/*
    TODO: This is messy, and does not properly free the memory 
*/
BIGNUM* Srp::ClientSession(
    char *username,
    char *password,
    char *salt,
    char *client_publickey,
    char *server_publickey,
    char *client_privatekey)
{
    BIGNUM *u = combined(
        client_publickey,
        server_publickey
    );
    BIGNUM *x = authHash(
        username,
        password,
        salt
    );
    printf("Hex => %s\n", BN_bn2hex(u));
    printf("Hex => %s\n", BN_bn2hex(x));

    BIGNUM *B = fromDecimals(server_publickey);
    BIGNUM *A = fromDecimals(client_privatekey);
    BIGNUM *g = fromHex(G);
    BIGNUM *k = fromDecimals(K);
    BIGNUM *n = fromHex(N);
    BIGNUM *gx = BN_new();
    BIGNUM *_kgx = BN_new();
    BIGNUM *kgx = BN_new();

    BN_CTX *bnctx = BN_CTX_new();
    BN_mod_exp(gx, g, x, n, bnctx);

    BN_mul(
        _kgx, k, gx, bnctx
    );
    BN_mod(
        kgx,
        _kgx, 
        n,
        bnctx
    );

    printf("k => %s\n", BN_bn2hex(k));
    printf("gx => %s\n", BN_bn2hex(gx));
    printf("kgx => %s\n", BN_bn2hex(kgx));

    BIGNUM *_diff = BN_new();
    BIGNUM *diff = BN_new();
    BN_sub(
        _diff, 
        B,
        kgx
    );
    BN_mod(
        diff,
        _diff,
        n,
        bnctx
    );
    BIGNUM *zero = fromDecimals("0");

    if (BN_cmp(diff, zero) == -1){
        BN_add(
            _diff,
            diff,
            n
        );
        // TODO, this is dirty, and also need to copy to another diff.

    }

    printf("diff => %s\n", BN_bn2hex(diff));
    BIGNUM *ux = BN_new();
    BIGNUM *_ux = BN_new();
    BN_mul(
        _ux, u, x, bnctx
    );
    BN_mod(
        ux,
        _ux, 
        n,
        bnctx
    );
    printf("ux => %s\n", BN_bn2hex(ux));

    BIGNUM *aux = BN_new();
    BIGNUM *_aux = BN_new();
    BN_add(
        _aux, 
        A,
        ux
    );
    BN_mod(
        aux, _aux, n, bnctx
    );
    printf("aux => %s\n", BN_bn2hex(aux));

    BIGNUM *session_secret = BN_new();
    BN_mod_exp(session_secret, diff, aux, n, bnctx);
    printf("session_secret => %s\n", BN_bn2hex(session_secret));


    BIGNUM *K = BN_new();
    int session_secret_l = BN_num_bytes(session_secret);
    unsigned char *session_secret_c = (unsigned char*)malloc((sizeof(unsigned char)) * session_secret_l);
    BN_bn2bin(session_secret, session_secret_c);

    Sha1 *sha1 = new Sha1();
    sha1->update(session_secret_c, session_secret_l);
    char *hash = sha1->hash();
    delete sha1;

    BIGNUM *value = fromHex(hash);
    printf("value => %s\n", BN_bn2hex(value));
    return value;
}


BIGNUM *combined(
    char *A,
    char *B
)  {
    // A and B is numeric 
    BIGNUM *a_p = fromDecimals(A);
    BIGNUM *b_p = fromDecimals(B);

    int a_l = BN_num_bytes(a_p);
    int b_l = BN_num_bytes(b_p);

    unsigned char *a_c = (unsigned char*)malloc((sizeof(unsigned char)) * a_l);
    unsigned char *b_c = (unsigned char*)malloc((sizeof(unsigned char)) * b_l);

    BN_bn2bin(a_p, a_c);
    BN_bn2bin(b_p, b_c);


    BN_free(a_p);
    BN_free(b_p);

    Sha1 *sha1 = new Sha1();
    sha1->update(a_c, a_l);
    sha1->update(b_c, b_l);
    char *hash = sha1->hash();
    delete sha1;
    BIGNUM *value = fromHex(hash);

    free(hash);
    free(a_c);
    free(b_c);

    return value;
}


BIGNUM *authHash(
    char *username,
    char *password,
    char *salt
) {
    unsigned char chars[] = {':'};
    Sha1 *sha1 = new Sha1();
    sha1->update((unsigned char*)username, strlen(username));
    sha1->update((unsigned char*)&chars, 1);
    sha1->update((unsigned char*)password, strlen(password));
    unsigned char *hash = sha1->raw_hash();
    delete sha1;

    Sha1 *salt_sha1 = new Sha1();
    salt_sha1->update((unsigned char*)salt, strlen(salt));
    salt_sha1->update((unsigned char*)hash, 20);

    char *salt_sha1_hash = salt_sha1->hash();
    delete salt_sha1;

    BIGNUM *value = fromHex(salt_sha1_hash);

    free(hash);
    free(salt_sha1_hash);

    return value;
}

BIGNUM *randomBigNum(int length)
{
    int hex_length = (2 * length);
    char bytes[hex_length + 1];
    BIGNUM *bn1 = BN_new();
    for (int i = 0; i < hex_length; i += 2)
    {
        // Static for now while debugging

        bytes[i] = '1'; // rand() & 0xFF;
        if ((i + 1) < (hex_length - 1))
        {
            bytes[i + 1] = '0';
        }
    }
    BN_hex2bn(&bn1, bytes);
    return bn1;
}

BIGNUM *fromHex(const char n[])
{
    BIGNUM *bn1 = BN_new();
    BN_hex2bn(&bn1, n);
    return bn1;
}

BIGNUM *fromDecimals(const char n[])
{
    BIGNUM *bn1 = BN_new();
    BN_dec2bn(&bn1, n);
    return bn1;
}
