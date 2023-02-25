Goal is to create a stable driver than can be compiled wasm.

Uses [Cheat.h](https://github.com/Tuplanolla/cheat) for testing, and OpenSSL for BigNum and hashes.

TODO
- Make it possible to run a select query, and read the response.
- Add AFL to fuzz the project, especially the message decoding logic has to be fuzzed. 

References used
- [node-firebird driver](https://github.com/hgourvest/node-firebird)
- 
