#ifndef LIST_H
#define LIST_H

//Palette struct
struct color {
    int frequency;  //times the color appears in the image
    unsigned char red,green,blue;   //RGB triplet value
    struct color *next; //pointer to next color in list
};
//

/**
 * This function initialize color palette list with a head value
 * @param  PPMPixel: the color
 * @return          the pointer to the head of the list
 */
struct color* init(PPMPixel);

/**
 * This function search a color in color palette list
 * @param  PPMPixel: the color
 * @param  struct color:  the list to search in
 * @return          1 if found, 0 if not
 */
int search(PPMPixel);

/**
 * This function add a new entry into the palette list
 * @param  PPMPixel the color
 * @return          the new node
 */
struct color* add(PPMPixel);

/**
 * The function return the head pointer of the palette list
 * @return the pointer to the head of the list
 */
struct color* returnHead();

//Just a fuction used in test
void printColor();

#endif
