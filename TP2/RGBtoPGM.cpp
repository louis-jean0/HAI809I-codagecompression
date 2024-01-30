#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 5) {
        printf("Utilisation : %s image_in.ppm image_R.pgm image_G.pgm image_B.pgm\n",argv[0]);
        exit(1);
    }

    char nomImageIn[256], nomImageR[256], nomImageG[256], nomImageB[256];

    int nH, nW, nTaille, nTaille3;

    sscanf(argv[1],"%s",nomImageIn);
    sscanf(argv[2],"%s",nomImageR);
    sscanf(argv[3],"%s",nomImageG);
    sscanf(argv[4],"%s",nomImageB);

    lire_nb_lignes_colonnes_image_ppm(nomImageIn,&nH,&nW);

    nTaille = nH * nW;
    nTaille3 = 3 * nTaille;

    OCTET *ImgIn, *ImgR, *ImgG, *ImgB;

    allocation_tableau(ImgIn,OCTET,nTaille3);
    allocation_tableau(ImgR,OCTET,nTaille);
    allocation_tableau(ImgG,OCTET,nTaille);
    allocation_tableau(ImgB,OCTET,nTaille);

    lire_image_ppm(nomImageIn,ImgIn,nTaille);

    planR(ImgR,ImgIn,nTaille);
    planG(ImgG,ImgIn,nTaille);
    planB(ImgB,ImgIn,nTaille);

    ecrire_image_pgm(nomImageR,ImgR,nH,nW);
    ecrire_image_pgm(nomImageG,ImgG,nH,nW);
    ecrire_image_pgm(nomImageB,ImgB,nH,nW);

    free(ImgIn);
    free(ImgR);
    free(ImgG);
    free(ImgB);

    return 0;

}