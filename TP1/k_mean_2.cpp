#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 5) {
        printf("Utilisation : %s image_in.ppm image_out_debut.ppm image_out_iteration.ppm image_out_finale.ppm\n",argv[0]);
        exit(1);
    }

    int nH, nW, nTaille;
    // Ici les couleurs ont été choisies au color picker pour l'image bard.ppm
    OCTET couleur1[3] = {187,29,133};
    OCTET couleur2[3] = {205,185,199};
    char nomImageIn[256], nomImageOutDebut[256], nomImageOutIteration[256], nomImageOutFinale[256];

    sscanf(argv[1],"%s",nomImageIn);
    sscanf(argv[2],"%s",nomImageOutDebut);
    sscanf(argv[3],"%s",nomImageOutIteration);
    sscanf(argv[4],"%s",nomImageOutFinale);

    OCTET *ImgIn, *ImgOutDebut, *ImgOutIteration, *ImgOutFinale;

    lire_nb_lignes_colonnes_image_ppm(nomImageIn,&nH,&nW);

    nTaille = nH * nW;
    int nTaille3 = 3 * nTaille; // On multiplie par 3 car 3 composantes couleur

    allocation_tableau(ImgIn,OCTET,nTaille3);
    allocation_tableau(ImgOutDebut,OCTET,nTaille3);
    allocation_tableau(ImgOutIteration,OCTET,nTaille3);
    allocation_tableau(ImgOutFinale,OCTET,nTaille3);

    lire_image_ppm(nomImageIn,ImgIn,nTaille);

    k_mean_2(ImgIn,ImgOutDebut,ImgOutIteration,ImgOutFinale,nTaille,couleur1,couleur2); // Fonction dans image_ppm.h

    ecrire_image_ppm(nomImageOutDebut,ImgOutDebut,nH,nW);
    ecrire_image_ppm(nomImageOutIteration, ImgOutIteration, nH, nW);
    ecrire_image_ppm(nomImageOutFinale,ImgOutFinale,nH,nW);

    free(ImgIn);
    free(ImgOutDebut);
    free(ImgOutFinale);

    return 0;

}