#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ImageHandler.h"
#include "list.h"

//My image format struct (the struct is similar to GIF: header, palette, raw data)
typedef struct {
    int x, y;
    int pSize;  //palette size
    PPMPixel *colors;
    unsigned char *data;
} myImage;
//

void decompressImage(PPMImage *, myImage *);
myImage* readMyImage(const char *);

int main(int argc, char const *argv[]) {
    PPMImage *image;
    myImage *compressedRawImage;

    compressedRawImage = readMyImage(argv[1]);
    fprintf(stderr, "Size: %d * %d with %d colors\n", compressedRawImage->x, compressedRawImage->y, compressedRawImage->pSize);
    decompressImage(image,compressedRawImage);
    return 0;
}

/**
 * This function executes the image decompression
 * @param resultImage: pointer to new PPM image
 * @param image: pointer to compressed image (format: ppmc)
 */
void decompressImage(PPMImage *resultImage, myImage *image) {
	resultImage = (PPMImage *) malloc(sizeof(PPMImage));

	resultImage->x = image->x;
	resultImage->y = image->y;

	resultImage->data = (PPMPixel *) malloc(sizeof(PPMPixel) * image->x * image->y);

	int i,j;
	for(i = 0; i < resultImage->x * resultImage->y; i++){
		j = image->data[i];
		resultImage->data[i].red = image->colors[j].red;
		resultImage->data[i].green = image->colors[j].green;
		resultImage->data[i].blue = image->colors[j].blue;
	}

    writePPM("decompressed.ppm",resultImage);
}

myImage* readMyImage(const char * filename) {
	int c;
	char buff[16];
    myImage *image;
    FILE *fp;

    //open PPMC file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
        }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
        }

    //check the image format
    if (buff[0] != 'C' || buff[1] != 'P' || buff[2] != '6') {
         fprintf(stderr, "Invalid image format  (must be 'CP6')\n");
         exit(1);
    }

    //alloc memory form image
    image = (myImage *) malloc(sizeof(myImage));
    if (!image) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n')
    ;
         c = getc(fp);
    }

    ungetc(c, fp);

    //read image size information
    if (fscanf(fp, "%d %d %d", &image->x, &image->y, &image->pSize) != 3) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;

    //memory allocation for  color palette and data
    image->colors =(PPMPixel*) malloc(sizeof(PPMPixel)*image->pSize);
    image->data = (unsigned char*) malloc(image->x * image->y);


    if (!image) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    fread(image->colors, 3,image->pSize, fp);

    fread(image->data, 1, image->x * image->y, fp);

 	return image;
}
