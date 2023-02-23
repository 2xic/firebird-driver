#include "Connection.h"
#include "../message.h"
#include "../MessagePadder.h"
#include "../crypto/srp.h"
#include <string>

DatabaseConnection::DatabaseConnection()
{
    this->messagePadder = new MessagePadder();
    this->message = new Message();
    this->srp = new Srp();

    std::string username = "SYSDBA";
    const char *username_c = username.c_str();

    std::string plugin = "Srp";
    const char *plugin_c = plugin.c_str();

    std::string plugin_list = "Srp,Legacy_Auth";
    const char *plugin_list_c = plugin_list.c_str();
    char*publickey = (char*)this->srp->HexPublicKey();

    write_opcode(this->message, CNCT_login, (char *)username_c);
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

    // Message padder
    this->messagePadder->write4Bytes(op_connect);
    this->messagePadder->write4Bytes(op_attach);
    this->messagePadder->write4Bytes(CONNECT_VERSION3);
    this->messagePadder->write4Bytes(ARCHITECTURE_GENERIC);

    const char *database = "test\0";

    this->messagePadder->writeString((char*)database);
    this->messagePadder->write4Bytes(4);
    this->messagePadder->writeMessage(message);
    
    for (int i = 0; i < 4; i++) {
        this->messagePadder->write4Bytes(suppoerted_protocols[i][0]);
        this->messagePadder->write4Bytes(suppoerted_protocols[i][1]);
        this->messagePadder->write4Bytes(suppoerted_protocols[i][2]);
        this->messagePadder->write4Bytes(suppoerted_protocols[i][3]);
        this->messagePadder->write4Bytes(suppoerted_protocols[i][4]);
    }
}
