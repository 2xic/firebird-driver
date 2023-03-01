{
    "targets": [
        {
            "target_name": "addon",
            "sources": [
                "./src/addon.cpp",
                "src/serialization/Message.cpp",
                "src/serialization/MessageDecoder.cpp",
                "src/serialization/MessagePaddr.cpp",
                "src/crypto/Srp.cpp",
                "src/database/Connection.cpp",
                "src/database/Firebird.cpp",
                "src/crypto/Sha1.cpp",
                "src/serialization/opcodes/Acceptdata.cpp",
                "src/serialization/opcodes/Responsedata.cpp"
            ]
        }
    ]
}
