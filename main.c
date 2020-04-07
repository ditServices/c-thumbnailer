#include <stdio.h>
#include <stdlib.h>

#include "thumbnailer.h"

int main(int argc, char * argv[]) {

    if( argc < 2 ) {
        printf("Usage: [directory path]\n");
        exit(EXIT_FAILURE);
    }

    Thumbnail_dir* working_dir = new_working_dir(argv[1]);
    int processed = process_thumbnails(working_dir);
    printf("Processed %d images\n", processed);
    tidy_up(working_dir);

    return 0;
}
