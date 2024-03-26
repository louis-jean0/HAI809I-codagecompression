#include "image_ppm.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("Utilisation : %s image_in.ppm predi_verticale.pgm predi_horizontale.pgm predi_moyenne.pgm image_difference.pgm\n",argv[0]);
        exit(1);
    }

    int nH,nW,nTaille,nTaille3;
    
    char nomImgIn[256];

    OCTET *ImgIn,*ImgVerticale,*ImgHorizontale,*ImgMoyenne,*ImgDifference;

    sscanf(argv[1],"%s",nomImgIn);

    lire_nb_lignes_colonnes_image_ppm(nomImgIn,&nH,&nW);

    nTaille = nH * nW;
    nTaille3 = 3 * nTaille;

    allocation_tableau(ImgIn,OCTET,nTaille3);
    lire_image_ppm(nomImgIn,ImgIn,nTaille);
    allocation_tableau(ImgVerticale, OCTET, nTaille);
    allocation_tableau(ImgHorizontale, OCTET, nTaille);
    allocation_tableau(ImgMoyenne, OCTET, nTaille);
    allocation_tableau(ImgDifference, OCTET, nTaille);

    OCTET *ImgY,*ImgCb,*ImgCr;
    allocation_tableau(ImgY, OCTET, nTaille);
    allocation_tableau(ImgCb, OCTET, nTaille);
    allocation_tableau(ImgCr, OCTET, nTaille);

    RGBtoYCbCr(ImgIn, ImgY, ImgCb, ImgCr, nTaille);

    ecrire_image_pgm("Y.pgm", ImgY, nH, nW);
    ecrire_image_pgm("Cb.pgm", ImgCb, nH, nW);
    ecrire_image_pgm("Cr.pgm", ImgCr, nH, nW);

    for(int i = 0; i < nH; i += 16) {
        for(int j = 0; j < nW; j += 16) {
            for(int di = 0; di < 16; di++) {
                for(int dj = 0; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    if (di == 0 || dj == 0) {
                        // Copier la première ligne et la première colonne directement depuis l'image d'origine
                        ImgVerticale[indexOut] = ImgY[indexOut];
                        ImgHorizontale[indexOut] = ImgY[indexOut];
                        ImgMoyenne[indexOut] = ImgY[indexOut];
                    } else {
                        int indexVertical = i * nW + (j + dj);
                        int indexHorizontal = (i + di) * nW + j;
                        ImgVerticale[indexOut] = ImgY[indexVertical];
                        ImgHorizontale[indexOut] = ImgY[indexHorizontal];
                    }
                }
            }
            int somme = 0;
            for(int di = 1; di < 16; di++) {
                somme += ImgY[(i + di) * nW + j];
            }
            for(int dj = 0; dj < 16; dj++) {
                somme += ImgY[i * nW + (j + dj)];
            }
            int moyenne = somme / 31;
            for(int di = 1; di < 16; di++) {
                for(int dj = 1; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    ImgMoyenne[indexOut] = moyenne;
                }
            }
        }
    }

    ecrire_image_pgm("image_verticale.pgm", ImgVerticale, nH, nW);
    ecrire_image_pgm("image_horizontale.pgm", ImgHorizontale, nH, nW);
    ecrire_image_pgm("image_moyenne.pgm", ImgMoyenne, nH, nW);

    for(int i = 0; i < nH; i += 16) {
        for(int j = 0; j < nW; j += 16) {
            int distance_verticale = 0;
            int distance_horizontale = 0;
            int distance_moyenne = 0;
            for(int di = 0; di < 16; di++) {
                for(int dj = 0; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    distance_verticale += pow(ImgY[indexOut] - ImgVerticale[indexOut],2);
                    distance_horizontale += pow(ImgY[indexOut] - ImgHorizontale[indexOut],2);
                    distance_moyenne += pow(ImgY[indexOut] - ImgMoyenne[indexOut],2);
                }
            }
            for(int di = 0; di < 16; di++) {
                for(int dj = 0; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    if(distance_verticale < distance_horizontale && distance_verticale < distance_moyenne) {
                        //ImgDifference[indexOut] = ImgVerticale[indexOut];
                        ImgDifference[indexOut] = 0;
                    }
                    else if(distance_horizontale < distance_verticale && distance_horizontale < distance_moyenne) {
                        //ImgDifference[indexOut] = ImgHorizontale[indexOut];
                        ImgDifference[indexOut] = 255;
                    }
                    else if(distance_moyenne < distance_verticale && distance_moyenne < distance_horizontale) {
                        //ImgDifference[indexOut] = ImgMoyenne[indexOut];
                        ImgDifference[indexOut] = 128;
                    }
                    else {
                        //ImgDifference[indexOut] = ImgY[indexOut];
                        ImgDifference[indexOut] = 128;
                    }
                }
            }
        }
    }

    ecrire_image_pgm("carte.pgm",ImgDifference,nH,nW);

    free(ImgIn);
    free(ImgY);
    free(ImgCb);
    free(ImgCr);
    free(ImgVerticale);
    free(ImgHorizontale);
    free(ImgMoyenne);
    free(ImgDifference);

    return 0;

}