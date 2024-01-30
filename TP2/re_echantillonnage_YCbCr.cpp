#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 5) {
        printf("Utilisation : %s image_dominante.pgm image_minorant1.pgm image_minorant2.pgm image_out.ppm\n",argv[0]);
        exit(1);
    }

    char nomImageDominante[256], nomImageMinorant1[256], nomImageMinorant2[256], nomImageOut[256];

    int nH, nW, nTaille, nTaille3;

    sscanf(argv[1],"%s",nomImageDominante);
    sscanf(argv[2],"%s",nomImageMinorant1);
    sscanf(argv[3],"%s",nomImageMinorant2);
    sscanf(argv[4],"%s",nomImageOut);

    lire_nb_lignes_colonnes_image_pgm(nomImageDominante,&nH,&nW);

    nTaille = nH * nW;
    nTaille3 = 3 * nTaille;

    OCTET *ImgDominante, *ImgMinorant1, *ImgMinorant2, *ImgOut;

    allocation_tableau(ImgDominante,OCTET,nTaille);
    allocation_tableau(ImgMinorant1,OCTET,nTaille/4);
    allocation_tableau(ImgMinorant2,OCTET, nTaille/4);
    allocation_tableau(ImgOut,OCTET,nTaille3);

    lire_image_pgm(nomImageDominante,ImgDominante,nTaille);
    lire_image_pgm(nomImageMinorant1,ImgMinorant1,nTaille/4);
    lire_image_pgm(nomImageMinorant2,ImgMinorant2,nTaille/4);

    OCTET *ImgMinorant1ReEchantillonnee, *ImgMinorant2ReEchantillonnee;

    allocation_tableau(ImgMinorant1ReEchantillonnee,OCTET,nTaille);
    allocation_tableau(ImgMinorant2ReEchantillonnee,OCTET,nTaille);

    re_echantillonner_image_facteur_4(ImgMinorant1,ImgMinorant1ReEchantillonnee,nH,nW);
    re_echantillonner_image_facteur_4(ImgMinorant2,ImgMinorant2ReEchantillonnee,nH,nW);

    reconstruire_ppm_depuis_YCbCr(ImgMinorant1ReEchantillonnee, ImgDominante, ImgMinorant1ReEchantillonnee, ImgOut, nTaille); // Il faut ImgR, ImgG et ImgB dans cet ordre
    
    ecrire_image_ppm(nomImageOut,ImgOut,nH,nW);

    free(ImgDominante);
    free(ImgMinorant1);
    free(ImgMinorant2);
    free(ImgMinorant1ReEchantillonnee);
    free(ImgMinorant2ReEchantillonnee);
    free(ImgOut);

    return 0;

}
