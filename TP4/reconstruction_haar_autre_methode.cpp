#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Utilisation : %s image_originale.pgm image_out.pgm\n",argv[0]);
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

            // Reconstruction
            ImgOut[indiceImage(2*i,2*j,nW)] = ImgBF[current_pixel_idx] + (ImgMFv[current_pixel_idx]/2) + (ImgMFh[current_pixel_idx]/2) + (ImgHF[current_pixel_idx]/4) - 160;
            ImgOut[indiceImage(2*i,2*j+1,nW)] = (ImgMFv[current_pixel_idx]/2) - (ImgMFh[current_pixel_idx]/2) + ImgBF[current_pixel_idx] + 32 - (ImgHF[current_pixel_idx]/4);
            ImgOut[indiceImage(2*i+1,2*j,nW)] = ImgBF[current_pixel_idx] + 32 - (ImgMFv[current_pixel_idx]/2) + (ImgMFh[current_pixel_idx]/2) - (ImgHF[current_pixel_idx]/4);
            ImgOut[indiceImage(2*i+1,2*j+1,nW)] = ImgBF[current_pixel_idx] + (ImgHF[current_pixel_idx]/4) - (ImgMFv[current_pixel_idx]/2) - (ImgMFh[current_pixel_idx]/2) + 96;
        }
    }

    OCTET *BF_re_ech, *MFh_re_ech, *MFv_re_ech, *HF_re_ech;
    allocation_tableau(BF_re_ech, OCTET, nTaille);
    allocation_tableau(MFh_re_ech, OCTET, nTaille);
    allocation_tableau(MFv_re_ech, OCTET, nTaille);
    allocation_tableau(HF_re_ech, OCTET, nTaille);

    re_echantillonner_image_facteur_4(ImgBF, BF_re_ech, nH, nW);
    re_echantillonner_image_facteur_4(ImgMFh, MFh_re_ech, nH, nW);
    re_echantillonner_image_facteur_4(ImgMFv, MFv_re_ech, nH, nW);
    re_echantillonner_image_facteur_4(ImgHF, HF_re_ech, nH, nW);

    for(int i = 1; i < nH - 1; i++) {
        for(int j = 1; j < nW - 1; j++) {
            ImgOut[indiceImage(i, j, nW)] = BF_re_ech[indiceImage(i, j, nW)] + (MFv_re_ech[indiceImage(i, j, nW)]/2) + (MFh_re_ech[indiceImage(i, j, nW)]/2) + (HF_re_ech[indiceImage(i, j, nW)]/4) - 160;
            ImgOut[indiceImage(i, j+1, nW)] = (MFv_re_ech[indiceImage(i+1, j, nW)]/2) - (MFh_re_ech[indiceImage(i+1, j, nW)]/2) + BF_re_ech[indiceImage(i+1, j, nW)] + 32 - (HF_re_ech[indiceImage(i+1, j, nW)]/4);
            ImgOut[indiceImage(i+1, j, nW)] = BF_re_ech[indiceImage(i+1, j, nW)] + 32 - (MFv_re_ech[indiceImage(i+1, j, nW)]/2) + (MFh_re_ech[indiceImage(i+1, j, nW)]/2) - (HF_re_ech[indiceImage(i+1, j, nW)]/4);
            ImgOut[indiceImage(i+1, j+1, nW)] = BF_re_ech[indiceImage(i+1, j+1, nW)] + (HF_re_ech[indiceImage(i+1, j+1, nW)]/4) - (MFv_re_ech[indiceImage(i+1, j+1, nW)]/2) - (MFh_re_ech[indiceImage(i+1, j+1, nW)]/2) + 96;
        }
    }

    ecrire_image_pgm(nomImgOut, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgBF);
    free(ImgMFh);
    free(ImgMFv);
    free(ImgHF);
    free(ImgOut);
    //free(ImgOut_re_ech);

    return 0;

}