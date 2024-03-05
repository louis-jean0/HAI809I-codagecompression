#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 7) {
        printf("Utilisation : %s image_originale.pgm image_out.pgm coeffBF coeffMFh coeffMFv coeffHF\n",argv[0]);
        exit(1);
    }

    int nH, nW, nTaille;
    char nomImageOriginale[256], nomImgOut[256];

    sscanf(argv[1],"%s",nomImageOriginale);
    sscanf(argv[2],"%s",nomImgOut);

    OCTET *ImgIn, *ImgBF, *ImgMFh, *ImgMFv, *ImgHF, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(nomImageOriginale,&nH,&nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn,OCTET,nTaille);
    allocation_tableau(ImgBF,OCTET,nTaille/4);
    allocation_tableau(ImgMFh,OCTET,nTaille/4);
    allocation_tableau(ImgMFv,OCTET,nTaille/4);
    allocation_tableau(ImgHF,OCTET,nTaille/4);
    allocation_tableau(ImgOut, OCTET, nTaille);

    lire_image_pgm(nomImageOriginale,ImgIn,nTaille);

    OCTET *QBF, *QMFh, *QMFv, *QHF;
    allocation_tableau(QBF, OCTET, nTaille/4);
    allocation_tableau(QMFh, OCTET, nTaille/4);
    allocation_tableau(QMFv, OCTET, nTaille/4);
    allocation_tableau(QHF, OCTET, nTaille/4);
    int coeffBF, coeffMFh, coeffMFv, coeffHF;
    coeffBF = atoi(argv[3]);
    coeffMFh = atoi(argv[4]);
    coeffMFv = atoi(argv[5]);
    coeffHF = atoi(argv[6]);

    for(int i = 0; i < nH/2; i++) {
        OCTET A,B,C,D;
        for(int j = 0; j < nW/2; j++) {
            A = ImgIn[indiceImage(2*i,2*j,nW)];
            B = ImgIn[indiceImage(2*i+1,2*j,nW)];
            C = ImgIn[indiceImage(2*i,2*j+1,nW)];
            D = ImgIn[indiceImage(2*i+1,2*j+1,nW)];

            int current_pixel_idx = indiceImage(i,j,nW/2);
            ImgBF[current_pixel_idx] = (A + B + C + D) / 4;
            ImgMFh[current_pixel_idx] = ((A + B - C - D) / 2) + 128;
            ImgMFv[current_pixel_idx] = ((A - B + C - D) / 2) + 128;
            ImgHF[current_pixel_idx] = (A - B - C + D) + 128;

            // Quantification
            QBF[current_pixel_idx] = round(ImgBF[current_pixel_idx] / (float)coeffBF) * coeffBF;
            QMFh[current_pixel_idx] = round(ImgMFh[current_pixel_idx] / (float)coeffMFh) * coeffMFh;
            QMFv[current_pixel_idx] = round(ImgMFv[current_pixel_idx] / (float)coeffMFv) * coeffMFv;
            QHF[current_pixel_idx] = round(ImgHF[current_pixel_idx] / (float)coeffHF) * coeffHF;

            // Reconstruction
            ImgOut[indiceImage(2*i,2*j,nW)] = QBF[current_pixel_idx] + (QMFv[current_pixel_idx]/2) + (QMFh[current_pixel_idx]/2) + (QHF[current_pixel_idx]/4) - 160;
            ImgOut[indiceImage(2*i+1,2*j,nW)] = QBF[current_pixel_idx] + 32 - (QMFv[current_pixel_idx]/2) + (QMFh[current_pixel_idx]/2) - (QHF[current_pixel_idx]/4);
            ImgOut[indiceImage(2*i,2*j+1,nW)] = (QMFv[current_pixel_idx]/2) - (QMFh[current_pixel_idx]/2) + QBF[current_pixel_idx] + 32 - (QHF[current_pixel_idx]/4);
            ImgOut[indiceImage(2*i+1,2*j+1,nW)] = QBF[current_pixel_idx] + (QHF[current_pixel_idx]/4) - (QMFv[current_pixel_idx]/2) - (QMFh[current_pixel_idx]/2) + 96;
        }
    }

    ecrire_image_pgm(nomImgOut, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgBF);
    free(ImgMFh);
    free(ImgMFv);
    free(ImgHF);
    free(ImgOut);
    free(QBF);
    free(QMFh);
    free(QMFv);
    free(QHF);

    return 0;

}