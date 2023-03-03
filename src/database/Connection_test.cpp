#include "Connection.h"
#include "../utils/Config.h"
#include <cheat.h>
#include <stdio.h>
#include <stdlib.h>

CHEAT_TEST(connection,
    const char username[] = "SYSDBA";
    const char password[] = "masterkey";
    const char database[] = "test";
    DatabaseConnection *connection = new DatabaseConnection(
        (char*)username,
        (char*)password,
        (char*)database
    );
    connection->Connect();
    
    cheat_assert(
        strncmp(
            connection->message->dump(),
            "090653595344424108035372700a0f5372702c4c65676163795f4175746807ff00326535366566393061663364313330613165376163613737383636306564393664393434313830333637376166316138643166336238663966653432383139633437346133303963633266633863656536306365313462623739326163373935353438396334613032613162303434346437663461646634663533333731646336333332613561376138663733396465366264393631613431383863633765613234323637633766343863636630613330636465623233303865656536306266656639643565363062633335386161343536376234383862663464653933636464626266373132353537366334613266323933333366613230633530383607030130630b04000000000103632b2b04056c6f63616c06000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
            1024
        ) == 0
    );

    cheat_assert(
        strncmp(
            connection->messagePaddr->dumpToPosition(),
            "0000000100000013000000030000000100000004746573740000000400000138090653595344424108035372700a0f5372702c4c65676163795f4175746807ff00326535366566393061663364313330613165376163613737383636306564393664393434313830333637376166316138643166336238663966653432383139633437346133303963633266633863656536306365313462623739326163373935353438396334613032613162303434346437663461646634663533333731646336333332613561376138663733396465366264393631613431383863633765613234323637633766343863636630613330636465623233303865656536306266656639643565363062633335386161343536376234383862663464653933636464626266373132353537366334613266323933333366613230633530383607030130630b04000000000103632b2b04056c6f63616c06000000000a000000010000000200000003000000010000800b000000010000000500000005000000020000800c000000010000000500000005000000030000800d00000001000000050000000500000004",
            848
        ) == 0
    );

    delete connection;
)


CHEAT_TEST(connection_attach,
    const char username[] = "demo";
    const char password[] = "demo";
    const char database[] = "/test";
    DatabaseConnection *connection = new DatabaseConnection(
        (char*)username,
        (char*)password,
        (char*)database
    );
    char data[] = "4d30b86d0e3cda4019d9a63fd098789e571381d3";
    connection->Attach(data);
    cheat_assert(
        strncmp(
            connection->messagePaddr->dumpToPosition(),
            "0000001300000000000000052f7465737400000000000043013004555446381c0464656d6f4704d20400004a047465737454283464333062383664306533636461343031396439613633666430393837383965353731333831643300",
            184
        ) == 0
    );

    delete connection;
)


CHEAT_TEST(connection_start_transaction,
    const char username[] = "demo";
    const char password[] = "demo";
    const char database[] = "/test";
    DatabaseConnection *connection = new DatabaseConnection(
        (char*)username,
        (char*)password,
        (char*)database
    );
    connection->startTransaction(0);
    cheat_assert(
        strncmp(
            connection->messagePaddr->dumpToPosition(),
            "0000001d000000000000000403090602",
            32
        ) == 0
    );

    delete connection;
)


CHEAT_TEST(connection_prepare_statment,
    const char username[] = "demo";
    const char password[] = "demo";
    const char database[] = "/test";
    const char query[] = "SELECT * FROM MYDATA";
    DatabaseConnection *connection = new DatabaseConnection(
        (char*)username,
        (char*)password,
        (char*)database
    );
    connection->prepareStatment(0, 1, (char*)query);
    cheat_assert(
        strncmp(
            connection->messagePaddr->dumpToPosition(),
            "0000003e0000000000000044000000010000ffff000000030000001453454c454354202a2046524f4d204d594441544100000014150407090b0c0d0e101113080507090b0c0d0e080000ffff",
            152
        ) == 0
    );

    delete connection;
)

CHEAT_TEST(connection_execute_statment,
    const char username[] = "demo";
    const char password[] = "demo";
    const char database[] = "/test";
    const char query[] = "SELECT * FROM MYDATA";
    DatabaseConnection *connection = new DatabaseConnection(
        (char*)username,
        (char*)password,
        (char*)database
    );
    connection->executeStatment(0, 1, 2);
    cheat_assert(
        strncmp(
            connection->messagePaddr->dumpToPosition(),
            "0000003f0000000200000001000000000000000000000000",
            48
        ) == 0
    );
    delete connection;
)


CHEAT_TEST(connection_execute_fetch,
    const char username[] = "demo";
    const char password[] = "demo";
    const char database[] = "/test";
    const char query[] = "SELECT * FROM MYDATA";
    DatabaseConnection *connection = new DatabaseConnection(
        (char*)username,
        (char*)password,
        (char*)database
    );
    connection->fetch(2);
    cheat_assert(
        strncmp(
            connection->messagePaddr->dumpToPosition(),
            "000000410000000200000011050204000400080007000e04000700ff4c00000000000000000000c8",
            80
        ) == 0
    );
    delete connection;
)
