#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 6) {
        printf("Utilisation : %s image_originale.pgm image_BF.pgm image_MFh.pgm image_MFv.pgm image_HF.pgm\n",argv[0]);
        exit(1);
    }

    int nH, nW, nTaille;
    char nomImageOriginale[256], nomImageBF[256], nomImageMFh[256], nomImageMFv[256], nomImageHF[256];

    sscanf(argv[1],"%s",nomImageOriginale);
    sscanf(argv[2],"%s",nomImageBF);
    sscanf(argv[3],"%s",nomImageMFh);
    sscanf(argv[4],"%s",nomImageMFv);
    sscanf(argv[5],"%s",nomImageHF);

    OCTET *ImgIn, *ImgBF, *ImgMFh, *ImgMFv, *ImgHF;

    lire_nb_lignes_colonnes_image_pgm(nomImageOriginale,&nH,&nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn,OCTET,nTaille);
    allocation_tableau(ImgBF,OCTET,nTaille/4);
    allocation_tableau(ImgMFh,OCTET,nTaille/4);
    allocation_tableau(ImgMFv,OCTET,nTaille/4);
    allocation_tableau(ImgHF,OCTET,nTaille/4);

    lire_image_pgm(nomImageOriginale,ImgIn,nTaille);

    for(int i = 0; i < nH/2; i++) {
        OCTET pixel1,pixel2,pixel3,pixel4;
        for(int j = 0; j < nW/2; j++) {
            pixel1 = ImgIn[indiceImage(2*i,2*j,nW)];
            pixel2 = ImgIn[indiceImage(2*i,2*j+1,nW)];
            pixel3 = ImgIn[indiceImage(2*i+1,2*j,nW)];
            pixel4 = ImgIn[indiceImage(2*i+1,2*j+1,nW)];

            int current_pixel_idx = indiceImage(i,j,nW/2);
            ImgBF[current_pixel_idx] = (pixel1 + pixel2 + pixel3 + pixel4) / 4;
            ImgMFh[current_pixel_idx] = ((pixel1 + pixel2 - pixel3 - pixel4) / 2) + 128;
            ImgMFv[current_pixel_idx] = ((pixel1 - pixel2 + pixel3 - pixel4) / 2) + 128;
            ImgHF[current_pixel_idx] = (pixel1 - pixel2 - pixel3 + pixel4) + 128;
        }
    }

    ecrire_image_pgm(nomImageBF,ImgBF,nH/2,nW/2);
    ecrire_image_pgm(nomImageMFh,ImgMFh,nH/2,nW/2);
    ecrire_image_pgm(nomImageMFv,ImgMFv,nH/2,nW/2);
    ecrire_image_pgm(nomImageHF,ImgHF,nH/2,nW/2);

    free(ImgIn);
    free(ImgBF);
    free(ImgMFh);
    free(ImgMFv);
    free(ImgHF);

    return 0;

}