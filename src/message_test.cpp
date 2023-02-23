#include <cheat.h>
#include "message.h"

CHEAT_TEST(uint_message_encoding,
    Message *message = new Message();
    message->writeInt8(1);
    cheat_assert(message->read(0) == 1);
)


CHEAT_TEST(string_message_encoding,
    const char *string = "test\0";

    Message *message = new Message();
    message->writeString((char*)string);
    cheat_assert(message->read(0) == 4);
    cheat_assert(message->read(1) == 116);
    cheat_assert(message->read(2) == 101);
    cheat_assert(message->read(3) == 115);
    cheat_assert(message->read(4) == 116);
    cheat_assert(message->length() == 5);
)

CHEAT_TEST(should_be_writeable_ff,
    Message *message = new Message();
    message->writeInt8(0xff);

    cheat_assert(message->read(0) == 0xff);
)
