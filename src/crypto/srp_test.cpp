#include <cheat.h>
#include "srp.h"
#include "../config.h"
#include "../utils.h"

CHEAT_TEST(srp_encoding,
    Srp *message = new Srp();
    cheat_assert(strncmp(
        "2e56ef90af3d130a1e7aca778660ed96d9441803677af1a8d1f3b8f9fe42819c474a309cc2fc8cee60ce14bb792ac7955489c4a02a1b0444d7f4adf4f53371dc6332a5a7a8f739de6bd961a4188cc7ea24267c7f48ccf0a30cdeb2308eee60bfef9d5e60bc358aa4567b488bf4de93cddbbf7125576c4a2f29333fa20c50860c",
        toLowerCase(message->HexPublicKey()),
        256
    ) == 0);
)
