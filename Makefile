CC := cc
# CC := gcc # for wingnuts
CFLAGS := -O2 -std=c99 -Iinc
CPPFLAGS := -std=gnu11 -Iinc

CPPFLAGS := $(CPPFLAGS)

#CFLAGS := $(CFLAGS) -ggdb
CFLAGS := $(CFLAGS) -O2

LDFLAGS :=
LIBRARIES := -pthread
SOURCES := $(wildcard src/*.c) 
OBJECTS := $(SOURCES:.c=.o)
EDITOR_EXEC := p4

.PHONY: all clean

all: $(EDITOR_EXEC)

clean:
	$(RM) $(OBJECTS) $(EDITOR_EXEC)

$(EDITOR_EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o $@ $(LIBRARIES)

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
