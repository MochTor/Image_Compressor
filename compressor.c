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

void compressImage(PPMImage *, myImage *, int);
int colorList(PPMImage *);
int maxFrequency(struct color*);
int* frequencyVector(struct color *el);
double findDistance(PPMPixel,PPMPixel);
double findDistanceC(struct color *, PPMPixel);

int main(int argc, char const *argv[]) {
    PPMImage *image;
    myImage *compressedRawImage;
    int colorNumber;
    image = readPPM(argv[1]);
    fprintf(stderr, "PPM image properly read\n");
    fprintf(stderr, "Analyzing image\n");
    colorNumber = colorList(image);
    //fprintf(stderr, "Colors: #%d\n", colorNumber);  //Tested with GIMP color analizer: it works properly
    compressImage(image, compressedRawImage, colorNumber);
    printf("Compression ended\n");
    return 0;
}

/**
 * This function executes the image compression
 * @param image: pointer to original PPM image
 * @param resultImage: pointer to compressed image
 * @param colorNumber: number of colors in the original image
 */
void compressImage(PPMImage *image, myImage *resultImage, int colorNumber) {
    int paletteSize;
    struct color *colorPointer = returnHead();  //Initialized to the head of the palette
    int i = 0;
    int *frequencies = NULL;

    //Allocating resulting image data
    resultImage = (myImage *) malloc(sizeof(myImage));
    resultImage->colors = (PPMPixel *) malloc(sizeof(PPMPixel) * colorNumber);  //For now, I'm just considering a palette size equals to colors number
	resultImage->data = (unsigned char *) malloc(sizeof(unsigned char) * image->x * image ->y);
    //

    printf("Calculating color reduction\n");

    //Calculating color palette size
    if (colorNumber > 256)
        paletteSize = 256;
    else
        paletteSize = colorNumber;

    //Finding first 256 color frequencies in the image and creating a vector for them
    frequencies = frequencyVector(colorPointer);

    //Filling palette with first 256 most frequent colors, selected from 8 different subspace of RGB color space cube
    int j = 0;  //added color index
    int i1, i2, i3, i4, i5, i6, i7, i8; //index for counting colors found in subcubes (tracking count for equal distribution)
    i1 = i2 = i3 = i4 = i5 = i6 = i7 = i8 = 0;

    if (paletteSize < 256) {
        for (i = 0; i < paletteSize; i++) {
            resultImage->colors[j].red = colorPointer->red;
            resultImage->colors[j].green = colorPointer->green;
            resultImage->colors[j].blue = colorPointer->blue;
            j++;
            colorPointer = colorPointer->next;
        }
    } else
        for (i = 0; i < paletteSize; i++) {
            while (colorPointer) {
                if (colorPointer->frequency == frequencies[i] && j < 256) {
                    if (colorPointer->red > 126 && colorPointer->green < 127 && colorPointer->blue < 127 && i1 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i1++;
                    } else if (colorPointer->red > 126 && colorPointer->green < 127 && colorPointer->blue > 126 && i2 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i2++;
                    } else if (colorPointer->red > 126 && colorPointer->green > 126 && colorPointer->blue > 126 && i3 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i3++;
                    } else if (colorPointer->red > 126 && colorPointer->green > 126 && colorPointer->blue < 127 && i4 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i4++;
                    } else if (colorPointer->red < 127 && colorPointer->green < 127 && colorPointer->blue < 127 && i5 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i5++;
                    } else if (colorPointer->red < 127 && colorPointer->green < 127 && colorPointer->blue > 126 && i6 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i6++;
                    } else if (colorPointer->red < 127 && colorPointer->green > 126 && colorPointer->blue > 126 && i7 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i7++;
                    } else if (colorPointer->red < 127 && colorPointer->green > 126 && colorPointer->blue < 127 && i8 < 32) {
                        resultImage->colors[j].red = colorPointer->red;
                        resultImage->colors[j].green = colorPointer->green;
                        resultImage->colors[j].blue = colorPointer->blue;
                        j++;
                        i8++;
                    }
                }
                colorPointer = colorPointer->next;
            }
            colorPointer = returnHead();
        }
    //printf("%d %d %d %d %d %d %d %d\n", i1, i2, i3, i4, i5, i6, i7, i8);

    printf("Compressing image\n");
    //Create compressed image
    resultImage->x = image->x;
    resultImage->y = image->y;
    resultImage->pSize = i1 + i2 + i3 + i4 + i5 +i6 + i7 + i8;

    double minDistance;
    int flag = 0;
    int paletteIndex;
    for(i = 0; i < image->x * image->y; i++){
        paletteIndex = 0;
        flag = 0;
        minDistance = findDistance(image->data[i],resultImage->colors[0]);
        for (j = 1; j < paletteSize && !flag; j++) {
            if(minDistance > findDistance(image->data[i],resultImage->colors[j])){
                minDistance = findDistance(image->data[i],resultImage->colors[j]);
                paletteIndex = j;
                if(minDistance == 0)
                    flag = 1;
            }
        }
        resultImage->data[i]= paletteIndex;
    }

    printf("Generating compressed file\n");
    //--------- Write image---------
    FILE * fp;

	//open file for output
    fp = fopen("reduced.ppmc", "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file");
         exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "CP6\n");   //Compressed P6

    //comments
    fprintf(fp, "# Created by MochTor\n");

    //fprintf(stderr, "Qui arrivo: %d %d\n", image->x, image->y);
    //image size
    fprintf(fp, "%d %d %d\n",resultImage->x, resultImage->y, resultImage->pSize);

    //palette data
    fwrite(resultImage->colors, 3, resultImage->pSize, fp);

    //pixel data
    fwrite(resultImage->data, 1, resultImage->x * resultImage->y, fp);

    fclose(fp);
    //-------------------------------
}

