#include <stdio.h>
#include <stdlib.h>
#include "ImageHandler.h"
#include "list.h"

//The head of the list
struct color *paletteHead = NULL;

//Using a pointer to last element symplifies the add of a new element at the end of the list (no while cycle needed)
struct color *lastElement = NULL;

struct color* init(PPMPixel pixel) {
    struct color *new = (struct color*) malloc(sizeof(struct color));
    new->frequency = 1;
    new->red = pixel.red;
    new->green = pixel.green;
    new->blue = pixel.blue;
    new->next = NULL;

    paletteHead = new;
    lastElement = paletteHead;
    return new;
}

int search(PPMPixel pixel){
    struct color *current = paletteHead;
    int found = 0;

    while (current != NULL) {
        if (current->red == pixel.red && current->green == pixel.green && current->blue == pixel.blue) {
            current->frequency++;
            return 1;   //color found!
        } else {
            current = current->next;
        }
    }

    return 0;   //color not found!
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

    lastElement->next = new;
    lastElement = new;

    return new;
}
