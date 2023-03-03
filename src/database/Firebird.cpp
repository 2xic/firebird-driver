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
#include "../serialization/opcodes/Response.h"
#include "../serialization/opcodes/Responsedata.h"
#include "../serialization/opcodes/Acceptdata.h"

// TODO: Fix isInstanceOf does not work with node addon
/*
template<typename Base, typename T>
inline bool isInstanceOf(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}
*/

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

Firebird::~Firebird()
{
    delete this->connection;
}

int Firebird::Connect()
{
    if ((this->status = connect(this->client_fd, (struct sockaddr *)&this->serv_addr,
                                sizeof(this->serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    this->connection = new DatabaseConnection(
        (char *)FIREBIRD_USER,
        (char *)FIREBIRD_PASSWORD,
        (char *)FIREBIRD_DATABASE);
    this->connection->Connect();

    int bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position);

    log("sent length == %i\n ", bytesSent);
    log("status == %i\n ", this->status);
    log("data == %s\n", this->connection->messagePaddr->dumpToPosition());

    Response *decoded = this->ReadResponse();

    log("Sending attach opcode\n");

    // if (isInstanceOf<AcceptData>(decoded)) {
    AcceptData *data = (AcceptData *)decoded;
    char *accept_data = data->data();
    this->connection->Attach(
        accept_data);
    bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position);
    log("Sent == %i\n", bytesSent);

    free(accept_data);
    decoded = NULL;
    delete data;

    // } else {
    //   delete decoded;
    //     printf("Wrong class, should exit");
    // }

    return 0;
}

int Firebird::Query()
{
    ResponseData *database_handle = this->ReadResponseData();
    this->connection->startTransaction(
        database_handle->database_handle);
    int bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position);
    log("Sent == %i\n", bytesSent);

    ResponseData *traansaction_handle = this->ReadResponseData();
    // Is the transaction active ?
    // Yes it is
    this->connection->prepareStatment(
        database_handle->database_handle,
        traansaction_handle->database_handle,
        (char *)"SELECT * FROM MYDATA");
    bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position);
    log("Sent == %i\n", bytesSent);
    ResponseData *statment_handle = this->ReadResponseData();

    // Is the transaction active ?
    // Yes it is
    this->connection->executeStatment(
        database_handle->database_handle,
        traansaction_handle->database_handle,
        statment_handle->database_handle);
    bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position);
    log("Sent == %i\n", bytesSent);
    ResponseData *statment_response = this->ReadResponseData();
    //    printf("handle == %i\n", response->database_handle);

    this->connection->fetch(
        statment_handle->database_handle);
    bytesSent = write(
        this->client_fd,
        this->connection->messagePaddr->payload,
        this->connection->messagePaddr->position);
    log("Sent == %i\n", bytesSent);
    Response *query_resposne = this->ReadResponse();

    delete traansaction_handle;
    delete statment_handle;
    delete statment_response;
    delete database_handle;
    delete query_resposne;

    return 0;
}

int Firebird::Disconnect()
{
    return close(this->client_fd);
}

ResponseData *Firebird::ReadResponseData()
{
    Response *response = this->ReadResponse();
    /* if (!isInstanceOf<ResponseData>(response)){
         printf("Expected response data \n");
         exit(0);
     }*/
    return (ResponseData *)response;
}

Response *Firebird::ReadResponse()
{
    MessageDecoder *decode = new MessageDecoder();
    unsigned char buffer[1024] = {0};
    int length = recv(this->client_fd, buffer, 1024, 0);
    log("Output = %i\n", length);

    decode->decode((unsigned char *)&buffer, length);
    Response *response = decode->opcode();

    /*
    if (isInstanceOf<ResponseData>(response)){
        if (((ResponseData*)response)->is_error){
            printf("Received error, should exiting\n");
            exit(0);
        }
    }
    */

    delete decode;

    return response;
}
