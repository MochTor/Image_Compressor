#include <stdio.h>
#include "ImageHandler.h"

image *openImage(const char *imageName) {
    FILE *fp;
    char imageData[20];
    fp = fopen(imageName, "rb");    //I use rb mode because the file it's a non-text file
    if (fp == NULL) {
        fprintf(stderr, "Error opening the image. Try again");
        exit(1);
    }
}
