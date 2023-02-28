#include <cheat.h>
#include "./Acceptdata.h"
#include "./Response.h"
#include "../MessageDecoder.h"
#include "./Accpetdata_test_data.h"
#include "../../utils/Utils.h"

CHEAT_TEST(accept_data_should_decode,
    MessageDecoder *message = new MessageDecoder();
    message->decode(test_data, 361);
    Response *ac_data = message->opcode();
    char *data = ((AcceptData*)ac_data)->data();
    printf("%s\n", data);
    cheat_assert(
        strncmp(
            toLowerCase(data),
            "975057ca06867f552afbf0a4d9e23790bb87c036",
            40
        ) == 0
    );
)
