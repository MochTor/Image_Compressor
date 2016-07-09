#include <stdio.h>
#include <stdlib.h>
#include "ImageHandler.h"
#include "list.h"

struct color *paletteHead = NULL;

struct color* init(PPMPixel pixel) {
    struct color *new = (struct color*) malloc(sizeof(struct color));
    new->frequency = 1;
    new->red = pixel.red;
    new->green = pixel.green;
    new->blue = pixel.blue;
    new->next = NULL;

    paletteHead = new;
    return new;
}

int search(PPMPixel pixel, struct color **prev){
    struct color *ptr = paletteHead;
    struct color *tmp = NULL;
    int found = 0;

    while (ptr != NULL) {
        if (ptr->red == pixel.red && ptr->green == pixel.green && ptr->blue == pixel.blue) {
            ptr->frequency++;
            found = 1;
            break;
        } else {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if (found) {
        if(prev)
            *prev = tmp;
        return 1;
    } else {
        return 0;
    }
}

struct color* add(PPMPixel pixel){
    if (paletteHead == NULL)
        return (init(pixel));

    struct color *new = (struct color*) malloc(sizeof(struct color));
    new->frequency = 1;
    new->red = pixel.red;
    new->green = pixel.green;
    new->blue = pixel.blue;
    new->next = NULL;

    return new;
}
