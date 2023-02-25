#ifndef MessagePaddr_H
#define MessagePaddr_H

#include "./Message.h"

class MessagePaddr
{
int size;

public:
    int position;
    unsigned char *payload;

    MessagePaddr();

    void write4Bytes(int value);

    void writeString(char *string);

    void writeMessage(Message *message);

    void expand(int expand);

    char *dump();

    char *dumpToPosition();

    int length();
};

#endif
