FPTW_VERSION := 0.1.0
CC ?= gcc

INCLUDES := -I .
CFLAGS := -DDEBUG -O0 -ggdb -Wall -fPIC $(INCLUDES)
# CFLAGS += -fstack-protector-all -D_FORTIFY_SOURCE=2 -fno-strict-overflow
OK=\033[1m\033[32mOK\033[0m

ARCH ?= $(shell uname -m)
BRANCH := $(shell git symbolic-ref HEAD | sed -e 's,.*/\(.*\),\1,')
HASH := $(shell git rev-parse --short HEAD)
VERSION := ${FPTW_VERSION}+${HASH}
PLATFORM := $(shell uname -mors)

LDADD := -lwolfssl

all: lib
	@echo
	@echo "${OK} Built on $(PLATFORM)"
	@echo

lib:
	CC="$(CC)" CFLAGS="$(CFLAGS)" LDADD="$(LDADD)" VERSION="$(VERSION)" ARCH="$(ARCH)" \
	make -C src lib-dynamic

test: CFLAGS := -I src -I test -O0 -ggdb -fPIC
test: SOURCES := test/test-dp3t.c test/codec.c
test:
	$(CC) $(CFLAGS) $(SOURCES) -o test-dp3t src/libfptw-$(ARCH).so $(LDADD) \
		 -DVERSION=\"${VERSION}\" -DPLATFORM="\"${PLATFORM}\""

check: test
	./test-dp3t

gdb: test
	gdb -ex run --args ./test-dp3t

valgrind: test
	valgrind --tool=memcheck ./test-dp3t

clean:
	make -C src clean
	rm -f test/*.o

.PHONY: test
