CC=gcc
CFLAGS=-Wall
LDFLAGS=
LIBS=

SOURCES_MAIN = src/main/main.c src/main/options.c
HEADERS_MAIN = src/main/main.h src/main/options.h
OBJECTS_MAIN = $(SOURCES_MAIN:.c=.o)

dbxml_server: $(OBJECTS_MAIN)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS_MAIN) $(LIBS)

$(OBJECTS_MAIN): $(HEADERS_MAIN)

clean:
	rm -fv dbxml_server $(OBJECTS_MAIN)
