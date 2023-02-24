#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./MessageDecoder.h"
#include "../opcodes.h"
#include "../utils.h"
#include "./opcodes/Acceptdata.h"

#define INT_SIZE 4
#define SHORT_SIZE 2


void DebugBuffer(MessageDecoder *decoder) {
    for(int i = 0; i < decoder->length; i++){
        int x = (decoder->payload[i]) & 0xff;
        printf("%x ", x);
        /*
        for(int j = 0; j < INT_SIZE; j++){
            int x = (decoder->payload[i] >> (8*j)) & 0xff;
            printf("%x ", x);
        }*/
    }
    printf("\n");
}


MessageDecoder::MessageDecoder()
{
    this->position = 0;
}

void MessageDecoder::decode(unsigned char* stream, int length)
{
    // TODO: Better if this is set in the constructor
    this->payload = stream;
    this->position = 0;
    this->length = length;
//    this->opcode();
}

void MessageDecoder::opcode(){
    DebugBuffer(this);
    int opcode = this->readInt();

    switch(opcode){
        case op_accept_data:{
            printf("Opcode op_accept_data\n");
            AcceptData *ad = new AcceptData();
            ad->decode(this);
            break;
        }
        default:{
            printf("Unknown opcode %i, %x \n", opcode, opcode);
            printf("Exiting .... \n");
            exit(0);
        }
    }
}

int MessageDecoder::readInt() {
    int value = 0;
    for (int i = 0; i < INT_SIZE; i++) {
        value |= ((int) this->payload[(this->position+ (3 - i))]) << (8 * i);
    }
    this->position += INT_SIZE;
    return value;
}

unsigned short MessageDecoder::readShortLe() {
    // DebugBuffer(this);
    int value = 0;
    for (int i = 0; i < SHORT_SIZE; i++) {
        value |= ((int) this->payload[(this->position + i)]) << (8 * i);
    }
    this->position += SHORT_SIZE;
    return value;
}

unsigned char *MessageDecoder::readBuffer(int length) {
    this->position += length;
    return NULL;
}

/*
    Problem, we are working on 4 bytes at the time, and not a single byte
*/
unsigned char* MessageDecoder::readArray() {
    int length = this->readInt();
    printf("Array length == %i\n", length);
    if (length) {
        unsigned char *size = (unsigned char*)malloc(sizeof(unsigned char) * length);
        memcpy(size, (this->payload + this->position), length);

        this->position += alignBy4(length);
        return size;
    }
    return NULL;
}

char *MessageDecoder::readString() {
    int length = this->readInt();
    printf("String length == %i\n", length);
    if (length) {
        char *size = (char*)malloc(sizeof(char) * length);
        memcpy(size, (this->payload + this->position), length);
        size[length] = '\0';

        this->position += alignBy4(length);
        return size;
    }
    return NULL;
}
