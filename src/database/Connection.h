#ifndef Connection_H
#define Connection_H
#include "../message.h"
#include "../MessagePadder.h"
#include "../crypto/srp.h"
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include "../opcodes.h"
#include "../config.h"
#include "../utils.h"

class DatabaseConnection
{
Srp *srp;

public:
    Message *message;
    MessagePadder* messagePadder;

    DatabaseConnection();
};

inline void write_opcode(Message *message, int opcode, char*string) {
    message->writeInt8(opcode);
    message->writeString(string);
}

inline void write_int_opcode(Message *message, int opcode, int code) {
    message->writeInt8(opcode);
    message->writeInt8(code);
}

inline int min(int a, int b){
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

inline int max(int a, int b){
    if (a < b) {
        return b;
    } else {
        return a;
    }
}

inline void write_multi_block(Message *message, char*string) {
    toLowerCase(string);
    int left = strlen(string);
    int step = 0;

    log("Left == %i\n", left);
    log("%s\n", string);

    while(left){
        int length = min(254, left);

        message->writeInt8(CNCT_specific_data);
        message->writeInt8(length + 1);// + 1);
        message->writeInt8(step);

        message->expand(length);
        int stepLength = (step * 254)  + length;
        char temp = string[stepLength];
        *(string + stepLength) = '\0';

        log("out = %s\n", (string + (step  * 254)));
        log("length %i\n", length);

        message->writeRaw((string + (step  * 254)));
        *(string + stepLength) = temp;
        step++;
        
        left = max(0, left - length);
    }
}

#endif
