#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./Responsedata.h"
#include "../../utils/Opcodes.h"
#include "../../utils/Config.h"
#include "../../crypto/Srp.h"

ResponseData::ResponseData(MessageDecoder *decoder){
    this->database_handle = decoder->readInt();
    decoder->readQuad();
    decoder->readArray();

    log("pos == %i\n", decoder->position);
    int op = decoder->readInt();
    
    bool done = false;
    while(!done){
        switch(op) {
            case isc_arg_end: {
                // 
            }
            case isc_arg_gds: {
                int number = decoder->readInt();
                if (number == 0){
                    done = true;
                    break;
                }
                log("gscode == %i == num\n", number);
                log("pos == %i\n", decoder->position);
                this->is_error = true;
                done = true;
                break;
            }
            default: {
                log("Unkown opcode == %i\n", op);
                done = true;
                break;
            }
        }
    }
}

char* ResponseData::data(){
    return NULL;
}

