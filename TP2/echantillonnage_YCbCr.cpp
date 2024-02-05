#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 5) {
        printf("Utilisation : %s image_in.ppm image_Y.pgm image_Cb.pgm image_Cr.pgm\n",argv[0]);
        exit(1);
    }

    char nomImageIn[256], nomImageY[256], nomImageCb[256], nomImageCr[256];

    int nH, nW, nTaille, nTaille3;

    sscanf(argv[1],"%s",nomImageIn);
    sscanf(argv[2],"%s",nomImageY);
    sscanf(argv[3],"%s",nomImageCb);
    sscanf(argv[4],"%s",nomImageCr);

    lire_nb_lignes_colonnes_image_ppm(nomImageIn,&nH,&nW);

    nTaille = nH * nW;
    nTaille3 = 3 * nTaille;

    OCTET *ImgIn, *ImgY, *ImgCb, *ImgCr;

    allocation_tableau(ImgIn,OCTET,nTaille3);
    allocation_tableau(ImgY,OCTET,nTaille);
    allocation_tableau(ImgCb,OCTET,nTaille);
    allocation_tableau(ImgCr,OCTET,nTaille);

    lire_image_ppm(nomImageIn,ImgIn,nTaille);

    RGBtoYCbCr(ImgIn,ImgY,ImgCb,ImgCr,nTaille);

    OCTET *ImgY_echantillonnee, *ImgCb_echantillonnee;

    allocation_tableau(ImgY_echantillonnee,OCTET,nTaille/4);
    allocation_tableau(ImgCb_echantillonnee,OCTET,nTaille/4);

    echantillonner_image_facteur_4(ImgY,ImgY_echantillonnee,nH,nW);
    echantillonner_image_facteur_4(ImgCb,ImgCb_echantillonnee,nH,nW);

    ecrire_image_pgm(nomImageY,ImgY_echantillonnee,nH/2,nW/2);
    ecrire_image_pgm(nomImageCb,ImgCb_echantillonnee,nH/2,nW/2);
    ecrire_image_pgm(nomImageCr,ImgCr,nH,nW);

    free(ImgIn);
    free(ImgY);
    free(ImgCb);
    free(ImgCr);
    free(ImgY_echantillonnee);
    free(ImgCb_echantillonnee);

    return 0;

}