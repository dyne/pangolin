FPTW_VERSION := 0.1.0
CC ?= gcc

INCLUDES := -I .
CFLAGS := -DDEBUG -O0 -ggdb -Wall -fPIC $(INCLUDES)
# CFLAGS += -fstack-protector-all -D_FORTIFY_SOURCE=2 -fno-strict-overflow

ARCH ?= $(shell uname -m)
BRANCH := $(shell git symbolic-ref HEAD | sed -e 's,.*/\(.*\),\1,')
HASH := $(shell git rev-parse --short HEAD)
VERSION := ${FPTW_VERSION}+${HASH}
PLATFORM := $(shell uname -mors)

LDADD := -lwolfssl

all: lib
	@echo
	@echo "Built on $(PLATFORM)"
	@echo

lib:
	CC="$(CC)" CFLAGS="$(CFLAGS)" LDADD="$(LDADD)" VERSION="$(VERSION)" ARCH="$(ARCH)" \
	make -C src lib-dynamic

test: CFLAGS := -I . -I src -fPIC
test:
	$(CC) $(CFLAGS) test/test-dp3t.c -o test-dp3t src/libfptw-$(ARCH).so \
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
