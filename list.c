#include <stdio.h>
#include <stdlib.h>
#include "ImageHandler.h"
#include "list.h"

struct color* init(PPMPixel pixel) {
    struct color *ptr = (struct color*) malloc(sizeof(struct color));
    ptr->red = pixel.red;
    ptr->green = pixel.green;
    ptr->blue = pixel.blue;
    ptr->frequency = 1;
    ptr->next = NULL;

    head = curr = ptr;
    return ptr;
}

int search(PPMPixel pixel, struct color **prev){
    struct color *ptr = head;
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
