#include "btrfs_fuse.h"


int main(int argc, char * argv[]) {
    return btrfs_fuse_main("testfs", argc, argv);
}
