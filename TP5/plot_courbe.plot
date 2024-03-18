# Définir le nom du fichier de données
dataFile = 'results.dat'

# Définir les titres des axes et la légende
set xlabel 'qp'
set ylabel 'Distance'
set title 'Distances RMSE et de Hausdorff en fonction de qp'

# Activer la légende
set key outside

# Choisir le terminal
set terminal png
set output 'rmse_hausdorff.png'

# Tracer les données RMSE et Hausdorff depuis le fichier
plot dataFile using 1:2 with linespoints title 'RMSE', \
     dataFile using 1:3 with linespoints title 'Hausdorff'

# Pause à la fin du script pour maintenir la fenêtre ouverte
pause -1 'Appuyez sur une touche pour continuer'
