
FUSE_CFLAGS = `pkg-config fuse --cflags`
FUSE_LFLAGS = `pkg-config fuse --libs`

CC = gcc
LD = gcc
CFLAGS = -std=gnu11 -pedantic-errors -Wall -Werror $(FUSE_CFLAGS) -g -O0 # -O2
LFLAGS = $(FUSE_LFLAGS)

BUILDPATH = build
SOURCES = main.c btrfs.c
HEADERS = btrfs.h
TARGET = lab1

OBJECTS = $(SOURCES:%.c=$(BUILDPATH)/%.o)

.PHONY: all build clean
.SUFFIXES:

all: build

clean:
	@rm -vrf $(BUILDPATH) 2> /dev/null; true
	@rm -v $(TARGET) 2> /dev/null; true

build: $(TARGET)

%.c:

$(OBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LFLAGS)
