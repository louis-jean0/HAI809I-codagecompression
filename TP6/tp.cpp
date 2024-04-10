#include "image_ppm.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

void mul_mat(std::vector<std::vector<float>> &a, std::vector<std::vector<float>> &b, std::vector<std::vector<float>> &c) {
    if(a[0].size() != b.size()) {
        std::cerr<<"Erreur : le nombre de colonnes de la première matrice est différent du nombre de lignes de la seconde matrice"<<std::endl;
        exit(EXIT_FAILURE);
    }
    int nb_lignes_a = a.size();
    int nb_lignes_b = b.size();
    int nb_colonnes_b = b[0].size();
    c.resize(nb_lignes_a);
    for(int i = 0; i < nb_lignes_a; ++i) {
        c[i].resize(nb_colonnes_b);
    }
    for(int i = 0; i < nb_lignes_a; ++i) {
        for(int j = 0; j < nb_colonnes_b; ++j) {
            c[i][j] = 0;
            for(int k = 0; k < nb_lignes_b; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void mul_scalaire_mat(std::vector<std::vector<float>> &a, std::vector<std::vector<float>> &b, std::vector<std::vector<float>> &c) {
    if(a.size() != b.size() || a[0].size() != b[0].size()) {
        std::cerr<<"Erreur : les deux matrices à multiplier ne sont pas de même taille"<<std::endl;
        exit(EXIT_FAILURE);
    }
    int size0 = a.size();
    int size1 = a[0].size();
    c.resize(size0);
    for(int i = 0; i < size0; ++i) {
        c[i].resize(size1);
    }
    for(int i = 0; i < size0; ++i) {
        for(int j = 0; j < size1; ++j) {
            c[i][j] = a[i][j] * b[i][j];
        }
    }
}

float calculer_Qstep(int QP) {
        float Qstep = 0.625;
        for(int i = 0; i < QP; i++) {
            Qstep *= pow(2.0,1.0/6.0);
        }
        return Qstep;
    }

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Utilisation : %s image_in1.ppm image_in2.ppm\n",argv[0]);
        exit(1);
    }

    int nH,nW,nTaille,nTaille3;
    
    char nomImgIn1[256];

    OCTET *ImgIn,*ImgVerticale,*ImgHorizontale,*ImgMoyenne,*ImgDifference,*ImgPrediction;

    sscanf(argv[1],"%s",nomImgIn1);

    lire_nb_lignes_colonnes_image_ppm(nomImgIn1,&nH,&nW);

    nTaille = nH * nW;
    nTaille3 = 3 * nTaille;

    allocation_tableau(ImgIn,OCTET,nTaille3);
    lire_image_ppm(nomImgIn1,ImgIn,nTaille);
    allocation_tableau(ImgVerticale, OCTET, nTaille);
    allocation_tableau(ImgHorizontale, OCTET, nTaille);
    allocation_tableau(ImgMoyenne, OCTET, nTaille);
    allocation_tableau(ImgDifference, OCTET, nTaille);
    allocation_tableau(ImgPrediction, OCTET, nTaille);

    /****************************************** Ouverture de la seconde image et conversion en YCbCr ******************************************/

    char nomImgIn2[256];
    sscanf(argv[2], "%s", nomImgIn2);

    OCTET *ImgIn2;
    allocation_tableau(ImgIn2, OCTET, nTaille3); // Même taille que la première image
    lire_image_ppm(nomImgIn2, ImgIn2, nTaille);

    OCTET *ImgY2,*ImgCb2,*ImgCr2;
    allocation_tableau(ImgY2, OCTET, nTaille);
    allocation_tableau(ImgCb2, OCTET, nTaille);
    allocation_tableau(ImgCr2, OCTET, nTaille);

    RGBtoYCbCr(ImgIn2, ImgY2, ImgCb2, ImgCr2, nTaille);

    ecrire_image_pgm((char*)"Y2.pgm", ImgY2, nH, nW);
    ecrire_image_pgm((char*)"Cb2.pgm", ImgCb2, nH, nW);
    ecrire_image_pgm((char*)"Cr2.pgm", ImgCr2, nH, nW);

    /****************************************** Conversion YCbCr de la première image ******************************************/

    OCTET *ImgY,*ImgCb,*ImgCr;
    allocation_tableau(ImgY, OCTET, nTaille);
    allocation_tableau(ImgCb, OCTET, nTaille);
    allocation_tableau(ImgCr, OCTET, nTaille);

    RGBtoYCbCr(ImgIn, ImgY, ImgCb, ImgCr, nTaille);

    ecrire_image_pgm((char*)"Y.pgm", ImgY, nH, nW);
    ecrire_image_pgm((char*)"Cb.pgm", ImgCb, nH, nW);
    ecrire_image_pgm((char*)"Cr.pgm", ImgCr, nH, nW);

    /****************************************** Découpage en macroblocs et prédiction spatiale ******************************************/

    OCTET *ImgVerticale2,*ImgHorizontale2,*ImgMoyenne2,*ImgPrediction2;
    allocation_tableau(ImgVerticale2, OCTET, nTaille);
    allocation_tableau(ImgHorizontale2, OCTET, nTaille);
    allocation_tableau(ImgMoyenne2, OCTET, nTaille);
    allocation_tableau(ImgPrediction2, OCTET, nTaille);

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
                        ImgVerticale2[indexOut] = ImgY2[indexOut];
                        ImgHorizontale2[indexOut] = ImgY2[indexOut];
                        ImgMoyenne2[indexOut] = ImgY2[indexOut];
                    } else {
                        int indexVertical = i * nW + (j + dj);
                        int indexHorizontal = (i + di) * nW + j;
                        ImgVerticale[indexOut] = ImgY[indexVertical];
                        ImgHorizontale[indexOut] = ImgY[indexHorizontal];
                        ImgVerticale2[indexOut] = ImgY2[indexVertical];
                        ImgHorizontale2[indexOut] = ImgY2[indexHorizontal];
                    }
                }
            }
            int somme = 0;
            int somme2 = 0;
            for(int di = 1; di < 16; di++) {
                somme += ImgY[(i + di) * nW + j];
                somme2 += ImgY2[(i + di) * nW + j];
            }
            for(int dj = 0; dj < 16; dj++) {
                somme += ImgY[i * nW + (j + dj)];
                somme2 += ImgY2[i * nW + (j + dj)];
            }
            int moyenne = somme / 31;
            int moyenne2 = somme2 / 31;
            for(int di = 1; di < 16; di++) {
                for(int dj = 1; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    ImgMoyenne[indexOut] = moyenne;
                    ImgMoyenne2[indexOut] = moyenne2;
                }
            }
        }
    }

    ecrire_image_pgm((char*)"image_verticale.pgm", ImgVerticale, nH, nW);
    ecrire_image_pgm((char*)"image_horizontale.pgm", ImgHorizontale, nH, nW);
    ecrire_image_pgm((char*)"image_moyenne.pgm", ImgMoyenne, nH, nW);

    /****************************************** Décision sur la prédiction à faire ******************************************/

    for(int i = 0; i < nH; i += 16) {
        for(int j = 0; j < nW; j += 16) {
            int distance_verticale = 0;
            int distance_horizontale = 0;
            int distance_moyenne = 0;
            int distance_verticale2 = 0;
            int distance_horizontale2 = 0;
            int distance_moyenne2 = 0;
            for(int di = 0; di < 16; di++) {
                for(int dj = 0; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    distance_verticale += pow(ImgY[indexOut] - ImgVerticale[indexOut],2);
                    distance_horizontale += pow(ImgY[indexOut] - ImgHorizontale[indexOut],2);
                    distance_moyenne += pow(ImgY[indexOut] - ImgMoyenne[indexOut],2);
                    distance_verticale2 += pow(ImgY2[indexOut] - ImgVerticale2[indexOut],2);
                    distance_horizontale2 += pow(ImgY2[indexOut] - ImgHorizontale2[indexOut],2);
                    distance_moyenne2 += pow(ImgY2[indexOut] - ImgMoyenne2[indexOut],2);
                }
            }
            for(int di = 0; di < 16; di++) {
                for(int dj = 0; dj < 16; dj++) {
                    int indexOut = (i + di) * nW + (j + dj);
                    if(distance_verticale < distance_horizontale && distance_verticale < distance_moyenne) {
                        ImgPrediction[indexOut] = ImgVerticale[indexOut];
                        //ImgPrediction[indexOut] = 0;
                    }
                    else if(distance_horizontale < distance_verticale && distance_horizontale < distance_moyenne) {
                        ImgPrediction[indexOut] = ImgHorizontale[indexOut];
                        //ImgPrediction[indexOut] = 255;
                    }
                    else if(distance_moyenne < distance_verticale && distance_moyenne < distance_horizontale) {
                        ImgPrediction[indexOut] = ImgMoyenne[indexOut];
                        //ImgPrediction[indexOut] = 128;
                    }
                    else {
                        ImgPrediction[indexOut] = ImgY[indexOut];
                        //ImgPrediction[indexOut] = 128;
                    }

                    if(distance_verticale2 < distance_horizontale2 && distance_verticale2 < distance_moyenne2) {
                        ImgPrediction2[indexOut] = ImgVerticale2[indexOut];
                    }
                    else if(distance_horizontale2 < distance_verticale2 && distance_horizontale2 < distance_moyenne2) {
                        ImgPrediction2[indexOut] = ImgHorizontale2[indexOut];
                    }
                    else if(distance_moyenne2 < distance_verticale2 && distance_moyenne2 < distance_horizontale2) {
                        ImgPrediction2[indexOut] = ImgMoyenne2[indexOut];
                    }
                    else {
                        ImgPrediction2[indexOut] = ImgY2[indexOut];
                    }
                }
            }
        }
    }

    ecrire_image_pgm((char*)"image_prediction.pgm",ImgPrediction,nH,nW);
        ecrire_image_pgm((char*)"image_prediction2.pgm",ImgPrediction2,nH,nW);


    /****************************************** Différence entre l'image de la luminance et l'image prédite ******************************************/

    for(int i = 0; i < nTaille; i++) {
        int diff = ImgY[i] - ImgPrediction[i] + 128;
        if(diff < 0) {
            diff = 0;
        }
        if(diff > 255) {
            diff = 255;
        }
        ImgDifference[i] = diff;
    }

    ecrire_image_pgm((char*)"image_difference.pgm", ImgDifference, nH, nW);

    /****************************************** DCT ******************************************/

    float *ImgPredictionDCT_float;
    allocation_tableau(ImgPredictionDCT_float, float, nTaille);
    OCTET *ImgPredictionDCT;
    allocation_tableau(ImgPredictionDCT, OCTET, nTaille);

    float a = 0.5;
    float b = sqrt((float)2/5);
    float ab = a*b;
    float a2 = a*a;
    float b2 = b*b;

    std::vector<std::vector<float>> matrice_scaling =       {{a2,ab/2,a2,ab/2},
                                                            {ab/2,b2/4,ab/2,b2/4},
                                                            {a2,ab/2,a2,ab/2},
                                                            {ab/2,b2/4,ab/2,b2/4}};

    std::vector<std::vector<float>> matrice_coefficients1 =  {{1,1,1,1},
                                                            {2,1,-1,-2},
                                                            {1,-1,-1,1},
                                                            {1,-2,2,-1}};

    std::vector<std::vector<float>> matrice_coefficients2 =  {{1,2,1,1},
                                                            {1,1,-1,-2},
                                                            {1,-1,-1,2},
                                                            {1,-2,1,-1}};                                                  
    
    // Application de la DCT sur chaque sous-macroblock 4x4 de l'image prédite
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgPrediction[(i + di) * nW + (j + dj)];
                }
            }
            // DCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_dct;
            mul_mat(matrice_coefficients1, sous_macroblock, etape1);
            mul_mat(etape1, matrice_coefficients2, etape2);
            mul_scalaire_mat(etape2, matrice_scaling, sous_macroblock_dct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgPredictionDCT_float[(i + di) * nW + (j + dj)] = sous_macroblock_dct[di][dj];
                }
            }
        }
    }

    ImgPredictionDCT = (OCTET*)ImgPredictionDCT_float;

    ecrire_image_pgm((char*)"image_prediction_dct.pgm", ImgPredictionDCT, nH, nW);

    float *ImgY_DCT_float;
    allocation_tableau(ImgY_DCT_float, float, nTaille);
    OCTET* ImgY_DCT;
    allocation_tableau(ImgY_DCT, OCTET, nTaille);

    // Application de la DCT sur chaque sous-macroblock 4x4 de l'image originale (Y)
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgY[(i + di) * nW + (j + dj)];
                }
            }
            // DCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_dct;
            mul_mat(matrice_coefficients1, sous_macroblock, etape1);
            mul_mat(etape1, matrice_coefficients2, etape2);
            mul_scalaire_mat(etape2, matrice_scaling, sous_macroblock_dct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgY_DCT_float[(i + di) * nW + (j + dj)] = sous_macroblock_dct[di][dj];
                }
            }
        }
    }

    ImgY_DCT = (OCTET*)ImgY_DCT_float;

    ecrire_image_pgm((char*)"image_Y_dct.pgm", ImgY_DCT, nH, nW);

    /****************************************** IDCT ******************************************/

    OCTET* ImgPredictionDCT_IDCT;
    allocation_tableau(ImgPredictionDCT_IDCT, OCTET, nTaille);

    std::vector<std::vector<float>> matrice_scaling_inverse = {{a2,ab,a2,ab},
                                                                {ab,b2,ab,b2},
                                                                {a2,ab,a2,ab},
                                                                {ab,b2,ab,b2}};

    std::vector<std::vector<float>> matrice_coefficients_inverse1 = {{1,1,1,0.5},
                                                                    {1,0.5,-1,-1},
                                                                    {1,-0.5,-1,1},
                                                                    {1,-1,1,-0.5}};

    std::vector<std::vector<float>> matrice_coefficients_inverse2 = {{1,1,1,1},
                                                                    {1,0.5,-0.5,-1},
                                                                    {1,-1,-1,1},
                                                                    {0.5,-1,1,-0.5}};                                                               

    // Application de l'IDCT sur chaque sous-macroblock 4x4 de l'image prédite après transformation DCT
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgPredictionDCT_float[(i + di) * nW + (j + dj)];
                }
            }
            // IDCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_idct;
            mul_scalaire_mat(sous_macroblock, matrice_scaling_inverse, etape1);
            mul_mat(matrice_coefficients_inverse1, etape1, etape2);
            mul_mat(etape2, matrice_coefficients_inverse2, sous_macroblock_idct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgPredictionDCT_IDCT[(i + di) * nW + (j + dj)] = sous_macroblock_idct[di][dj];
                }
            }
        }
    }

    ecrire_image_pgm((char*)"image_predi_dct_idct.pgm", ImgPredictionDCT_IDCT, nH, nW);

    OCTET *ImgY_DCT_IDCT;
    allocation_tableau(ImgY_DCT_IDCT, OCTET, nTaille);

    // Application de l"IDCT sur chaque sous-macroblock 4x4 de l'image de luminance après transformation DCT
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgY_DCT_float[(i + di) * nW + (j + dj)];
                }
            }
            // IDCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_idct;
            mul_scalaire_mat(sous_macroblock, matrice_scaling, etape1);
            mul_mat(matrice_coefficients_inverse1, etape1, etape2);
            mul_mat(etape2, matrice_coefficients_inverse2, sous_macroblock_idct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgY_DCT_IDCT[(i + di) * nW + (j + dj)] = sous_macroblock_idct[di][dj];
                }
            }
        }
    }

    ecrire_image_pgm((char*)"image_Y_dct_idct.pgm", ImgY_DCT_IDCT, nH, nW);

    /****************************************** QP ******************************************/

    int QP = 25; // Facteur de quantification
    float Qstep = calculer_Qstep(QP);

    std::vector<std::vector<float>> matrice_scaling_qp = matrice_scaling;
    std::vector<std::vector<float>> matrice_scaling_inverse_qp = matrice_scaling_inverse;

    for(int i = 0; i < matrice_scaling_qp.size(); i++) {
        for(int j = 0; j < matrice_scaling_qp[i].size(); j++) {
            matrice_scaling_qp[i][j] /= Qstep;
            matrice_scaling_inverse_qp[i][j] *= Qstep;
        }
    }

    // Application de la DCT sur chaque sous-macroblock 4x4 de l'image prédite avec QP
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgPrediction[(i + di) * nW + (j + dj)];
                }
            }
            // DCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_dct;
            mul_mat(matrice_coefficients1, sous_macroblock, etape1);
            mul_mat(etape1, matrice_coefficients2, etape2);
            mul_scalaire_mat(etape2, matrice_scaling_qp, sous_macroblock_dct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgPredictionDCT_float[(i + di) * nW + (j + dj)] = round(sous_macroblock_dct[di][dj]);
                }
            }
        }
    }

    // Application de l'IDCT sur chaque sous-macroblock 4x4 de l'image prédite après transformation DCT avec QP
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgPredictionDCT_float[(i + di) * nW + (j + dj)];
                }
            }
            // IDCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_idct;
            mul_scalaire_mat(sous_macroblock, matrice_scaling_inverse_qp, etape1);
            mul_mat(matrice_coefficients_inverse1, etape1, etape2);
            mul_mat(etape2, matrice_coefficients_inverse2, sous_macroblock_idct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgPredictionDCT_IDCT[(i + di) * nW + (j + dj)] = sous_macroblock_idct[di][dj];
                }
            }
        }
    }

    ImgPredictionDCT = (OCTET*)ImgPredictionDCT_float;

    ecrire_image_pgm((char*)"predi_qp15_sans_rec.pgm", ImgPredictionDCT, nH, nW);
    ecrire_image_pgm((char*)"image_predi_qp.pgm", ImgPredictionDCT_IDCT, nH, nW);

    // Application de la DCT sur chaque sous-macroblock 4x4 de l'image de la luminance avec QP
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgY[(i + di) * nW + (j + dj)];
                }
            }
            // DCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_dct;
            mul_mat(matrice_coefficients1, sous_macroblock, etape1);
            mul_mat(etape1, matrice_coefficients2, etape2);
            mul_scalaire_mat(etape2, matrice_scaling_qp, sous_macroblock_dct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgY_DCT_float[(i + di) * nW + (j + dj)] = round(sous_macroblock_dct[di][dj]);
                }
            }
        }
    }

    // Application de l'IDCT sur chaque sous-macroblock 4x4 de l'image de la luminance après transformation DCT avec QP
    for(int i = 0; i < nH; i += 4) {
        for(int j = 0; j < nW; j += 4) {
            std::vector<std::vector<float>> sous_macroblock(4,std::vector<float>(4,0));
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    sous_macroblock[di][dj] = ImgY_DCT_float[(i + di) * nW + (j + dj)];
                }
            }
            // IDCT
            std::vector<std::vector<float>> etape1;
            std::vector<std::vector<float>> etape2;
            std::vector<std::vector<float>> sous_macroblock_idct;
            mul_scalaire_mat(sous_macroblock, matrice_scaling_inverse_qp, etape1);
            mul_mat(matrice_coefficients_inverse1, etape1, etape2);
            mul_mat(etape2, matrice_coefficients_inverse2, sous_macroblock_idct);
            for(int di = 0; di < 4; di++) {
                for(int dj = 0; dj < 4; dj++) {
                    ImgY_DCT_IDCT[(i + di) * nW + (j + dj)] = sous_macroblock_idct[di][dj];
                }
            }
        }
    }

    ImgY_DCT = (OCTET*)ImgY_DCT_float;

    ecrire_image_pgm((char*)"Y_qp15_sans_rec.pgm", ImgY_DCT, nH, nW);
    ecrire_image_pgm((char*)"image_Y_qp.pgm", ImgY_DCT_IDCT, nH, nW);

    // /****************************************** Découpage en macroblocs et prédiction temporelle entre les deux images ******************************************/

    int k = 64; // Rayon de recherche (en pixels) de voisins

    struct Vecteur {
        int x,y;
    };

    Vecteur *vecteurs_mouvement;
    allocation_tableau(vecteurs_mouvement, Vecteur, (nH/16) * (nW/16)); // 1 vecteur par macrobloc

    for(int i = 0; i < nH; i += 16) {
        for(int j = 0; j < nW; j += 16) {
            Vecteur meilleur_vecteur = {0,0};
            int meilleure_sad = std::numeric_limits<int>::max();
            for(int dy = -k; dy <= k; dy++) {
                for(int dx = -k; dx <= k; dx++) {
                    int sad = 0; // Somme des différences des valeurs absolues
                    for(int di = 0; di < 16; di++) {
                        for(int dj = 0; dj < 16; dj++) {
                            if((i + di + dy) < nH && (j + dj + dx) < nW && (i + di + dy) >= 0 && (j + dj + dx) >= 0) {
                                sad += abs(ImgPrediction2[(i + di) * nW + (j + dj)] - ImgPrediction[(i + di + dy) * nW + (j + dj + dx)]);
                            }
                        }
                    }
                    if (sad < meilleure_sad) {
                        meilleure_sad = sad;
                        meilleur_vecteur = {dx,dy};
                    }
                }
            }
            vecteurs_mouvement[(i/16) * (nW/16) + (j/16)] = meilleur_vecteur;
        }
    }

    OCTET *ImgPredictionTemporelle;
    allocation_tableau(ImgPredictionTemporelle, OCTET, nTaille);

    for(int i = 0; i < nH; i += 16) {
        for(int j = 0; j < nW; j += 16) {
            Vecteur v = vecteurs_mouvement[(i/16)*(nW/16) + (j/16)];
            for(int di = 0; di < 16; di++) {
                for(int dj = 0; dj < 16; dj++) {
                    int x_source = j + dj + v.x;
                    int y_source = i + di + v.y;
                    if(x_source >= 0 && x_source < nW && y_source >= 0 && y_source < nH) {
                        ImgPredictionTemporelle[(i + di) * nW + (j + dj)] = ImgPrediction[y_source * nW + x_source];
                    } else {
                        ImgPredictionTemporelle[(i+di)*nW + (j+dj)] = ImgY2[(i+di)*nW + (j+dj)];
                    }
                }
            }
        }
    }

    ecrire_image_pgm((char*)"prediction_temporelle.pgm", ImgPredictionTemporelle, nH, nW);

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