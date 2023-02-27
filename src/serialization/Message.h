#ifndef MESSAGE_H
#define MESSAGE_H

class Message
{
int size;

public:
    unsigned char *payload;
    int position;

    Message();

    // TODO: This name does not make sense
    void writeInt8(int value);

    // TODO: This name does not make sense
    void writeInt32(int value);

    void writeString(char* value);

    void writeByte(unsigned char value);

    int read(int position);

    int length();

    void expand(int expand);

    char* dump();

    char *dumpToPosition();

    void writeRaw(char*value);
};

#endif
