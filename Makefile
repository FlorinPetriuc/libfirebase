#
# Copyright (C) 2016 Florin Petriuc. All rights reserved.
# Initial release: Florin Petriuc <petriuc.florin@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version
# 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#

LINKLIBS = -lssl -lpthread -lcrypto
CFLAGS = -c -Wall -Werror -fpic
LIBLDFLAGS = -shared -o
LIBFILES = ./build/obj/dns.o ./build/obj/c_misc.o ./build/obj/c_encryption.o ./build/obj/encryption.o ./build/obj/misc.o ./build/obj/tcp.o ./build/obj/ssl.o ./build/obj/libfirebase.o ./build/obj/log.o ./build/obj/fcm.o ./build/obj/http.o
BINLDFLAGS = -o
BINFILES = ./build/obj/libfirebase_test.o ./build/obj/base64_tests.o ./build/obj/full_fcm_tests.o ./build/obj/http_tests.o ./build/obj/encryption_tests.o $(LIBFILES)

all:$(LIBFILES)
	$(CC) $(LIBLDFLAGS) ./build/lib/libfirebase.so $(LIBFILES) $(LINKLIBS)

unit_tests:$(BINFILES)
	$(CC) $(BINLDFLAGS) ./build/test/libfirebase_test $(BINFILES) $(LINKLIBS)
	
./build/obj/libfirebase.o: ./src/libfirebase.c
	$(CC) $(CFLAGS) ./src/libfirebase.c -o ./build/obj/libfirebase.o

./build/obj/libfirebase_test.o: ./src/libfirebase_test.c
	$(CC) $(CFLAGS) ./src/libfirebase_test.c -o ./build/obj/libfirebase_test.o

./build/obj/base64_tests.o: ./src/tests/base64_tests.c
	$(CC) $(CFLAGS) ./src/tests/base64_tests.c -o ./build/obj/base64_tests.o

./build/obj/full_fcm_tests.o: ./src/tests/full_fcm_tests.c
	$(CC) $(CFLAGS) ./src/tests/full_fcm_tests.c -o ./build/obj/full_fcm_tests.o

./build/obj/http_tests.o: ./src/tests/http_tests.c
	$(CC) $(CFLAGS) ./src/tests/http_tests.c -o ./build/obj/http_tests.o

./build/obj/encryption_tests.o: ./src/tests/encryption_tests.c
	$(CC) $(CFLAGS) ./src/tests/encryption_tests.c -o ./build/obj/encryption_tests.o

./build/obj/fcm.o: ./src/fcm/fcm.c
	$(CC) $(CFLAGS) ./src/fcm/fcm.c -o ./build/obj/fcm.o

./build/obj/http.o: ./src/http/http.c
	$(CC) $(CFLAGS) ./src/http/http.c -o ./build/obj/http.o

./build/obj/ssl.o: ./src/env/linux/ssl.c
	$(CC) $(CFLAGS) ./src/env/linux/ssl.c -o ./build/obj/ssl.o

./build/obj/tcp.o: ./src/env/linux/tcp.c
	$(CC) $(CFLAGS) ./src/env/linux/tcp.c -o ./build/obj/tcp.o

./build/obj/dns.o: ./src/env/linux/dns.c
	$(CC) $(CFLAGS) ./src/env/linux/dns.c -o ./build/obj/dns.o

./build/obj/misc.o: ./src/env/linux/misc.c
	$(CC) $(CFLAGS) ./src/env/linux/misc.c -o ./build/obj/misc.o

./build/obj/c_misc.o: ./src/env/common/misc.c
	$(CC) $(CFLAGS) ./src/env/common/misc.c -o ./build/obj/c_misc.o

./build/obj/encryption.o: ./src/env/linux/encryption.c
	$(CC) $(CFLAGS) ./src/env/linux/encryption.c -o ./build/obj/encryption.o

./build/obj/c_encryption.o: ./src/env/common/encryption.c
	$(CC) $(CFLAGS) ./src/env/common/encryption.c -o ./build/obj/c_encryption.o

./build/obj/log.o: ./src/env/linux/log.c
	$(CC) $(CFLAGS) ./src/env/linux/log.c -o ./build/obj/log.o

install:
	cp ./build/lib/libfirebase.so /usr/lib/libfirebase.so
	cp ./build/include/firebase.h /usr/include/firebase.h

clean:
	rm ./build/obj/*
	rm ./build/lib/*
	rm ./build/test/*