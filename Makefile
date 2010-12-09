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

SOURCES_PROT  = src/protocol/start.c src/protocol/reqres.c
HEADERS_PROT  = src/protocol/protocol.h src/protocol/io.h
OBJECTS_PROT  = $(SOURCES_PROT:.c=.o)

SOURCES_SESS  = src/session/init.c
HEADERS_SESS  = src/session/session.h
OBJECTS_SESS  = $(SOURCES_PROT:.c=.o)

ALL_OBJECTS  = $(OBJECTS_MAIN) $(OBJECTS_CONN) $(OBJECTS_TLS) $(OBJECTS_PROT) $(OBJECTS_SESS)

dbxml_server: $(ALL_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJECTS) $(LIBS)

$(ALL_OBJECTS): $(HEADERS_MAIN)

$(OBJECTS_MAIN): $(HEADERS_CONN) $(HEADERS_TLS)

$(OBJECTS_CONN): $(HEADERS_TLS)

$(OBJECTS_TLS): $(HEADERS_PROT)

$(OBJECTS_PROT): $(HEADERS_TLS) $(HEADERS_SESSION)

$(OBJECTS_SESS): $(HEADERS_PROT)

clean:
	rm -fv dbxml_server $(ALL_OBJECTS)
