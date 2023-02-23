#ifndef MessagePadder_H
#define MessagePadder_H

#include "./message.h"

class MessagePadder
{
int size;

public:
    int position;
int *payload;

    MessagePadder();

    void write4Bytes(int value);

    void writeString(char *string);

    void writeMessage(Message *message);

    void expand(int expand);

    char *dump();

    char *dumpToPosition();

    char* dumpToPositionRaw();

    int length();
};

#endif