/**
 * This function finds the number of unique colors of a PPM image and fills the palette
 * @param  image: the image for which to find the color number
 * @return       the number of colors
 */
int colorList(PPMImage *image){
	int colorNumber = 0;
	PPMPixel pixel;
    for (int i = 0; i < image->x * image->y; i++) {
        pixel.red = image->data[i].red;
        pixel.green = image->data[i].green;
        pixel.blue = image->data[i].blue;
        if (!search(pixel)) {
            colorNumber++;
            add(pixel);
        }
    }
    return colorNumber;
}

/**
 * The function return the maximum frequency number for a color in the palette
 * @param  el: pointer to palette
 * @return    the frequency number
 */
int maxFrequency(struct color* el) {
    int max = 0;
    while (el) {
        if (el->frequency > max) {
            max = el->frequency;
        }
        el = el->next;
    }
    return max;
}

/**
 * This function bulds the frequency vector for colors in the color list:
 * it inserts frequencies in decreasing order [it consider just the first 256 frequencies,
 * which may not correspond to the first 256 colors]
 * @param  head: head of the list from which create frequencies vector
 * @return      pointer to the vector
 */
int* frequencyVector(struct color *head) {
    struct color* el = head;
    int max = maxFrequency(el);
    int secondMax;
    int* frequencyArray = (int *) malloc(sizeof(int)*256);
    frequencyArray[0] = max;
    for (int i = 1; i < 256; i++) {
        secondMax = 0;
        while (el) {
            if (el->frequency < max && el->frequency > secondMax) {
                secondMax = el->frequency;
            }
            el = el->next;
        }
        frequencyArray[i] = secondMax;
        max = secondMax;
        el = head;
    }
    return frequencyArray;
}

/**
 * This function return the distance between two RGB colors (PPMPixel version)
 * CIE76 formula was used
 * @param  pixel1: first color
 * @param  pixel2: second color
 * @return        the distance
 */
double findDistance(PPMPixel pixel1,PPMPixel pixel2) {
    double distance;
    int redComp = pixel1.red - pixel2.red;
    int greenComp = pixel1.green -pixel2.green;
    int blueComp = pixel1.blue -pixel2.blue;
    distance = (redComp*redComp + greenComp*greenComp + blueComp*blueComp);
    distance = sqrt(distance);
    return  distance;
}

/**
 * This function return the distance between two RGB colors (struct color version)
 * CIE76 formula was used
 * @param  pixel1: first color
 * @param  pixel2: second color
 * @return        the distance
 */
double findDistanceC(struct color *pixel1, PPMPixel pixel2) {
    double distance;
    int redComp = pixel1->red - pixel2.red;
    int greenComp = pixel1->green -pixel2.green;
    int blueComp = pixel1->blue -pixel2.blue;
    distance = (redComp*redComp + greenComp*greenComp + blueComp*blueComp);
    distance = sqrt(distance);
    return  distance;
}
