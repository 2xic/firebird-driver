#ifndef MessageDecoder_H
#define MessageDecoder_H

class MessageDecoder
{
public:
    int position;
    unsigned char*payload;
    int length;

    MessageDecoder();

    void decode(unsigned char *stream, int length);

    int readInt();

    unsigned char *readArray();

    char *readString();

    unsigned short readShortLe();

    unsigned char* readBuffer(int length);

    void opcode();
};

#endif
