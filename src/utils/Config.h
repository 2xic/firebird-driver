#ifndef config_H
#define config_H

//#define FIREBIRD_SERVER_PORT 3232
#define FIREBIRD_SERVER_PORT 3050
#define FIREBIRD_SERVER_HOST "127.0.0.1"
#define FIREBIRD_USER "DEMO"
#define FIREBIRD_PASSWORD "demo"
#define FIREBIRD_DATABASE "/test"

#define DEBUG false

#define log(format,args...)        \
                  if (DEBUG) {      \
                      printf(format, ## args);    \
                  }
#endif
