#include "database/Firebird.h"

int main(int argc, char const* argv[])
{
    Firebird *cli = new Firebird();
    int status = cli->Connect();

    printf("Did I connect ? status %i\n", status);

    return 0;
}
