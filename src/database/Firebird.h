#ifndef Firebird_H
#define Firebird_H

#include <sys/socket.h>
#include <unistd.h>
#include "Connection.h"
#include "../serialization/opcodes/Response.h"
#include "../serialization/opcodes/Responsedata.h"
#include <arpa/inet.h>

class Firebird
{
    struct sockaddr_in serv_addr;
    int status, valread, client_fd;
    DatabaseConnection *connection;

public:
    Firebird();
    ~Firebird();
    
    int Connect();
    int Disconnect();
    int Query();

    Response * ReadResponse(); 
    ResponseData* ReadResponseData();
};

#endif
