
FUSE_CFLAGS = `pkg-config fuse --cflags`
FUSE_LFLAGS = `pkg-config fuse --libs`

CC = gcc
LD = gcc
CFLAGS = -std=gnu11 -pedantic-errors -Wall -Werror $(FUSE_CFLAGS) -Ibtrfs/include -g -O0 # -O2
LFLAGS = $(FUSE_LFLAGS)

BUILDPATH = build
SOURCES = main.c
HEADERS = btrfs/include/btrfs.h
TARGET = lab1

OBJECTS = $(SOURCES:%.c=$(BUILDPATH)/%.o)

.PHONY: all build clean libbtrfs
.SUFFIXES:

all: build

clean:
	@+cd btrfs; $(MAKE) clean
	@rm -vrf $(BUILDPATH) 2> /dev/null; true
	@rm -v $(TARGET) 2> /dev/null; true

build: $(TARGET)

libbtrfs:
	@+cd btrfs; $(MAKE)

btrfs/libbtrfs.a: libbtrfs

%.c:

$(OBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS) btrfs/libbtrfs.a
	$(LD) -o $@ $^ $(LFLAGS)
