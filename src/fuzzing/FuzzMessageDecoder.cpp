#include <stdio.h>
#include <stdlib.h>
#include "../serialization/MessageDecoder.h"

int main(int argc, char** argv){
    if (argc == 2){
        FILE *f = fopen(argv[1], "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        unsigned char *string = (unsigned char*)malloc(fsize + 1);
        fread(string, fsize, 1, f);
        fclose(f);

        string[fsize] = 0;

        MessageDecoder *message = new MessageDecoder();
        message->decode(string, fsize);
        message->opcode();

        free(string);
        delete message;
    }
}

// afl-fuzz -i afl_in -o afl_out -- ./fuzz_message_decder @@
