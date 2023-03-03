Goal was to create a poc driver than can be compiled ~~wasm~~ native node addon (for speed).

Wasm was dropped because building openssl was tedious, also looks like it might be [slower](https://nickb.dev/blog/wasm-and-native-node-module-performance-comparison/)

*Might* revisit this project later to complete the driver.

## TODO
- ~~Make it possible to run a mvp select query, and read the response~~
- ~~Run valgrind to fix potentional memory leaks~~
- ~~Add AFL to fuzz the project, especially the message decoding logic has to be fuzzed. ~~
- Complete the implementation ?  

## Dependencies
We use [cheat.h](https://github.com/Tuplanolla/cheat) for testing, and OpenSSL for BigNum + hashes.

## Speed 
Plain c++
```
TIMEFORMAT='%3lR' /usr/bin/time -p  ./firebird_cli
real 0.03
user 0.00
sys 0.00
```

The c++ running as a native node addon (from javascript)
```
TIMEFORMAT='%3lR' /usr/bin/time -p  node test.js
real 0.07
user 0.04
sys 0.01
```

Reference client (javascript)
```
TIMEFORMAT='%3lR' /usr/bin/time -p  node ./ref/node-firebird/example.js
real 0.30
user 0.12
sys 0.01
```

## References used
- [node-firebird driver](https://github.com/hgourvest/node-firebird)

