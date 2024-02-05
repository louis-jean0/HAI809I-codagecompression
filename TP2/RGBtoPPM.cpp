#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 5) {
        printf("Utilisation : %s image_in.ppm image_R.ppm image_G.ppm image_B.ppm\n",argv[0]);
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
    allocation_tableau(ImgR,OCTET,nTaille3);
    allocation_tableau(ImgG,OCTET,nTaille3);
    allocation_tableau(ImgB,OCTET,nTaille3);

    lire_image_ppm(nomImageIn,ImgIn,nTaille);

    // planR(ImgR,ImgIn,nTaille3);
    // planG(ImgG,ImgIn,nTaille3);
    // planB(ImgB,ImgIn,nTaille3);

    for(int i = 0; i < nTaille; i++) {
        ImgR[3*i] = ImgIn[3*i];
        ImgR[3*i+1] = 0;
        ImgR[3*i+2] = 0;
        ImgG[3*i+1] = ImgIn[3*i+1];
        ImgG[3*i] = 0;
        ImgG[3*i+2] = 0;
        ImgB[3*i+2] = ImgIn[3*i+2];
        ImgB[3*i] = 0;
        ImgB[3*i+1] = 0;
    }

    ecrire_image_ppm(nomImageR,ImgR,nH,nW);
    ecrire_image_ppm(nomImageG,ImgG,nH,nW);
    ecrire_image_ppm(nomImageB,ImgB,nH,nW);

    free(ImgIn);
    free(ImgR);
    free(ImgG);
    free(ImgB);

    return 0;

}