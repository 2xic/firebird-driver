#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "config.h"
#include "Connection.h"
#include "Firebird.h"

Firebird::Firebird()
{
    if ((this->client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = htons(FIREBIRD_SERVER_PORT);
    if (inet_pton(AF_INET, FIREBIRD_SERVER_HOST, &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return;
    }
}

int Firebird::Connect()
{
//    printf("%i\n", this->serv_addr.sin_addr);
 //   printf("%i\n", this->serv_addr.sin_port);
    if ((this->status = connect(this->client_fd, (struct sockaddr *)&this->serv_addr,
                            sizeof(this->serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    this->connection = new DatabaseConnection();
    /*
    char *message = this->connection->messagePadder->dumpToPosition();
    printf("message == %li, %d\n ", strlen(message), this->connection->messagePadder->position);
    printf("message == %s\n ", message);
    exit(0);
    */

    // TODO: Fix the bug casuing data to be sent the wrong way
    std::string hex = "0000000100000013000000030000000100000004746573740000000400000138090653595344424108035372700a0f5372702c4c65676163795f4175746807ff00326535366566393061663364313330613165376163613737383636306564393664393434313830333637376166316138643166336238663966653432383139633437346133303963633266633863656536306365313462623739326163373935353438396334613032613162303434346437663461646634663533333731646336333332613561376138663733396465366264393631613431383863633765613234323637633766343863636630613330636465623233303865656536306266656639643565363062633335386161343536376234383862663464653933636464626266373132353537366334613266323933333366613230633530383607030130630b04000000000103632b2b04056c6f63616c06000000000a000000010000000200000003000000010000800b000000010000000500000005000000020000800c000000010000000500000005000000030000800d00000001000000050000000500000004";
    int len = hex.length();
    std::string newString;
    for(int i=0; i< len; i+=2)
    {
        std::string byte = hex.substr(i,2);
        char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
        newString.push_back(chr);
    }

    int bytesSent = write(
        this->client_fd, 
        newString.c_str(),
        this->connection->messagePadder->position
    );
    /*this->connection->messagePadder->payload*/

    printf("sent length == %i\n ", bytesSent);
    printf("status == %i\n ", this->status);
    printf("data == %s\n", this->connection->messagePadder->dumpToPosition());

    while(1) {
        char buffer[1024] = { 0 };
        int valread = recv(this->client_fd, buffer, 1024, 0);
        printf("Output = %i\n", valread);
        printf("%x\n", buffer);
        for(int i = 0; i < 1024; i++){
            printf("%x", buffer[i]);
        }
        printf("\n");
        exit(0);
//        printf("Output = %s\n", buffer);
        sleep(1);
    }

    return 0;
}