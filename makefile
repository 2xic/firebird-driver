.PHONY: build

build:
	g++ \
		src/main.cpp src/message.cpp \
		./src/serialization/MessageDecoder.cpp \
		./src/serialization/MessagePadder.cpp \
		./src/crypto/srp.cpp \
		./src/database/Connection.cpp \
		./src/database/Firebird.cpp \
		./src/crypto/sha1.cpp \
		./src/serialization/opcodes/Acceptdata.cpp \
	-I ./src/ \
	-lcrypto \
	-o firebird_cli

test:
	cd src && g++ \
		test.cpp \
		message.cpp \
		./serialization/MessagePadder.cpp \
		./serialization/MessageDecoder.cpp \
		./crypto/srp.cpp \
		./crypto/sha1.cpp \
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
