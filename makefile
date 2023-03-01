.PHONY: build

build:
	g++ -Wno-deprecated-declarations \
		./src/main.cpp \
		./src/serialization/Message.cpp \
		./src/serialization/MessageDecoder.cpp \
		./src/serialization/MessagePaddr.cpp \
		./src/crypto/Srp.cpp \
		./src/database/Connection.cpp \
		./src/database/Firebird.cpp \
		./src/crypto/Sha1.cpp \
		./src/serialization/opcodes/Acceptdata.cpp \
		./src/serialization/opcodes/Responsedata.cpp \
	-I ./src/ \
	-lcrypto \
	-o firebird_cli

leaks:
	g++ -Wno-deprecated-declarations \
		./src/leaks.cpp \
		./src/serialization/Message.cpp \
		./src/serialization/MessageDecoder.cpp \
		./src/serialization/MessagePaddr.cpp \
		./src/crypto/Srp.cpp \
		./src/database/Connection.cpp \
		./src/database/Firebird.cpp \
		./src/crypto/Sha1.cpp \
		./src/utils/Profile.cpp \
		./src/serialization/opcodes/Acceptdata.cpp \
		./src/serialization/opcodes/Responsedata.cpp \
	-I ./src/ \
	-lcrypto \
	-ggdb3 \
	-o firebird_leaks
	#./firebird_leaks
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			./firebird_leaks
test:
	cd src && g++ -Wno-deprecated-declarations \
		test.cpp \
		./serialization/Message.cpp \
		./serialization/MessagePaddr.cpp \
		./serialization/MessageDecoder.cpp \
		./crypto/Srp.cpp \
		./crypto/Sha1.cpp \
		./database/Connection.cpp \
		./serialization/opcodes/Acceptdata.cpp \
		./serialization/opcodes/Responsedata.cpp \
		-o ../firebird_cli_test -I ./ -lm -lcrypto 
	./firebird_cli_test --noisy

run: build	
	./firebird_cli

fuzz:
	echo "fuzz"
		./afl/afl-g++ -Wno-deprecated-declarations \
		./src/fuzzing/FuzzMessageDecoder.cpp \
		./src/serialization/Message.cpp \
		./src/serialization/MessageDecoder.cpp \
		./src/serialization/MessagePaddr.cpp \
		./src/crypto/Srp.cpp \
		./src/database/Connection.cpp \
		./src/database/Firebird.cpp \
		./src/crypto/Sha1.cpp \
		./src/utils/Profile.cpp \
		./src/serialization/opcodes/Acceptdata.cpp \
		./src/serialization/opcodes/Responsedata.cpp \
	-I ./src/ \
	-lcrypto \
	-o FuzzMessageDecoder
	# ./FuzzMessageDecoder
	./afl/afl-fuzz -i ./afl/input/ -o ./afl/output/ -- ./FuzzMessageDecoder
	 @@

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

node_addon:
	#node-gyp configure 
	node-gyp build

delta: build node_addon
	TIMEFORMAT='%3lR' /usr/bin/time -p  ./firebird_cli
	TIMEFORMAT='%3lR' /usr/bin/time -p  node ./ref/node-firebird/example.js
	TIMEFORMAT='%3lR' /usr/bin/time -p  node test.js
