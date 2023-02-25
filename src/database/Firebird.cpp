#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../utils/Config.h"
#include "Connection.h"
#include "Firebird.h"
#include "../serialization/MessageDecoder.h"

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
    if ((this->status = connect(this->client_fd, (struct sockaddr *)&this->serv_addr,
                            sizeof(this->serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    this->connection = new DatabaseConnection();
    int bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position
    );

    printf("sent length == %i\n ", bytesSent);
    printf("status == %i\n ", this->status);
    printf("data == %s\n", this->connection->messagePaddr->dumpToPosition());

    MessageDecoder *decode = new MessageDecoder();
    while(1) {
        unsigned char buffer[1024] = { 0 };
        int length = recv(this->client_fd, buffer, 1024, 0);
        printf("Output = %i\n", length);

        decode->decode((unsigned char*)&buffer, length);
        decode->opcode();

        printf("\n");
        exit(0);
    }

    return 0;
}
