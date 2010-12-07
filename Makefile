CC=gcc
CFLAGS=-Wall -pthread `pkg-config gnutls --cflags` -g
LDFLAGS=-pthread `pkg-config gnutls --libs`
LIBS=

SOURCES_MAIN = src/main/main.c src/main/options.c
HEADERS_MAIN = src/main/main.h src/main/options.h src/main/session.h
OBJECTS_MAIN = $(SOURCES_MAIN:.c=.o)

SOURCES_CONN = src/conn/bind.c src/conn/handle_client.c
HEADERS_CONN = src/conn/bind.h src/conn/handle_client.h
OBJECTS_CONN = $(SOURCES_CONN:.c=.o)

SOURCES_TLS  = src/tls/start.c src/tls/init.c src/tls/io.c
HEADERS_TLS  = src/tls/start.h src/tls/init.h src/tls/io.h
OBJECTS_TLS  = $(SOURCES_TLS:.c=.o)

SOURCES_PROT  = src/protocol/protocol.c
HEADERS_PROT  = src/protocol/protocol.h
OBJECTS_PROT  = $(SOURCES_PROT:.c=.o)

ALL_OBJECTS  = $(OBJECTS_MAIN) $(OBJECTS_CONN) $(OBJECTS_TLS) $(OBJECTS_PROT)

dbxml_server: $(ALL_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJECTS) $(LIBS)

$(ALL_OBJECTS): $(HEADERS_MAIN)

$(OBJECTS_MAIN): $(HEADERS_CONN) $(HEADERS_TLS)

$(OBJECTS_CONN): $(HEADERS_TLS)

$(OBJECTS_TLS): $(HEADERS_PROT)

$(OBJECTS_PROT): $(HEADERS_TLS)

clean:
	rm -fv dbxml_server $(ALL_OBJECTS)
