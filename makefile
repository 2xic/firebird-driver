.PHONY: build

build:
	g++ src/main.cpp src/message.cpp ./src/serialization/MessagePadder.cpp ./src/crypto/srp.cpp ./src/database/Connection.cpp ./src/database/Firebird.cpp \
	-I ./src/ \
	-lcrypto \
	-o firebird_cli

test:
	cd src && g++ test.cpp message.cpp ./serialization/MessagePadder.cpp ./crypto/srp.cpp ./database/Connection.cpp -o ../firebird_cli_test -I ./ -lm -lcrypto
	./firebird_cli_test --noisy

docker_run:
	sudo docker container run -p 3050:3050 \
	-it --entrypoint "/bin/bash" \
	-t firebird_db

docker_build:
	sudo docker build -t firebird_db .
