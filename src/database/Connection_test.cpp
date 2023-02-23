#include <cheat.h>
#include "Connection.h"
#include "../config.h"
#include <stdio.h>
#include <stdlib.h>

CHEAT_TEST(connection,
    DatabaseConnection *connection = new DatabaseConnection();
    
    cheat_assert(
        strncmp(
            connection->message->dump(),
            "090653595344424108035372700a0f5372702c4c65676163795f4175746807ff00326535366566393061663364313330613165376163613737383636306564393664393434313830333637376166316138643166336238663966653432383139633437346133303963633266633863656536306365313462623739326163373935353438396334613032613162303434346437663461646634663533333731646336333332613561376138663733396465366264393631613431383863633765613234323637633766343863636630613330636465623233303865656536306266656639643565363062633335386161343536376234383862663464653933636464626266373132353537366334613266323933333366613230633530383607030130630b04000000000103632b2b04056c6f63616c06000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
            1024
        ) == 0
    );

    cheat_assert(
        strncmp(
            connection->messagePadder->dumpToPosition(),
            "0000000100000013000000030000000100000004746573740000000400000138090653595344424108035372700a0f5372702c4c65676163795f4175746807ff00326535366566393061663364313330613165376163613737383636306564393664393434313830333637376166316138643166336238663966653432383139633437346133303963633266633863656536306365313462623739326163373935353438396334613032613162303434346437663461646634663533333731646336333332613561376138663733396465366264393631613431383863633765613234323637633766343863636630613330636465623233303865656536306266656639643565363062633335386161343536376234383862663464653933636464626266373132353537366334613266323933333366613230633530383607030130630b04000000000103632b2b04056c6f63616c06000000000a000000010000000200000003000000010000800b000000010000000500000005000000020000800c000000010000000500000005000000030000800d00000001000000050000000500000004",
            848
        ) == 0
    );
)