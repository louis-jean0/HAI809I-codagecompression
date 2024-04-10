dataFile = 'Y_qp15_sans_rec.dat'

set title "Histogramme de l'image de la luminance après DCT avec QP = 15"

# Choisir le terminal
set terminal png
set output 'histo_Y_qp15_sans_rec.png'

plot dataFile with lines

pause -1 'Appuyez sur une touche pour continuer'