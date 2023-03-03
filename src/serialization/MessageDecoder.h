#ifndef MessageDecoder_H
#define MessageDecoder_H
#include "./opcodes/Response.h"

class MessageDecoder
{
public:
    int position;
    unsigned char*payload;
    int length;

    MessageDecoder();
    ~MessageDecoder();

    void decode(unsigned char *stream, int length);

    int readInt();

    unsigned char *readArray();

    char *readString();

    unsigned short readShortLe();

    unsigned char* readBuffer(int length);

    void readQuad();

    Response * opcode();
};

#endif
