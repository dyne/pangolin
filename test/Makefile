CC := gcc
INCLUDES := -I . -I ..
CFLAGS := -DDEBUG -O0 -ggdb -fPIC $(INCLUDES)
SOURCES := 

all: dp3t
	./dp3t

dp3t: dp3t.o
	gcc $(CFLAGS) dp3t.o ../codec.o -o dp3t -lwolfssl

clean:
	rm -f *.o
	rm -f dp3t
