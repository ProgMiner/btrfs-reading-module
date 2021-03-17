#include "btrfs_fuse.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


enum action {
    ACTION_LIST, ACTION_FUSE, ACTION_HELP
};

static enum action parse_args(int argc, char * argv[], const char ** filename) {
    if (argc < 2) {
        return ACTION_LIST;
    }

    if (strcmp(argv[1], "-h") == 0
        || strcmp(argv[1], "--help") == 0) {
        return ACTION_HELP;
    }

    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == '\0') {
        *filename = NULL;
    } else {
        *filename = argv[1];
    }

    return ACTION_FUSE;
}

static char * __list_drives_search_name(char * line) {
    size_t length = strlen(line);
    bool text = false;
    int i;

    for (i = length - 1; i > 0; --i) {
        if (!text) {
            if (!isspace(line[i])) {
                line[i + 1] = '\0';
                text = true;
            }
        } else {
            if (isspace(line[i])) {
                return line + i + 1;
            }
        }
    }

    return NULL;
}

static bool __list_drives_check_partition(const char * name) {
    size_t path_length = 7 + strlen(name);
    bool result;

    char * path = malloc(path_length * sizeof(char));
    if (!path) {
        return false;
    }

    snprintf(path, path_length, "/proc/%s", name);
    result = access(path, F_OK) == 0;

    free(path);
    return result;
}

static int list_drives() {
    FILE * file = fopen("/proc/partitions", "r");
    char * buffer = malloc(256 * sizeof(char));
    size_t buffer_length = 256;
    const char * name;
    int i;

    if (!file) {
        perror("Cannot retrieve list of partitions");
        return -errno;
    }

    printf("List of available partitions:\n");
    for (i = -1; getline(&buffer, &buffer_length, file) > 0; ++i) {
        if (i < 1) {
            continue;
        }

        name = __list_drives_search_name(buffer);
        if (!name || __list_drives_check_partition(name)) {
            --i;
            continue;
        }

        printf("%d. /dev/%s\n", i, name);
    }

    return 0;
}

int main(int argc, char * argv[]) {
    const char * filename;

    switch (parse_args(argc, argv, &filename)) {
    case ACTION_LIST:
        return list_drives();

    case ACTION_FUSE:
        return btrfs_fuse_main(filename, argc - 1, argv + 1);

    case ACTION_HELP:
        printf("Usage: %s <BTRFS image> <FUSE args...>\n", argv[0]);
        printf("       %s -h or %s --help\n", argv[0], argv[0]);
        printf("       %s -\n", argv[0]);
        printf("       %s\n", argv[0]);
        printf("\n"
               "  - First case mounts BTRFS image as FUSE file system\n"
               "  - Second case shows this message\n"
               "  - Third case shows FUSE help\n"
               "  - Fourth case shows list of OS partitions\n"
               "\n"
               "Do not trust FUSE help in usage part!\n");

        return 0;
    }
}
