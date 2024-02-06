#include <stdio.h>
#include <stdlib.h>
#include "image_ppm.h"

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Utilisation : %s image_in.pgm image_sortie.pgm\n",argv[0]);
        exit(1);
    }

    char nomImgIn[256], nomImgOut[256];
    sscanf(argv[1],"%s",nomImgIn);
    sscanf(argv[2],"%s",nomImgOut);

    int nH, nW, nTaille;
    OCTET *ImgIn, *ImgDiff;
    lire_nb_lignes_colonnes_image_pgm(nomImgIn, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(nomImgIn, ImgIn, nTaille);
    allocation_tableau(ImgDiff, OCTET, nTaille);

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int index = i*nW + j;
            int sum = 0;
            int count = 0;

            // Vérifie et ajoute le pixel à gauche
            if (j > 0) {
                sum += ImgIn[index - 1];
                count++;
            }

            // Vérifie et ajoute le pixel au-dessus
            if (i > 0) {
                sum += ImgIn[index - nW];
                count++;
                
                // Vérifie et ajoute le pixel en diagonale au-dessus à gauche
                if (j > 0) {
                    sum += ImgIn[index - nW - 1];
                    count++;
                }
            }

            int predictedValue = (count > 0) ? sum / count : 0;
            int diff = ImgIn[index] - predictedValue;
            ImgDiff[index] = (OCTET)std::min(std::max(diff + 128, 0), 255);
        }
    }

    ecrire_image_pgm(nomImgOut, ImgDiff, nH, nW);
    free(ImgIn);
    free(ImgDiff);

    return 0;
}
