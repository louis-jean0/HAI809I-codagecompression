#include "image_ppm.h"
#include <iostream>

void transformation_recursive(OCTET* Img, OCTET* ImgOut, int nH, int nW, int niveau, int N) {
    if(niveau > N) return;

    int nTaille = nH * nW;
    OCTET *ImgBF, *ImgMFh, *ImgMFv, *ImgHF;
    allocation_tableau(ImgBF, OCTET, nTaille/4);
    allocation_tableau(ImgMFh, OCTET, nTaille/4);
    allocation_tableau(ImgMFv, OCTET, nTaille/4);
    allocation_tableau(ImgHF, OCTET, nTaille/4);

    for(int i = 0; i < nH/2; i++) {
        for(int j = 0; j < nW/2; j++) {
            OCTET A = Img[indiceImage(2*i, 2*j, nW)];
            OCTET B = Img[indiceImage(2*i+1, 2*j, nW)];
            OCTET C = Img[indiceImage(2*i, 2*j+1, nW)];
            OCTET D = Img[indiceImage(2*i+1, 2*j+1, nW)];

            int current_pixel_idx = indiceImage(i, j, nW/2);
            ImgBF[current_pixel_idx] = (A + B + C + D) / 4;
            ImgMFh[current_pixel_idx] = ((A + B - C - D) / 2) + 128;
            ImgMFv[current_pixel_idx] = ((A - B + C - D) / 2) + 128;
            ImgHF[current_pixel_idx] = (A - B - C + D) + 128;

            // Reconstruction
            ImgOut[indiceImage(2*i,2*j,nW)] = ImgBF[current_pixel_idx] + (ImgMFv[current_pixel_idx]/2) + (ImgMFh[current_pixel_idx]/2) + (ImgHF[current_pixel_idx]/4) - 160;
            ImgOut[indiceImage(2*i+1,2*j,nW)] = ImgBF[current_pixel_idx] + 32 - (ImgMFv[current_pixel_idx]/2) + (ImgMFh[current_pixel_idx]/2) - (ImgHF[current_pixel_idx]/4);
            ImgOut[indiceImage(2*i,2*j+1,nW)] = (ImgMFv[current_pixel_idx]/2) - (ImgMFh[current_pixel_idx]/2) + ImgBF[current_pixel_idx] + 32 - (ImgHF[current_pixel_idx]/4);
            ImgOut[indiceImage(2*i+1,2*j+1,nW)] = ImgBF[current_pixel_idx] + (ImgHF[current_pixel_idx]/4) - (ImgMFv[current_pixel_idx]/2) - (ImgMFh[current_pixel_idx]/2) + 96;

        }
    }

    double PSNR = PSNR_pgm(Img, ImgOut, nH, nW);
    std::cout<<"PSNR pour le niveau "<<niveau<<" : "<<PSNR<<"dB"<<std::endl;

    int nHNext = nH/2;
    int nWNext = nW/2;
    OCTET* ImgBFNext;
    allocation_tableau(ImgBFNext, OCTET, nHNext * nWNext);

    for(int i = 0; i < nHNext; ++i) {
        for (int j = 0; j < nWNext; ++j) {
            ImgBFNext[indiceImage(i, j, nWNext)] = ImgBF[indiceImage(i, j, nWNext)];
        }
    }

    free(ImgMFh);
    free(ImgMFv);
    free(ImgHF);

    transformation_recursive(ImgBFNext, ImgOut, nHNext, nWNext, niveau + 1, N);

    free(ImgBFNext);
}

int main(int argc, char* argv[]) {

    if(argc != 4) {
        printf("Utilisation : %s image_originale.pgm image_out.pgm N \n",argv[0]);
        exit(1);
    }

    int nH, nW, nTaille;
    char nomImageOriginale[256], nomImgOut[256];

    sscanf(argv[1],"%s",nomImageOriginale);
    sscanf(argv[2],"%s",nomImgOut);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(nomImageOriginale,&nH,&nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn,OCTET,nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);

    lire_image_pgm(nomImageOriginale,ImgIn,nTaille);

    int N = atoi(argv[3]);

    transformation_recursive(ImgIn, ImgOut, nH, nW, 0, N);

    ecrire_image_pgm(nomImgOut, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgOut);

    return 0;

}