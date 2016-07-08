#ifndef LIST_H
#define LIST_H

//Palette struct
struct color {
    int frequency;
    unsigned char red,green,blue;
    struct color *next;
};
//

//This function initialize color palette list
struct color* init(PPMPixel);

//This function search a color in color palette list
int search(PPMPixel, struct color **);

#endif
