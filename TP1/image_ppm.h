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

void planV(OCTET *pt_image, OCTET *src, int taille_image){
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

double distance_euclidienne_ppm(OCTET pixel1[3], OCTET pixel2[3]) {
   return sqrt(pow(pixel2[0] - pixel1[0],2) +
               pow(pixel2[1] - pixel1[1],2) +
               pow(pixel2[2] - pixel1[2],2));
}

void k_mean_2(OCTET* ImgIn, OCTET* ImgOutDebut, OCTET* ImgOutFinale, int nTaille, OCTET couleur1[3], OCTET couleur2[3]) {

   bool changement;
   int* clusterAssignments = (int*)malloc(nTaille*sizeof(int));
   memset(clusterAssignments,0,nTaille*sizeof(int));
   bool debut = true;
   int cpt = 0;
   do {
      changement = false;
      cpt++;
      for(int i = 0; i < nTaille; i++) {
         int currentPixelIdx = i * 3;
         OCTET currentPixel[3] = {ImgIn[currentPixelIdx],ImgIn[currentPixelIdx+1],ImgIn[currentPixelIdx+2]};

         double distance_pixel_couleur1 = distance_euclidienne_ppm(currentPixel,couleur1);
         double distance_pixel_couleur2 = distance_euclidienne_ppm(currentPixel,couleur2);

         int newCluster = distance_pixel_couleur1 < distance_pixel_couleur2 ? 1 : 2;

         if(clusterAssignments[i] != newCluster) {
            clusterAssignments[i] = newCluster;
            changement = true;
         }
      }

      for(int i = 0; i < nTaille; i++) {
         int currentPixelIdx = 3 * i;
         if(clusterAssignments[i] == 1) {
            ImgOutFinale[currentPixelIdx] = couleur1[0];
            ImgOutFinale[currentPixelIdx+1] = couleur1[1];
            ImgOutFinale[currentPixelIdx+2] = couleur1[2];
         }
         else {
            ImgOutFinale[currentPixelIdx] = couleur2[0];
            ImgOutFinale[currentPixelIdx+1] = couleur2[1];
            ImgOutFinale[currentPixelIdx+2] = couleur2[2];
         }
      }

      // Sortie de l'image de la première itération
      if(debut) {
         for(int i = 0; i < nTaille; i++) {
            int currentPixelIdx = 3 * i;
            ImgOutDebut[currentPixelIdx] = ImgOutFinale[currentPixelIdx];
            ImgOutDebut[currentPixelIdx+1] = ImgOutFinale[currentPixelIdx+1];
            ImgOutDebut[currentPixelIdx+2] = ImgOutFinale[currentPixelIdx+2];
         }
         debut = false;
      }

      // Mise à jour des centroïdes
      
      int sum1[3] = {0}, sum2[3] = {0};
      int count1 = 0, count2 = 0;

      for(int i = 0; i < nTaille; i++) {
         int currentPixelIdx = i * 3;
         if (clusterAssignments[i] == 1) {
               sum1[0] += ImgOutFinale[currentPixelIdx];
               sum1[1] += ImgOutFinale[currentPixelIdx + 1];
               sum1[2] += ImgOutFinale[currentPixelIdx + 2];
               count1++;
         } else {
               sum2[0] += ImgOutFinale[currentPixelIdx];
               sum2[1] += ImgOutFinale[currentPixelIdx + 1];
               sum2[2] += ImgOutFinale[currentPixelIdx + 2];
               count2++;
         }
      }

      if(count1 > 0) {
         couleur1[0] = sum1[0] / count1;
         couleur1[1] = sum1[1] / count1;
         couleur1[2] = sum1[2] / count1;
      }

      if(count2 > 0) {
         couleur2[0] = sum2[0] / count2;
         couleur2[1] = sum2[1] / count2;
         couleur2[2] = sum2[2] / count2;
      }
   } while(changement);

   printf("Itérations : %d\n",cpt);

   printf("Couleur 1 : %d %d %d\n",couleur1[0],couleur1[1],couleur1[2]);
   printf("Couleur 2 : %d %d %d\n",couleur2[0],couleur2[1],couleur2[2]);
}