#include <openssl/bn.h>
#include <openssl/crypto.h>
#include "Srp.h"
#include "../utils/Config.h"
#include "../utils/Utils.h"
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

void log_bignum(const char *__restrict__ __format, BIGNUM *a){
    if(DEBUG){
        char *a_hex = BN_bn2hex(a);
        printf(__format, a_hex);
        free(a_hex);
    }
}

Srp::Srp()
{
    this->bnctx = BN_CTX_new();
    BN_CTX_start(this->bnctx);

    this->privatekey = randomBigNum(PRIVATE_KEY_SIZE);
    BIGNUM *g = fromHex(G);
    BIGNUM *n = fromHex(N);
    BIGNUM *publicFromPrivate = BN_new();

    BN_mod_exp(publicFromPrivate, g, this->privatekey, n, this->bnctx);
    this->publickey = publicFromPrivate;

    BN_free(g);
    BN_free(n);

    //BN_CTX_free(bnctx);

    if (DEBUG)
    {
        log_bignum("Private => %s\n", publicFromPrivate);
        BN_print_fp(stdout, g);
        log("\n");
        BN_print_fp(stdout, n);

        log("\n");
        log("Public => %s\n", this->HexPublicKey());
    }
}
Srp::~Srp() {
    BN_CTX_end(this->bnctx);
    BN_free(this->privatekey);
    BN_free(this->publickey);
}

char *Srp::HexPublicKey()
{
    return BN_bn2hex(this->publickey);
}

char *Srp::DecPublicKey()
{
    return BN_bn2dec(this->publickey);
}

char *Srp::HexPrivateKey()
{
    return BN_bn2hex(this->privatekey);
}

char *Srp::DecPrivateKey()
{
    return BN_bn2dec(this->privatekey);
}

unsigned char *BigNum2Char(BIGNUM *a){
    int a_l = BN_num_bytes(a);
    unsigned char *a_c = (unsigned char*)malloc((sizeof(unsigned char)) * (a_l));
    BN_bn2bin(a, a_c);

    return a_c;
}

BIGNUM *BigNum2Hash2BigNUm(BIGNUM *a) {
    int a_l = BN_num_bytes(a);
    unsigned char *a_c = (unsigned char*)malloc((sizeof(unsigned char)) * a_l);
    BN_bn2bin(a, a_c);

    Sha1 *sha1 = new Sha1();
    sha1->update(a_c, a_l);
    char *hash = sha1->hash();

    BIGNUM *value = fromHex(hash);

    free(a_c);
    free(hash);
    delete sha1;

    return value;
}

void RawBigNumHash(Sha1 *hasher, BIGNUM * a){
    unsigned char *a_chars = BigNum2Char(a);
    hasher->update(a_chars, BN_num_bytes(a));
    free(a_chars);
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
    log_bignum("Hex => %s\n", u);
    log_bignum("Hex => %s\n", x);

    BIGNUM *B = fromDecimals(server_publickey);
    BIGNUM *A = fromDecimals(client_privatekey);
    BIGNUM *g = fromHex(G);
    BIGNUM *k = fromDecimals(K);
    BIGNUM *n = fromHex(N);
    BIGNUM *gx = BN_new();
    BIGNUM *_kgx = BN_new();
    BIGNUM *kgx = BN_new();

    BN_CTX *bnctx = this->bnctx;
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

    log_bignum("k => %s\n", k);
    log_bignum("gx => %s\n", gx);
    log_bignum("kgx => %s\n", kgx);

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

    log_bignum("diff => %s\n", diff);
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
    log_bignum("ux => %s\n", ux);

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
    log_bignum("aux => %s\n", aux);

    BIGNUM *session_secret = BN_new();
    BN_mod_exp(session_secret, diff, aux, n, bnctx);
    log_bignum("session_secret => %s\n", session_secret);

    Sha1 *sha1 = new Sha1();
    RawBigNumHash(sha1, session_secret);
    
    char *hash = sha1->hash();

    BIGNUM *value = fromHex(hash);
    log_bignum("value => %s\n", value);

    delete sha1;
    free(hash);

    BN_free(session_secret);
    BN_free(aux);
    BN_free(_aux);
    BN_free(ux);
    BN_free(_ux);
    BN_free(zero);
    BN_free(diff);
    BN_free(_diff);
    BN_free(_kgx);
    BN_free(kgx);
    BN_free(k);
    BN_free(gx);

    BN_free(B);
    BN_free(A);
    BN_free(g);
    BN_free(n);
    BN_free(u);
    BN_free(x);

   // BN_CTX_free(bnctx);
    
    return value;
}

