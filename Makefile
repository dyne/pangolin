FPTW_VERSION := 0.1.0
CC ?= gcc

INCLUDES := -I .
CFLAGS := -DDEBUG -O0 -ggdb -Wall -fPIC $(INCLUDES)
CFLAGS += -fstack-protector-all -D_FORTIFY_SOURCE=2 -fno-strict-overflow

ARCH := $(shell uname -m)
BRANCH := $(shell git symbolic-ref HEAD | sed -e 's,.*/\(.*\),\1,')
HASH := $(shell git rev-parse --short HEAD)
VERSION := ${FPTW_VERSION}+${HASH}

SOURCES := codec.o

LDADD := -lwolfssl

all: ${SOURCES} cli

cli: $(SOURCES) main.o
	$(CC) $(CFLAGS) $(SOURCES) main.o -o fptw $(LDFLAGS) $(LDADD)

redis: CFLAGS += -DLIBRARY
redis: $(SOURCES) redis.o
	$(CC) $(CFLAGS) $(SOURCES) redis.o -shared ${LDADD} -o fptw.so

test: codec.o
	make -C test

clean:
	rm -f *.o
	rm -f fptw
	rm -f fptw.so
	make -C test clean

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -DVERSION=\"${VERSION}\"

.PHONY: test
