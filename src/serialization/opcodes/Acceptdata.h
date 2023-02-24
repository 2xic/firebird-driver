#ifndef AcceptData_H
#define AcceptData_H
#include "../MessageDecoder.h"

class AcceptData
{
public:
    AcceptData();

    void decode(MessageDecoder *decoder);
};

#endif
