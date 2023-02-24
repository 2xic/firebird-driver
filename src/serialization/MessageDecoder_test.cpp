#include <cheat.h>
#include "../MessagePadder.h"
#include "./MessageDecoder.h"
#include "../message.h"
#include "../opcodes.h"

// TODO: Endinaess is diffrent it seems
/*
CHEAT_TEST(
    message_padder_to_decoder,
    MessagePadder *message = new MessagePadder();
    message->write4Bytes(op_connect);

    MessageDecoder *decoder = new MessageDecoder();
    decoder->decode(message->payload, message->position);
    int value = decoder->readInt();
    cheat_assert(
        value == op_connect
    );
)

CHEAT_TEST(
    message_decoder_aaaa,
    MessagePadder *message = new MessagePadder();
    message->write4Bytes(0x41414141);

    MessageDecoder *decoder = new MessageDecoder();
    decoder->decode(message->payload, message->position);
    int value = decoder->readInt();
    cheat_assert(
        value == 0x41414141
    );
)
*/

/*
CHEAT_TEST(
    test_endinaess,
    unsigned char payloadx[4] = {0x5e, 0x00, 0x00, 0x00};

    MessageDecoder *decoder = new MessageDecoder();
    decoder->decode(payloadx, 4);
    int value = decoder->readInt();
    cheat_assert(
        value == 94
    );
)
*/