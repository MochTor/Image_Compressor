#ifndef image_handler_h
#define image_handler_h

//The following structures define a pixel of the image and the strut of the image
typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;
//

void writePPM(const char *, PPMImage *);
PPMImage *readPPM(const char *);

#endif
