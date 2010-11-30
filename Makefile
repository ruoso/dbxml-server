CC=gcc
CFLAGS=-Wall
LDFLAGS=
LIBS=

SOURCES_MAIN = src/main/main.c
ALL_SOURCES  = $(SOURCES_MAIN)
ALL_OBJECTS  = $(ALL_SOURCES:.c=.o)

dbxml_server: $(ALL_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJECTS) $(LIBS)

src/main/dbxml_server: src/main/dbxml_server.o

clean:
	rm -fv dbxml_server $(ALL_OBJECTS)
