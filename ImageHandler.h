#ifndef image_handler
#define  image_handler

//The following structs define a pixel of the image
typedef struct _pixel {
    //I could use unsigned char instead of short int 'cause P6 component limit is 255
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} pixel;

typedef struct _image {
    int x;
    int y;
    pixel color;
} image;

#endif
