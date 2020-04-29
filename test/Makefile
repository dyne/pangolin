CC ?= gcc
INCLUDES := -I . -I ../src
CFLAGS := -DDEBUG -O0 -ggdb -fPIC $(INCLUDES)
ARCH ?= $(shell uname -m)

all: dp3t
	./dp3t

dp3t: dp3t.o
	gcc $(CFLAGS) dp3t.o -o dp3t ../src/libfptw-$(ARCH).so

clean:
	rm -f *.o
	rm -f dp3t

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ -DVERSION=\"${VERSION}\" -DPLATFORM="\"${PLATFORM}\""
