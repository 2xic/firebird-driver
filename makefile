.PHONY: build

build:
	g++ \
		./src/main.cpp \
		./src/serialization/Message.cpp \
		./src/serialization/MessageDecoder.cpp \
		./src/serialization/MessagePaddr.cpp \
		./src/crypto/Srp.cpp \
		./src/database/Connection.cpp \
		./src/database/Firebird.cpp \
		./src/crypto/Sha1.cpp \
		./src/serialization/opcodes/Acceptdata.cpp \
	-I ./src/ \
	-lcrypto \
	-o firebird_cli

test:
	cd src && g++ \
		test.cpp \
		./serialization/Message.cpp \
		./serialization/MessagePaddr.cpp \
		./serialization/MessageDecoder.cpp \
		./crypto/Srp.cpp \
		./crypto/Sha1.cpp \
		./database/Connection.cpp \
		./serialization/opcodes/Acceptdata.cpp \
		-o ../firebird_cli_test -I ./ -lm -lcrypto
	./firebird_cli_test --noisy

fuzz:
	echo "fuzz"

fuzz_build:
	wget -c https://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz
	tar -xf afl-latest.tgz
	cd afl-2.52b && make

docker_run:
	sudo docker container run -p 3050:3050 \
	-it --entrypoint "/bin/bash" \
	-t firebird_db

docker_build:
	sudo docker build -t firebird_db .
