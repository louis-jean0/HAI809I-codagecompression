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

    int nH,nW,nTaille;
    OCTET *ImgIn, *ImgDiff;
    lire_nb_lignes_colonnes_image_pgm(nomImgIn,&nH,&nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn,OCTET,nTaille);
    lire_image_pgm(nomImgIn, ImgIn, nTaille);
    allocation_tableau(ImgDiff, OCTET, nTaille);

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int index = i*nW + j;
            int pixelValue = ImgIn[index];
            int predictedValue;

            if (j == 0) { // Premier pixel de la ligne
                predictedValue = (i == 0) ? 0 : ImgIn[index - nW]; // Utilise le pixel au-dessus si possible
            } else {
                predictedValue = ImgIn[index - 1]; // Utilise le pixel précédent
            }

            int diff = pixelValue - predictedValue;
            ImgDiff[index] = (OCTET)std::min(std::max(diff + 128, 0), 255); // Ajuste et assure que la valeur est dans [0, 255]
        }
    }

    ecrire_image_pgm(nomImgOut, ImgDiff, nH, nW);
    free(ImgIn);
    free(ImgDiff);

    // Ici, vous pourriez tracer la distribution de la carte des différences
    // et appliquer l'algorithme d'Huffman pour la compression.

    return 0;
}
