#include "database/Firebird.h"

int main(int argc, char const* argv[])
{
    Firebird *cli = new Firebird();
    cli->Connect();
    cli->Query();
    cli->Disconnect();

    delete cli;

    return 0;
}
