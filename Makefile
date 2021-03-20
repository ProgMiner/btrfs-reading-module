CC = gcc
LD = gcc
CFLAGS = -std=c99 -pedantic-errors -Wall -Werror -O2 # -DBTRFS_DEBUG -g -O0
LFLAGS =

BUILDPATH = build
SOURCES = btrfs.c btrfs_low.c btrfs_chunk_list.c btrfs_traverse_btree.c btrfs_debug.c \
		btrfs_find_in_btree.c lib/crc32c.c \
		struct/btrfs_disk_key.c struct/btrfs_key.c struct/btrfs_key_pointer.c struct/btrfs_item.c
HEADERS = include/btrfs.h btrfs.h btrfs_low.h types.h btrfs_chunk_list.h btrfs_debug.h \
		btrfs_traverse_btree.h btrfs_find_in_btree.h lib/crc32c.h \
		struct/btrfs_block_group_item.h struct/btrfs_chunk.h struct/btrfs_csum_item.h \
		struct/btrfs_dev_extent.h struct/btrfs_dev_item.h struct/btrfs_dev_stats_item.h \
		struct/btrfs_dev_replace_item.h struct/btrfs_balance_item.h struct/btrfs_dir_item.h \
		struct/btrfs_dir_log_item.h struct/btrfs_extent_data_ref.h struct/btrfs_extent_inline_ref.h \
		struct/btrfs_extent_item.h struct/btrfs_extent_item_v0.h struct/btrfs_extent_ref_v0.h \
		struct/btrfs_file_extent_item.h struct/btrfs_free_space_entry.h \
		struct/btrfs_free_space_header.h struct/btrfs_free_space_info.h struct/btrfs_inode_ref.h \
		struct/btrfs_inode_extref.h struct/btrfs_inode_item.h struct/btrfs_header.h \
		struct/btrfs_key.h struct/btrfs_disk_key.h struct/btrfs_root_item.h struct/btrfs_root_ref.h \
		struct/btrfs_root_backup.h struct/btrfs_shared_data_ref.h struct/btrfs_stripe.h \
		struct/btrfs_super_block.h struct/btrfs_timespec.h struct/btrfs_tree_block_info.h \
		struct/btrfs_qgroup_status_item.h struct/btrfs_qgroup_info_item.h \
		struct/btrfs_qgroup_limit_item.h struct/btrfs_key_pointer.h struct/btrfs_item.h
TARGET = libbtrfs.a

TEST_SOURCES = test/test.c
TEST_TARGET = test/test

OBJECTS = $(SOURCES:%.c=$(BUILDPATH)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:%.c=$(BUILDPATH)/%.o)

.PHONY: all build clean test
.SUFFIXES:

all: build

clean:
	@rm -vrf $(BUILDPATH) 2> /dev/null; true
	@rm -v $(TARGET) $(TEST_TARGET) 2> /dev/null; true

build: $(TARGET)

test: test/test
	./test/test testfs

%.c:

$(OBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TEST_OBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	ar rcs $@ $^

$(TEST_TARGET): $(TEST_OBJECTS) $(TARGET)
	$(LD) -o $@ $^ $(LFLAGS)
