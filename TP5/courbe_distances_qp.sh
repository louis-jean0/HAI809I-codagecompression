#!/bin/bash

# Nom du fichier de sortie
outputFile="results.dat"

# Entête du fichier de résultats
echo "# qp RMSE Hausdorff" > $outputFile

# Chemin vers le dataset et l'exécutable
datasetPath="dataset3D/bunny.ply"
executablePath="./tp"

# Boucle pour qp de 1 à 30
for qp in $(seq 1 30); do
    echo $qp
    echo "Running with qp=$qp"
    result=$($executablePath $datasetPath $qp)
    
    # Extraire les distances RMSE et Hausdorff
    rmse=$(echo "$result" | grep "Distance RMSE" | awk '{print $4}')
    hausdorff=$(echo "$result" | grep "Distance Hausdorff" | awk '{print $4}')
    
    # Ajouter au fichier de résultats
    echo "$qp $rmse $hausdorff" >> $outputFile

done

echo "Results stored in $outputFile"