#include <cheat.h>
#include "../MessagePadder.h"
#include "../message.h"
#include "../opcodes.h"

CHEAT_TEST(
    message_padder_string,
    const char *string = "test\0";

    MessagePadder *message = new MessagePadder();
    message->write4Bytes(op_connect);
    message->write4Bytes(op_attach);
    message->write4Bytes(CONNECT_VERSION3);
    message->write4Bytes(ARCHITECTURE_GENERIC);
    message->writeString((char *)string);
    /*
    [
        [PROTOCOL_VERSION10, ARCHITECTURE_GENERIC, ptype_rpc, ptype_batch_send, 1],
        [PROTOCOL_VERSION11, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 2],
        [PROTOCOL_VERSION12, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 3],
        [PROTOCOL_VERSION13, ARCHITECTURE_GENERIC, ptype_lazy_send, ptype_lazy_send, 4],
    ]
    */
    message->write4Bytes(4);

    cheat_assert(
        strncmp(
            message->dump(),
            "0000000100000013000000030000000100000004746573740000000400000000",
            64) == 0);
    Message *raw_message = new Message();
    raw_message->writeInt8(0);
    raw_message->writeString((char *)string);
    cheat_assert(
        raw_message->length() == 6);

    message->writeMessage(raw_message);

    cheat_assert(
        strncmp(
            message->dump(),
            "00000001000000130000000300000001000000047465737400000004000000060004746573740000000000000000000000000000000000000000000000000000",
            128) == 0);
    cheat_assert(
        message->length() == 40
    );

    for (int i = 0; i < 4; i++) {
        message->write4Bytes(suppoerted_protocols[i][0]);
        message->write4Bytes(suppoerted_protocols[i][1]);
        message->write4Bytes(suppoerted_protocols[i][2]);
        message->write4Bytes(suppoerted_protocols[i][3]);
        message->write4Bytes(suppoerted_protocols[i][4]);
    }

  //  printf("%s\n", message->dump());
    cheat_assert(
        strncmp(
            message->dump(),
            "000000010000001300000003000000010000000474657374000000040000000600047465737400000000000a000000010000000200000003000000010000800b000000010000000500000005000000020000800c000000010000000500000005000000030000800d000000010000000500000005000000040000000000000000",
            256) == 0);

    cheat_assert(
        strncmp(
            message->dumpToPosition(),
            "000000010000001300000003000000010000000474657374000000040000000600047465737400000000000a000000010000000200000003000000010000800b000000010000000500000005000000020000800c000000010000000500000005000000030000800d00000001000000050000000500000004",
            240) == 0);
)
