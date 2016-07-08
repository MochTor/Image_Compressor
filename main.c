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

void compressImage(PPMImage *);
void findFrequency();
int colorList(PPMImage *);

int main(int argc, char const *argv[]) {
    PPMImage *image;
    image = readPPM(argv[1]);
    fprintf(stderr, "Lettura immagine PPM eseguita\n");
    return 0;
}

void compressImage(PPMImage *image) {

}

int colorList(PPMImage *image){
	int CNumber = 0;
	PPMPixel pixel;
    int numero_colori = 0;
    for (int i =0; i < image->x * image->y;i++) {
        pixel.red = image->data[i].red;
        pixel.green = image->data[i].green;
        pixel.blue = image->data[i].blue;
        if (search(pixel, NULL) == 0) {
                //add_to_list(pix);
                numero_colori++;
        }
    }
    return numero_colori;
}
