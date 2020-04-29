CC ?= gcc
SOURCES := codec.o dp3t.o hashdict.o

all: ${SOURCES} cli

cli: $(SOURCES) cli.o
	$(CC) $(CFLAGS) $(SOURCES) cli.o -o fptw $(LDFLAGS) $(LDADD)

lib-dynamic: CFLAGS += -fPIE -fPIC -DLIBRARY
lib-dynamic: LDFLAGS += -fPIE -fPIC -shared
lib-dynamic: $(SOURCES)
	${CC} ${CFLAGS} ${SOURCES} -o libfptw-${ARCH}.so ${LDFLAGS} ${LDADD}


redis: CFLAGS += -DLIBRARY
redis: $(SOURCES) redis.o
	$(CC) $(CFLAGS) $(SOURCES) redis.o -shared ${LDADD} -o fptw.so

clean:
	rm -f *.o
	rm -f *.so

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -DVERSION=\"${VERSION}\"
