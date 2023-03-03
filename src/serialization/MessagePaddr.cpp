#include "./MessagePaddr.h"
#include "../utils/Config.h"
#include "../utils/Utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


MessagePaddr::MessagePaddr()
{
    this->position = 0;
    this->size = 32;
    this->payload = (unsigned char *)malloc(sizeof(unsigned char) * this->size);
}

MessagePaddr::~MessagePaddr(){
    this->size = 0;
    this->position = 0;
    free(this->payload);
    this->payload = NULL;
}

void MessagePaddr::write4Bytes(int value) {
    this->expand(4);

    int index = 3;
    while(value > 0) {
        this->payload[this->position + index] = (int)(value & 0xFF);
        value = (int)(value >> 8);
        index--;
    }
    while(index >= 0) {
        this->payload[this->position + index] = 0;
        index--;        
    }
    this->position+=4;
}

void MessagePaddr::writeString(char* value) {
    int length = strlen(value);
    int aligned = alignBy4(length);
    this->expand(aligned + 4);

    this->write4Bytes(length);

    // TODO: Using memcpy is probably faster
    for (int i = 0; i < length; i++){
        this->payload[this->position++] = value[i];
    }

    for (int i = 0; i < (aligned - length); i++){
        this->payload[this->position++] = 0;
    }
}


void MessagePaddr::writeMessage(Message *message)
{
    int aligned = alignBy4(message->length());
    this->expand(aligned + 4);
    this->write4Bytes(message->length());

    // TODO: Using memcpy is probably faster
    for (int i = 0; i < message->length(); i++){
        this->payload[this->position++] = message->payload[i];
    }
    this->position += (aligned - message->length());
    /*for (int i = this->position; i <= (aligned - message->length()); i++){
        this->payload[this->position++] = 0;
    }*/
}


void MessagePaddr::expand(int expand)
{
    bool expanded = false;
    int old_size = this->size;
    while (this->size < expand + this->position)
    {
        this->size *= 2;
        expanded = true;
    }

    if (expanded)
    {
        this->payload = (unsigned char *)realloc(this->payload, sizeof(unsigned char) *  this->size);
        memset((this->payload + old_size), 0, this->size - old_size);
    }
}

char *MessagePaddr::dumpToPosition()
{
    return getAsHex(this->payload, this->position);
}

char *MessagePaddr::dump()
{
    return getAsHex(this->payload, this->size);
}

int MessagePaddr::length() {
    return this->position;
}
