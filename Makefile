CC=gcc
CFLAGS=-Wall
LDFLAGS=
LIBS=

SOURCES_MAIN = src/main/main.c src/main/options.c
HEADERS_MAIN = src/main/main.h src/main/options.h
OBJECTS_MAIN = $(SOURCES_MAIN:.c=.o)

SOURCES_CONN = src/conn/bind.c src/conn/handle_client.c
HEADERS_CONN = src/conn/bind.h src/conn/handle_client.h
OBJECTS_CONN = $(SOURCES_CONN:.c=.o)

ALL_OBJECTS  = $(OBJECTS_MAIN) $(OBJECTS_CONN)

dbxml_server: $(ALL_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJECTS) $(LIBS)

$(ALL_OBJECTS): $(HEADERS_MAIN)

$(OBJECTS_MAIN): $(HEADERS_CONN)

clean:
	rm -fv dbxml_server $(ALL_OBJECTS)
