// ----------------------------------------------------------------------------
// Filename        : image_ppm.c
// Description     :
// Created On      : Tue Mar 31 13:26:36 2005
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define allocation_tableau(nom, type, nombre) \
if( (nom = (type*) calloc (nombre, sizeof(type) ) ) == NULL ) \
{\
 printf("\n Allocation dynamique impossible pour un pointeur-tableau \n");\
 exit(1);\
}

typedef unsigned char OCTET;

/*===========================================================================*/
void ignorer_commentaires(FILE * f)
{
  unsigned char c;
  while((c=fgetc(f)) == '#')
    while((c=fgetc(f)) != '\n');
  fseek(f, -sizeof(unsigned char), SEEK_CUR);
}
/*===========================================================================*/


/*===========================================================================*/
void ecrire_image_ppm(char  nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
   FILE *f_image;
   int taille_image = 3*nb_colonnes * nb_lignes;

   if( (f_image = fopen(nom_image, "wb")) == NULL)
      {
	 printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fprintf(f_image,"P6\r") ;                               /*ecriture entete*/
	 fprintf(f_image,"%d %d\r255\r", nb_colonnes, nb_lignes) ;

	 if( (fwrite((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     != (size_t)(taille_image))
	    {
	       printf("\nErreur d'ecriture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}
/*===========================================================================*/

/*===========================================================================*/		
void lire_nb_lignes_colonnes_image_ppm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
   FILE *f_image;
   int max_grey_val;

   /* cf : l'entete d'une image .ppm : P6                   */
   /*				       nb_colonnes nb_lignes */
   /*    			       max_grey_val          */


   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fscanf(f_image, "P6 ");
	 ignorer_commentaires(f_image);
	 fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
	 fclose(f_image);
      }
}
/*===========================================================================*/
/*===========================================================================*/
void lire_image_ppm(char  nom_image[], OCTET *pt_image, int taille_image)
{
   FILE *f_image;
   int  nb_colonnes, nb_lignes, max_grey_val;
   taille_image=3*taille_image;

   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	fscanf(f_image, "P6 ");
	ignorer_commentaires(f_image);
	fscanf(f_image, "%d %d %d%*c",
	       &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

	 if( (fread((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     !=  (size_t)(taille_image))
	    {
	       printf("\nErreur de lecture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}

/*===========================================================================*/
/*===========================================================================*/

void planR(OCTET *pt_image, OCTET *src, int taille_image){
   int i;
   for (i=0; i<taille_image; i++){
      pt_image[i]=src[3*i];
      }
   }
   
/*===========================================================================*/
/*===========================================================================*/

void planG(OCTET *pt_image, OCTET *src, int taille_image){
   int i;
   for (i=0; i<taille_image; i++){
      pt_image[i]=src[3*i+1];
      }
   }   

/*===========================================================================*/
/*===========================================================================*/

void planB(OCTET *pt_image, OCTET *src, int taille_image){
   int i;
   for (i=0; i<taille_image; i++){
      pt_image[i]=src[3*i+2];
      }
   }
   
/*===========================================================================*/   
/*===========================================================================*/

void ecrire_image_pgm(char  nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
   FILE *f_image;
   int taille_image = nb_colonnes * nb_lignes;

   if( (f_image = fopen(nom_image, "wb")) == NULL)
      {
	 printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fprintf(f_image,"P5\r") ;                               /*ecriture entete*/
	 fprintf(f_image,"%d %d\r255\r", nb_colonnes, nb_lignes) ;

	 if( (fwrite((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     != (size_t) taille_image)
	    {
	       printf("\nErreur de lecture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}
/*===========================================================================*/

void lire_nb_lignes_colonnes_image_pgm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
   FILE *f_image;
   int max_grey_val;

   /* cf : l'entete d'une image .pgm : P5                    */
   /*				       nb_colonnes nb_lignes */
   /*    			       max_grey_val          */


   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	 fscanf(f_image, "P5 ");
	 ignorer_commentaires(f_image);
	 fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
	 fclose(f_image);
      }
}
/*===========================================================================*/
/*===========================================================================*/
void lire_image_pgm(char  nom_image[], OCTET *pt_image, int taille_image)
{
   FILE *f_image;
   int  nb_colonnes, nb_lignes, max_grey_val;

   if( (f_image = fopen(nom_image, "rb")) == NULL)
      {
	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
	 exit(EXIT_FAILURE);
      }
   else
      {
	fscanf(f_image, "P5 ");
	ignorer_commentaires(f_image);
	fscanf(f_image, "%d %d %d%*c",
	       &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

	 if( (fread((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
	     !=  (size_t) taille_image)
	    {
	       printf("\nErreur de lecture de l'image %s \n", nom_image);
	       exit(EXIT_FAILURE);
	    }
	 fclose(f_image);
      }
}
/*===========================================================================*/
int indiceImage(int x, int y, int width) {
   if (x >= width) {
      printf("x trop grand/n");
   }
   return y * width + x;
}
/*===========================================================================*/

/*===========================================================================*/
int indiceImageCouleur(char C, int x, int y, int width) {

   int indice;

   switch (C)
   {
      case 'R':
         indice = 0;
      break;

      case 'G':
         indice = 1;
      break;

      case 'B':
         indice = 2;
      break;

      default:
         perror("Canal non précisé");

   }

   return (3 * ((y * width) + x)) + indice;

}
/*===========================================================================*/

/*===========================================================================*/
void erosion_binaire(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {

   for(int x = 1; x < nW - 1; x++) { // Bons indices pour respecter les problèmes sur les bords
        for(int y = 1; y < nH - 1; y++) {
            int vDroite = ImgIn[indiceImage(x + 1, y, nW)];
            int vGauche = ImgIn[indiceImage(x - 1, y, nW)];
            int vBas = ImgIn[indiceImage(x, y + 1, nW)];
            int vHaut = ImgIn[indiceImage(x, y - 1, nW)];

            if(vDroite || vGauche || vHaut || vBas) {
               ImgOut[indiceImage(x, y, nW)] = 255;
            }

            else {
               ImgOut[indiceImage(x, y, nW)] = 0;
            }
         }
   }
}
/*===========================================================================*/

/*===========================================================================*/
void dilatation_binaire(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW) {

   for(int x = 1; x < nW - 1; x++) { // Bons indices pour respecter les problèmes sur les bords
      for(int y = 1; y < nH - 1; y++) {
         int vDroite = ImgIn[indiceImage(x + 1, y, nW)];
         int vGauche = ImgIn[indiceImage(x - 1, y, nW)];
         int vBas = ImgIn[indiceImage(x, y + 1, nW)];
         int vHaut = ImgIn[indiceImage(x, y - 1, nW)];

         if((vDroite == 0 || vGauche == 0 || vHaut == 0 || vBas == 0)) {
            ImgOut[indiceImage(x, y, nW)] = 0;
         }

         else {
            ImgOut[indiceImage(x, y, nW)]  = 255;
         }
      }
   }
}
/*===========================================================================*/

/*===========================================================================*/
double distance_euclidienne_ppm(OCTET pixel1[3], OCTET pixel2[3]) {
   return sqrt(pow(pixel2[0] - pixel1[0],2) +
               pow(pixel2[1] - pixel1[1],2) +
               pow(pixel2[2] - pixel1[2],2));
}
/*===========================================================================*/

/*===========================================================================*/
void echantillonner_image_facteur_4(OCTET* ImgIn, OCTET* ImgOut, int nH, int nW) { // Divise le nombre de lignes et de colonnes par 2
   OCTET pixel1, pixel2, pixel3, pixel4, pixel_moyen;
   for(int i = 0; i < nH / 2; i++) {
      for(int j = 0; j < nW / 2; j++) {
         pixel1 = ImgIn[indiceImage(2*i,2*j,nW)];
         pixel2 = ImgIn[indiceImage(2*i,2*j+1,nW)];
         pixel3 = ImgIn[indiceImage(2*i+1,2*j,nW)];
         pixel4 = ImgIn[indiceImage(2*i+1,2*j+1,nW)];
         pixel_moyen = (pixel1 + pixel2 + pixel3 + pixel4) / 4;
         ImgOut[indiceImage(i,j,nW/2)] = pixel_moyen; 
      }
   }
}
/*===========================================================================*/

/*===========================================================================*/
void re_echantillonner_image_facteur_4(OCTET* ImgIn, OCTET* ImgOut, int nH, int nW) {
   OCTET pixel1, pixel2, pixel3, pixel4, pixel_interpole;
   for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
         int x = i/2;
         int y = j/2;
         pixel1 = ImgIn[indiceImage(x,y,nW/2)];
         if(x+1 < nH/2) pixel2 = ImgIn[indiceImage(x+1,y,nW/2)]; else pixel2 = pixel1;
         if(y+1 < nW/2) pixel3 = ImgIn[indiceImage(x,y+1,nW/2)]; else pixel3 = pixel1;
         if(x+1 < nH/2 && y+1 < nW/2) pixel4 = ImgIn[indiceImage(x+1,y+1,nW/2)]; else pixel4 = pixel1;
         pixel_interpole = (pixel1 + pixel2 + pixel3 + pixel4) / 4;
         ImgOut[indiceImage(i,j,nW)] = pixel_interpole;
      }
   }
}
/*===========================================================================*/

/*===========================================================================*/
void reconstruire_ppm(OCTET* ImgR, OCTET* ImgG, OCTET* ImgB, OCTET* ImgOut, int nTaille) {
   for(int i = 0; i < nTaille; i++) {
         ImgOut[3*i] = ImgR[i];
         ImgOut[3*i+1] = ImgG[i];
         ImgOut[3*i+2] = ImgB[i];
      }
}
/*===========================================================================*/

/*===========================================================================*/
void RGBtoYCbCr(OCTET* ImgIn, OCTET* ImgY, OCTET* ImgCb, OCTET* ImgCr, int nTaille) {
   OCTET *ImgR, *ImgG, *ImgB;

   allocation_tableau(ImgR,OCTET,nTaille);
   allocation_tableau(ImgG,OCTET,nTaille);
   allocation_tableau(ImgB,OCTET,nTaille);

   planR(ImgR,ImgIn,nTaille);
   planG(ImgG,ImgIn,nTaille);
   planB(ImgB,ImgIn,nTaille);

   for(int i = 0; i < nTaille; i++) {
      if(ImgY != NULL) ImgY[i] = 0.299*ImgR[i] + 0.587*ImgG[i] + 0.114*ImgB[i];
      if(ImgCb != NULL) ImgCb[i] = -0.1687*ImgR[i] - 0.3313*ImgG[i] + 0.5*ImgB[i] + 128;
      if(ImgCr != NULL) ImgCr[i] = 0.5*ImgR[i] - 0.4187*ImgG[i] - 0.0813*ImgB[i] + 128;
   }
}
/*===========================================================================*/

/*===========================================================================*/
void reconstruire_ppm_depuis_YCbCr(OCTET* ImgY, OCTET* ImgCb, OCTET* ImgCr, OCTET* ImgOut, int nTaille) {
    for (int i = 0; i < nTaille; i++) {
        int Y = ImgY[i];
        int Cb = ImgCb[i];
        int Cr = ImgCr[i];

        int R = std::min(std::max(0, Y + static_cast<int>(1.402 * (Cr - 128))), 255);
        int G = std::min(std::max(0, Y - static_cast<int>(0.34414 * (Cb - 128)) - static_cast<int>(0.71414 * (Cr - 128))), 255);
        int B = std::min(std::max(0, Y + static_cast<int>(1.772 * (Cb - 128))), 255);

        ImgOut[3*i] = static_cast<OCTET>(R);
        ImgOut[3*i+1] = static_cast<OCTET>(G);
        ImgOut[3*i+2] = static_cast<OCTET>(B);
    }
}
/*===========================================================================*/

/*===========================================================================*/
int clamp(OCTET value, OCTET min, OCTET max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
/*===========================================================================*/

double PSNR_pgm(OCTET *ImgOriginale, OCTET *Img2, int nH, int nW) {
   int nTaille = nH * nW;
   double MSE = 0.0;
    for(int i = 0; i < nTaille; i++) {
        MSE += pow(ImgOriginale[i] - Img2[i],2);
    }
    MSE /= nTaille;

    double PSNR = 10.0 * log10((255 * 255) / MSE);
    return PSNR;
}