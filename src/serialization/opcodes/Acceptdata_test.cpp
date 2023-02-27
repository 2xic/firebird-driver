#include <cheat.h>
#include "./Acceptdata.h"
#include "./Response.h"
#include "../MessageDecoder.h"
#include "./Accpetdata_test_data.h"

CHEAT_TEST(accept_data_should_decode,
    MessageDecoder *message = new MessageDecoder();
    message->decode(test_data, 361);
    Response *ac_data = message->opcode();
    char *data = ((AcceptData*)ac_data)->data();
    cheat_assert(
        strncmp(
            data,
            "2EA190BEF27875B26769F5898ACFAE3B1CB679F2",
            40
        ) == 0
    );
)
