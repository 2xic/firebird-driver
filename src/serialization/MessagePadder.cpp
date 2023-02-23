#include "../MessagePadder.h"
#include "../config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../utils.h"

int alignBy4(int n) {
    return (n + 3) & ~3;
}


MessagePadder::MessagePadder()
{
    this->position = 0;
    this->size = 32;
    this->payload = (int *)malloc(sizeof(int) * this->size);
}

void MessagePadder::write4Bytes(int value) {
    this->expand(4);

    //printf("%i %X\n", value, value);

    int index = 3;
    while(value > 0) {
        this->payload[this->position + index] = (int)(value & 0xFF);
     //   printf("%x\n", this->payload[this->position + index]);
        value = (int)(value >> 8);
        index--;
    }
    this->position+=4;
}

void MessagePadder::writeString(char* value) {
    int aligned = alignBy4(strlen(value));
    this->expand(aligned + 4);

    this->write4Bytes(strlen(value));

    // TODO: Using stnrcpy is probably faster
    for (int i = 0; i < strlen(value); i++){
        this->payload[this->position++] = value[i];
    }
    this->position += (aligned - strlen(value));
}


void MessagePadder::writeMessage(Message *message)
{
    int aligned = alignBy4(message->length());
    this->expand(aligned + 4);
    this->write4Bytes(message->length());
//    printf("pos == %i\n", message->length());
    // TODO: Using stnrcpy is probably faster
    for (int i = 0; i < message->length(); i++){
        this->payload[this->position++] = message->payload[i];
    }
    this->position += (aligned - message->length());
}


void MessagePadder::expand(int expand)
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
        //printf("Expandded\n");
        this->payload = (int *)realloc(this->payload, sizeof(int) *  this->size);
        memset((this->payload + old_size), 0, this->size - old_size);
        /*
        for(int i = old_size; i < this->size;i++){
            *(this->payload + i) = 0;
        }*/
    }
}


char *MessagePadder::dumpToPositionRaw()
{
    return getAsHex(this->payload, this->position, false);
}

char *MessagePadder::dumpToPosition()
{
    return getAsHex(this->payload, this->position, true);
}

char *MessagePadder::dump()
{
    return getAsHex(this->payload, this->size, true);
}


int MessagePadder::length() {
    return this->position;
}
