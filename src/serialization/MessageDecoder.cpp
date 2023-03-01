#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./MessageDecoder.h"
#include "../utils/Opcodes.h"
#include "../utils/Utils.h"
#include "./opcodes/Acceptdata.h"
#include "./opcodes/Responsedata.h"
#include "./opcodes/Response.h"

#define INT_SIZE 4
#define SHORT_SIZE 2

void DebugBuffer(MessageDecoder *decoder)
{
    for (int i = 0; i < decoder->length; i++)
    {
        int x = (decoder->payload[i]) & 0xff;
        printf("%x ", x);
    }
    printf("\n");
}

MessageDecoder::MessageDecoder()
{
    this->position = 0;
}

void MessageDecoder::decode(unsigned char *stream, int length)
{
    // TODO: Better if this is set in the constructor
    this->payload = stream;
    this->position = 0;
    this->length = length;
}

Response* MessageDecoder::opcode()
{
        // DebugBuffer(this);
        int opcode = this->readInt();

        switch (opcode)
        {
        case op_accept_data:
        {
            AcceptData *ad = new AcceptData();
            ad->decode(this);
            return ad;
        }
        case op_response:{
            ResponseData *rp = new ResponseData(this);
            return rp;
        }
        case op_fetch_response: {
            printf("status == %i \n", this->readInt());
            printf("fcount == %i \n", this->readInt());
            this->position = 16;

            printf("id == %i \n", this->readInt());

            int length = 4;
            char *test_boolean = (char *)malloc(sizeof(char) * length);
            memcpy(test_boolean, (this->payload + this->position), length);
            test_boolean[length] = '\0';

            printf("test_boolean == %s \n", test_boolean);

            free(test_boolean);
            return NULL;
        }
        default:
        {
            printf("Unknown opcode %i, %x \n", opcode, opcode);
            printf("Exiting .... \n");
            exit(0);
        }
    }
}

int MessageDecoder::readInt()
{
    int value = 0;
    for (int i = 0; i < INT_SIZE; i++)
    {
        value |= ((int)this->payload[(this->position + (3 - i))]) << (8 * i);
    }
    this->position += INT_SIZE;
    return value;
}

void MessageDecoder::readQuad() {
    this->readInt();
    this->readInt();
}

unsigned short MessageDecoder::readShortLe()
{
    int value = 0;
    for (int i = 0; i < SHORT_SIZE; i++)
    {
        value |= ((int)this->payload[(this->position + i)]) << (8 * i);
    }
    this->position += SHORT_SIZE;
    return value;
}

unsigned char *MessageDecoder::readBuffer(int length)
{
    unsigned char *output = (unsigned char *)malloc(sizeof(unsigned char) * length);
    // TODO: memcpy is faster probably
    for(int i = 0; i < length; i++){
        output[i] = this->payload[this->position++];
    }
    return output;
}

unsigned char *MessageDecoder::readArray()
{
    int length = this->readInt();
    if (length)
    {
        unsigned char *size = (unsigned char *)malloc(sizeof(unsigned char) * length);
        memcpy(size, (this->payload + this->position), length);

        this->position += alignBy4(length);
        return size;
    }
    return NULL;
}

char *MessageDecoder::readString()
{
    int length = this->readInt();
    if (length)
    {
        char *size = (char *)malloc(sizeof(char) * length);
        memcpy(size, (this->payload + this->position), length);
        size[length] = '\0';

        this->position += alignBy4(length);
        return size;
    }
    return NULL;
}
