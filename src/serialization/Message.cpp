// https://github.com/hgourvest/node-firebird/blob/765bc1aead64e6df37c8baa330413f4210c6cd91/lib/index.js#L3559
#include "./Message.h"
#include "../utils/Config.h"
#include "../utils/Utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Message::Message()
{
    this->position = 0;
    this->size = 32;
    this->payload = (unsigned char *)malloc(sizeof(unsigned char) * this->size);
}

void Message::writeInt8(int value)
{
    this->expand(4);
    // TODO: Fix this since we moved over to use unsigned char not int
    if (value > 0xFF){
        printf("THis is a bug, you need to fix the value range");
    }
    this->payload[this->position++] = value;
}

void Message::writeByte(unsigned char value)
{
    this->expand(1);
    this->payload[this->position++] = value;
}

void Message::writeInt32(int value)
{
    this->expand(4);

    int index = 0;
    while(value > 0) {
        this->payload[this->position + index] = (int)(value & 0xFF);
        value = (int)(value >> 8);
        index++;
    }
    while(index < 4) {
        this->payload[this->position + index] = 0;
        index++;        
    }
    this->position+=4;
}
/*
013004555446381c0464656d6f4704d2 4a0474657374

013004555446381c0464656d6f4704d2      4a0474657374
013004555446381c0464656d6f4704d20400004a0474657374542834643330623836643065336364613430313964396136336664303938373839653537313338316433
*/
void Message::writeString(char *value)
{
    this->expand(strlen(value) + 1);
    this->writeInt8(strlen(value));
    for (int i = 0; i < strlen(value); i++)
    {
        this->payload[this->position++] = (int)value[i];
    }
}

void Message::writeRaw(char *value)
{
    for (int i = 0; i < strlen(value); i++)
    {
        this->payload[this->position++] = value[i];
    }
}

int Message::read(int position)
{
    log("Reading: %i %c\n", (int)this->payload[position], this->payload[position]);
    return (int)this->payload[position];
}

int Message::length()
{
    return this->position;
}

void Message::expand(int expand)
{
    bool expanded = false;
    int old_size = this->size;
    while (this->size < (expand + this->position))
    {
        this->size *= 2;
        expanded = true;
    }

    if (expanded)
    {
        this->payload = (unsigned char *)realloc(this->payload, sizeof(unsigned char) * this->size);
        memset((this->payload + old_size), 0, this->size - old_size);
    }
}

char *Message::dump()
{
    return getAsHex(this->payload, this->size);
}


char *Message::dumpToPosition()
{
    return getAsHex(this->payload, this->position);
}
