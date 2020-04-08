//
// Created by howard on 05/04/2020.
//

#include "thumbnailer.h"
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>
#include <string.h>

#define RESIZE_PERCENTAGE 10


void create_dest_folder(char *dest) {
    mkdir(dest, 0755);
};

Thumbnail_dir* new_working_dir(char *path) {
    Thumbnail_dir* working_dir = malloc(sizeof(Thumbnail_dir));
    if((working_dir->dp = opendir(path)) == NULL) {
        printf("Error opening: %s\n", path);
        exit(EXIT_FAILURE);
    }
    chdir(path);
    create_dest_folder("Thumbs");

    MagickCoreGenesis(path, MagickFalse);
    working_dir->exception = AcquireExceptionInfo();

    return working_dir;
}

int process_thumbnails(Thumbnail_dir *working_dir) {
    int processed = 0;
    working_dir->image_info = CloneImageInfo( (ImageInfo *) NULL);

    while((working_dir->entry = readdir(working_dir->dp)) != NULL) {
        lstat(working_dir->entry->d_name, &working_dir->statbuf);

        if(S_ISDIR(working_dir->statbuf.st_mode)) {
            if(strcmp(".", working_dir->entry->d_name) == 0 || strcmp("..", working_dir->entry->d_name) == 0)
                continue;
            printf("Ignoring dir: %s/\n",working_dir->entry->d_name);
        }
        else {
            printf("%s\n",working_dir->entry->d_name);

            // image processing code here....
            (void) strcpy(working_dir->image_info->filename, working_dir->entry->d_name);
            working_dir->images = ReadImages(working_dir->image_info, working_dir->exception);

            if(working_dir->exception->severity != UndefinedException) {
                CatchException(working_dir->exception);
            }
            if(working_dir->images == (Image *) NULL) {
                printf("*** No images found ****\n");
                tidy_up(working_dir);
                exit(1);
            }

            //begin converting process
            working_dir->thumbnails = NewImageList();

            while ( (working_dir->image = RemoveFirstImageFromList(&working_dir->images)) != (Image *) NULL ) {
                working_dir->resize_image = ResizeImage(working_dir->image, working_dir->image->columns / 100 * RESIZE_PERCENTAGE, working_dir->image->rows / 100 * RESIZE_PERCENTAGE, Lanczos2Filter, 1.0, working_dir->exception);
                if (working_dir->resize_image == (Image *) NULL) {
                    MagickError(working_dir->exception->severity, working_dir->exception->reason, working_dir->exception->description);
                }
                (void) AppendImageToList(&working_dir->thumbnails, working_dir->resize_image);
                DestroyImage(working_dir->image);
            }

            char dest[250] = "Thumbs/";
            strcat(dest, working_dir->entry->d_name);
            printf("Writing to : %s\n", dest);

            (void) strcpy(working_dir->thumbnails->filename, dest);
            WriteImage(working_dir->image_info, working_dir->thumbnails);

            //inc counter
            ++processed;

        }
    }
    chdir("..");
    working_dir->thumbnails = DestroyImageList(working_dir->thumbnails);
    working_dir->image_info = DestroyImageInfo(working_dir->image_info);
    working_dir->exception = DestroyExceptionInfo(working_dir->exception);
    return processed;
}

void tidy_up(Thumbnail_dir *working_dir) {
    closedir(working_dir->dp);
    MagickCoreTerminus();
}