BIGNUM* Srp::ClientProof(
    char *username,
    char *password,
    char *salt,
    char *client_publickey,
    char *server_publickey,
    char *client_privatekey
)
{
    BIGNUM *clientSession = this->ClientSession(
        username,
        password,
        salt,
        client_publickey,
        server_publickey,
        client_privatekey   
    );
    BIGNUM *g = fromHex(G);
    BIGNUM *n = fromHex(N);

    BIGNUM *n_hash = BigNum2Hash2BigNUm(n);
    BIGNUM *g_hash = BigNum2Hash2BigNUm(g);

    log_bignum("n => %s\n", n);
    log_bignum("g => %s\n", g);

    log_bignum("n_hash => %s\n", n_hash);
    log_bignum("g_hash => %s\n", g_hash);
    log_bignum("n => %s\n", n);
    log_bignum("g => %s\n", g);

    BN_CTX *bnctx = this->bnctx;// BN_CTX_new();
    BIGNUM *n1_mod = BN_new();
    BN_mod_exp(n1_mod, n_hash, g_hash, n, bnctx);

    log_bignum("n1_mod => %s\n", n1_mod);

    Sha1 *username_sha1 = new Sha1();
    username_sha1->update((unsigned char*)username, strlen(username));
    char *username_sha1_hash = username_sha1->hash();

    BIGNUM *username_hash = fromHex(username_sha1_hash);
    log_bignum("n2_mod => %s\n", username_hash);
    log("\n");


    Sha1 *salt_sha1_final = new Sha1();
    RawBigNumHash(salt_sha1_final, n1_mod);
    RawBigNumHash(salt_sha1_final, username_hash);
    salt_sha1_final->update((unsigned char*)salt, strlen(salt));

    BIGNUM *client_publickey_num = fromDecimals(client_publickey);
    RawBigNumHash(salt_sha1_final, client_publickey_num);

    BIGNUM *server_public_key_num = fromDecimals(server_publickey);
    RawBigNumHash(salt_sha1_final, server_public_key_num);
    RawBigNumHash(salt_sha1_final, clientSession);

    char *salt_sha1_hash_final = salt_sha1_final->hash();
    log("hash == %s\n", salt_sha1_hash_final);

    BIGNUM *value = fromHex(salt_sha1_hash_final);

    delete salt_sha1_final;
    delete username_sha1;

    BN_free(n);
    BN_free(g);
    BN_free(n_hash);
    BN_free(g_hash);
    BN_free(username_hash);
    BN_free(client_publickey_num);
    BN_free(server_public_key_num);
    BN_free(n1_mod);
    BN_free(clientSession);

    free(salt_sha1_hash_final);
    free(username_sha1_hash);

//    BN_CTX_end(bnctx);
 //   BN_CTX_free(bnctx);

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

    Sha1 *sha1 = new Sha1();
    sha1->update(a_c, a_l);
    sha1->update(b_c, b_l);
    char *hash = sha1->hash();
    BIGNUM *value = fromHex(hash);

    delete sha1;

    BN_free(a_p);
    BN_free(b_p);

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

    Sha1 *salt_sha1 = new Sha1();
    salt_sha1->update((unsigned char*)salt, strlen(salt));
    salt_sha1->update((unsigned char*)hash, 20);

    char *salt_sha1_hash = salt_sha1->hash();

    BIGNUM *value = fromHex(salt_sha1_hash);

    delete sha1;
    delete salt_sha1;

    free(hash);
    free(salt_sha1_hash);

    return value;
}

BIGNUM *randomBigNum(int length)
{
    int hex_length = (2 * length) ;
    char bytes[hex_length + 1];
    for (int i = 0; i < hex_length; i += 2)
    {
        bytes[i] = '1';
        if ((i + 1) < (hex_length - 1))
        {
            bytes[i + 1] = '0';
        }
    }
    return fromHex(bytes);
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
