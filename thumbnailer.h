//
// Created by howard on 05/04/2020.
//

#ifndef TMBNL_THUMBNAILER_H
#define TMBNL_THUMBNAILER_H

#include <dirent.h>
#include <sys/stat.h>
#include <magick/MagickCore.h>


typedef struct {

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    ExceptionInfo *exception;
    Image *image, *images, *resize_image, *thumbnails;
    ImageInfo *image_info;

} Thumbnail_dir;

Thumbnail_dir* new_working_dir(char *path);

int process_thumbnails(Thumbnail_dir *working_dir);

void tidy_up(Thumbnail_dir *working_dir);

#endif //TMBNL_THUMBNAILER_H
