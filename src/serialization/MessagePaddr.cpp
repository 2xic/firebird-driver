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

void MessagePaddr::write4Bytes(int value) {
    this->expand(4);

    int index = 3;
    while(value > 0) {
        this->payload[this->position + index] = (int)(value & 0xFF);
        value = (int)(value >> 8);
        index--;
    }
    this->position+=4;
}

void MessagePaddr::writeString(char* value) {
    int aligned = alignBy4(strlen(value));
    this->expand(aligned + 4);

    this->write4Bytes(strlen(value));

    // TODO: Using memcpy is probably faster
    for (int i = 0; i < strlen(value); i++){
        this->payload[this->position++] = value[i];
    }
    this->position += (aligned - strlen(value));
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
