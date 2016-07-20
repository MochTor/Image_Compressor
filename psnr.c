#include <stdio.h>
#include <math.h>
#include "ImageHandler.h"

double MSE(PPMImage *, PPMImage *);
double PSNR(PPMImage *, PPMImage *);

int main(int argc, char const *argv[]){
	PPMImage * image1;
	PPMImage * image2;
	double psnr;

	image1 = readPPM(argv[1]);
	image2 = readPPM(argv[2]);

	psnr = PSNR(image1, image2);

	printf("PSNR value for image %s and %s is: %f\n",argv[1], argv[2], psnr);
	return 0;
}

double MSE(PPMImage *image1, PPMImage *image2) {
    double mse, MSE_R, MSE_G, MSE_B;
    int i;
    double tmp;

    MSE_R = 0;
    for(i=0; i < image1->x * image1->y; i++){
        tmp = image1->data[i].red - image2->data[i].red;
        tmp = tmp * tmp;
        MSE_R+= tmp;
    }
    MSE_R = MSE_R/(image1->x*image1->y);

    MSE_G = 0;
    for(i=0; i < image1->x * image1->y; i++){
        tmp = image1->data[i].green - image2->data[i].green;
        tmp = tmp * tmp;
        MSE_G+= tmp;
    }
    MSE_G = MSE_G/(image1->x*image1->y);

    MSE_B = 0;
    for(i=0; i < image1->x * image1->y; i++){
        tmp = image1->data[i].blue - image2->data[i].blue;
        tmp = tmp * tmp;
        MSE_B+= tmp;
    }
    MSE_B = MSE_B/(image1->x*image1->y);

    mse = (MSE_R + MSE_G + MSE_B)/3;

    return mse;
}

double PSNR(PPMImage *image1, PPMImage *image2) {
    double psnr;
    double mse;

    mse = MSE(image1,image2);

    mse = 255 / sqrt(mse);

    psnr = 20*log10(mse);

    return psnr;
}
