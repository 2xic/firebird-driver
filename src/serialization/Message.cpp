// https://github.com/hgourvest/node-firebird/blob/765bc1aead64e6df37c8baa330413f4210c6cd91/lib/index.js#L3559
#include "./Message.h"
#include "../utils/Config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Message::Message()
{
    this->position = 0;
    this->size = 32;
    this->payload = (int *)malloc(sizeof(int) * this->size);
}

void Message::writeInt8(int value)
{
    this->expand(4);
    this->payload[this->position++] = value;
}

void Message::writeString(char *value)
{
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
        this->payload = (int *)realloc(this->payload, sizeof(int) * this->size);
        memset((this->payload + old_size), 0, this->size - old_size);
    }
}

char *Message::dump()
{
    char *results = (char*)malloc(sizeof(char) * (this->size * 2 + 1));
    for (int i = 0; i < this->size; i++)
    {
        sprintf(results + i * 2, "%02x", this->payload[i]);
    }
    results[this->size * 2] = '\0';
    return results;
}
