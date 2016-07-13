#include <stdio.h>
#include <stdlib.h>
#include "ImageHandler.h"
#include "list.h"

//My image format struct (the struct is similar to GIF: header, palette, raw data)
typedef struct {
    int x, y;
    PPMPixel *colors;
    unsigned char *data;
} myImage;
//

void compressImage(PPMImage *, myImage *, int);
//struct color* findFrequency(struct color*, int);
int colorList(PPMImage *);
int maxFrequency(struct color*);
int* frequencyVector(struct color *el);

int main(int argc, char const *argv[]) {
    PPMImage *image;
    myImage *compressedRawImage;
    int colorNumber;
    image = readPPM(argv[1]);
    fprintf(stderr, "PPM image properly read\n");
    colorNumber = colorList(image);
    fprintf(stderr, "Colors: #%d\n", colorNumber);  //Tested with GIMP color analizer: it works properly
    // printColor();
    compressImage(image, compressedRawImage, colorNumber);
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

    //Calculating color palette size and filling result image palette
    if (colorNumber > 256)
        paletteSize = 256;
    else
        paletteSize = colorNumber;

    frequencies = frequencyVector(colorPointer);
    int ii;
    for (ii = 0; ii < 256; ii++) {
        printf("Frequency N %d: %d\n", ii, frequencies[ii]);
    }
    // int max = maxFrequency(colorPointer);
    // printf("Max frequency: %d\n", max);
    //struct color *maxColor = findFrequency(colorPointer, max);
    //printf("Color with maximum frequency: %d, %d, %d\n", maxColor->red, maxColor->green, maxColor->blue);

    //For the moment, I've just put the first 256 colors of the palette, without filtering them
    // while (i < paletteSize) {
    //     resultImage->colors[i].red = colorPointer->red;
    //     resultImage->colors[i].green = colorPointer->green;
    //     resultImage->colors[i].blue = colorPointer->blue;
    //     //printf("Filling palette with color number %d; values: %d, %d, %d, with frequency %d \n", i+1, resultImage->colors[i].red, resultImage->colors[i].green, resultImage->colors[i].blue, colorPointer->frequency);
    //
    //     colorPointer = colorPointer->next;
    //
    //     i++;
    // }

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

// struct color* findFrequency(struct color* el, int requiredFrequency) {
//     while (el->frequency != requiredFrequency) {
//         el = el->next;
//     }
//     return el;
// }

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
