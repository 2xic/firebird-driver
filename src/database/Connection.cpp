#include "Connection.h"
#include "../serialization/Message.h"
#include "../serialization/MessagePaddr.h"
#include "../crypto/Srp.h"
#include "../utils/Opcodes.h"
#include <string>

DatabaseConnection::DatabaseConnection(
    char* username,
    char* password,
    char* database
)
{
    this->username = username;
    this->password = password;
    this->database = database;

    this->messagePaddr = new MessagePaddr();
    this->message = new Message();
    this->srp = new Srp();
}

int DatabaseConnection::Connect() {
    std::string plugin = "Srp";
    const char *plugin_c = plugin.c_str();

    std::string plugin_list = "Srp,Legacy_Auth";
    const char *plugin_list_c = plugin_list.c_str();
    char*publickey = (char*)this->srp->HexPublicKey();

    write_opcode(this->message, CNCT_login, this->username);
    write_opcode(this->message, CNCT_plugin_name, (char *)plugin_c);
    write_opcode(this->message, CNCT_plugin_list, (char *)plugin_list_c);

    write_multi_block(this->message, publickey);

    // WireCrypt = Disabled
    write_int_opcode(this->message, CNCT_client_crypt, 4);
    this->message->writeInt8(WIRE_CRYPT_DISABLED);
    this->message->writeInt8(0);
    this->message->writeInt8(0);
    this->message->writeInt8(0);

    std::string user = "c++";
    const char *user_c = user.c_str();

    std::string host = "local";
    const char *host_c = host.c_str();
    
    //
    write_opcode(this->message, CNCT_user, (char *)user_c);
    write_opcode(this->message, CNCT_host, (char *)host_c);

    //
    write_int_opcode(this->message, CNCT_user_verification, 0);

    // Message paddr
    this->messagePaddr->write4Bytes(op_connect);
    this->messagePaddr->write4Bytes(op_attach);
    this->messagePaddr->write4Bytes(CONNECT_VERSION3);
    this->messagePaddr->write4Bytes(ARCHITECTURE_GENERIC);

//    const char *database = FIREBIRD_DATABASE;
    this->messagePaddr->writeString((char*)this->database);
    this->messagePaddr->write4Bytes(4);
    this->messagePaddr->writeMessage(message);
    
    for (int i = 0; i < 4; i++) {
        this->messagePaddr->write4Bytes(supported_protocols[i][0]);
        this->messagePaddr->write4Bytes(supported_protocols[i][1]);
        this->messagePaddr->write4Bytes(supported_protocols[i][2]);
        this->messagePaddr->write4Bytes(supported_protocols[i][3]);
        this->messagePaddr->write4Bytes(supported_protocols[i][4]);
    }
    return 0;
}

int DatabaseConnection::Attach(char *auth_data) {
    if (strlen(auth_data) != 40){
        printf("Something is wrong? Got length %li \n", strlen(auth_data));
    }
    // TODO: Should probably resize the buffer
    // Should also move the position variable behind a setter / getter
    this->messagePaddr->position = 0;
    this->message->position = 0;

    this->message->writeInt8(isc_dpb_version1);

    std::string encoding = "UTF8";
    const char *encoding_c = encoding.c_str();
    write_opcode(this->message, isc_dpb_lc_ctype, (char *)encoding_c);

    //std::string username = FIREBIRD_USER;
    //const char *username_c = username.c_str();
    write_opcode(this->message, isc_dpb_user_name, this->username);

    // If role it should be added here
    this->message->writeByte(isc_dpb_process_id);
    this->message->writeByte(4);
    this->message->writeInt32(1234);

    std::string process_name = "test";
    const char *process_name_c = process_name.c_str();
    write_opcode(this->message, isc_dpb_process_name, (char *)process_name_c);
    write_opcode(this->message, isc_dpb_specific_auth_data, toLowerCase(auth_data));

    this->messagePaddr->write4Bytes(op_attach);
    this->messagePaddr->write4Bytes(0);

    // Database name
    log("pos == %i\n", this->messagePaddr->position);
    this->messagePaddr->writeString(this->database);
    log("pos == %i\n", this->messagePaddr->position);

    this->messagePaddr->writeMessage(message);
    /*
    // Then send this message to the server and see what it has to say
*/
    log("full = %s\n", this->messagePaddr->dumpToPosition());
    log("blr = %s\n", this->message->dumpToPosition());

    return 0;
}

void DatabaseConnection::startTransaction(int database_handle) {
    this->messagePaddr->position = 0;
    this->message->position = 0;
    
    this->message->writeByte(ISOLATION_REPEATABLE_READ[0]);
    this->message->writeByte(ISOLATION_REPEATABLE_READ[1]);
    this->message->writeByte(ISOLATION_REPEATABLE_READ[2]);
    this->message->writeByte(ISOLATION_REPEATABLE_READ[3]);
    
    this->messagePaddr->write4Bytes(op_transaction);
    this->messagePaddr->write4Bytes(database_handle);
    this->messagePaddr->writeMessage(this->message);
}



void DatabaseConnection::prepareStatment(int database_handle, int transaction_handle, char* query) {
    this->messagePaddr->position = 0;
    this->message->position = 0;

    int dialect = 3;
    int buffer_length = 65535;

    this->messagePaddr->write4Bytes(op_allocate_statement);
    this->messagePaddr->write4Bytes(database_handle);

    for(int i = 0; i < 20; i++){
        message->writeByte(DESCRIBE[i]);
    }

    this->messagePaddr->write4Bytes(op_prepare_statement);
    this->messagePaddr->write4Bytes(transaction_handle);
    this->messagePaddr->write4Bytes(0xFFFF);
    this->messagePaddr->write4Bytes(dialect);
    this->messagePaddr->writeString(query);
    this->messagePaddr->writeMessage(message);
    this->messagePaddr->write4Bytes(buffer_length);
}

void DatabaseConnection::executeStatment(int database_handle, int transaction_handle, int statment_handle) {
    this->messagePaddr->position = 0;
    this->message->position = 0;

    int dialect = 3;
    int buffer_length = 65535;

    this->messagePaddr->write4Bytes(op_execute);
    this->messagePaddr->write4Bytes(statment_handle);
    this->messagePaddr->write4Bytes(transaction_handle);

    this->messagePaddr->writeMessage(message);
    this->messagePaddr->write4Bytes(0);
    this->messagePaddr->write4Bytes(0);
}


void DatabaseConnection::fetch(int statment_handle){
    this->messagePaddr->position = 0;
    this->message->position = 0;

    int dialect = 3;
    int buffer_length = 65535;

    this->messagePaddr->write4Bytes(op_fetch);
    this->messagePaddr->write4Bytes(statment_handle);
    /*
        TODO: This should be automated based on the statment response
    */
    message->writeByte(blr_version5);
    message->writeByte(blr_begin);
    message->writeByte(blr_message);
    message->writeByte(0);

    // WORD
    message->writeInt16(4);

    // SQLVarInt
    message->writeByte(blr_long);
    message->writeInt8(0);
    message->writeByte(blr_short);
    message->writeByte(0);

    // SQLVARTEXT
    message->writeByte(blr_text);
    message->writeInt16(4);
    message->writeByte(blr_short);
    message->writeByte(0);

    message->writeByte(blr_end);
    message->writeByte(blr_eoc);

    this->messagePaddr->writeMessage(message);

    this->messagePaddr->write4Bytes(0);
    this->messagePaddr->write4Bytes(200);
}

void DatabaseConnection::Query(int database_handle, char *query){

}
