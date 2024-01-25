#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Utilisation : %s image_in.ppm image_out.ppm\n",argv[0]);
        exit(1);
    }

    int nH, nW, nTaille;
    OCTET couleur1[3] = {255,0,0};
    OCTET couleur2[3] = {0,0,255};
    char nomImageIn[256], nomImageOut[256];

    sscanf(argv[1],"%s",nomImageIn);
    sscanf(argv[2],"%s",nomImageOut);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(nomImageIn,&nH,&nW);

    nTaille = nH * nW;
    int nTaille3 = 3 * nTaille; // On multiplie par 3 car 3 composantes couleur

    allocation_tableau(ImgIn,OCTET,nTaille3);
    allocation_tableau(ImgOut,OCTET,nTaille3);

    lire_image_ppm(nomImageIn,ImgIn,nTaille);

    int* clusterAssignments = k_mean_2(ImgIn,nTaille,couleur1,couleur2);

    for(int i = 0; i < nTaille; i++) {
        if(clusterAssignments[i] == 1) {
            ImgOut[3*i] = couleur1[0];
            ImgOut[3*i+1] = couleur1[1];
            ImgOut[3*i+2] = couleur1[2];
        }
        else {
            ImgOut[3*i] = couleur2[0];
            ImgOut[3*i+1] = couleur2[1];
            ImgOut[3*i+2] = couleur2[2];
        }
    }

    ecrire_image_ppm(nomImageOut,ImgOut,nH,nW);

    free(ImgIn);
    free(ImgOut);

    return 0;

}