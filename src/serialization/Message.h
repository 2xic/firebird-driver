#ifndef MESSAGE_H
#define MESSAGE_H

class Message
{
int position;
int size;

public:
    int *payload;

    Message();

    void writeInt8(int value);

    void writeString(char* value);

    int read(int position);

    int length();

    void expand(int expand);

    char* dump();

    void writeRaw(char*value);
};

#endif
