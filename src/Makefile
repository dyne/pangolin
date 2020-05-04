CC ?= gcc
SOURCES := dp3t.o

all: ${SOURCES}

lib-dynamic: CFLAGS += -fPIE -fPIC -DLIBRARY
lib-dynamic: LDFLAGS += -fPIE -fPIC -shared
lib-dynamic: $(SOURCES)
	${CC} ${CFLAGS} ${SOURCES} -o libfptw-${ARCH}.so ${LDFLAGS} ${LDADD}

clean:
	rm -f *.o
	rm -f *.so

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -DVERSION=\"${VERSION}\"
