Le code RGBMiniExamp.c est un exemple simple de création de fichiers de données, que Gnuplot peut transformer en une image RGB. 

Les informations de l'image sont stockées dans un Martix de dimension (TotalPixX fois TotalPixY).
Il y a trois paramètres ( Red, Blue et Green ) pour chaque pixel avec les coordonnées (x, y).
Le code RGBMiniExamp.c écrit les données de l'image dans des fichiers.

Le fichier PixelPlot est un script Gnuplot. Ouvrez-le pour comprendre ce qu'il fait.
Les données créées par RGBMiniExamp.c peuvent être tracées en tapant dans votre terminal (il faut que votre terminal soit dans le même dossier que PixelPlot)

gnuplot PixelPlot