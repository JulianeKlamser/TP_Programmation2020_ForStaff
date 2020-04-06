Le code RGBMiniExamp.c est un exemple simple de création de fichiers de données, que Gnuplot peut transformer en une image RGB. 

Les informations de l'image sont stockées dans un Martix de dimension (TotalPixX fois TotalPixY).
Il y a trois paramètres ( Red, Blue et Green ) pour chaque pixel avec les coordonnées (x, y).
Le code RGBMiniExamp.c écrit les données de l'image dans des fichiers.

Le fichier PixelPlot.plt est un script Gnuplot. Ouvrez-le pour comprendre ce qu'il fait (utiliser votre éditeur de code ou l'éditeur de texte de base de votre ordinateur).
Les données créées par RGBMiniExamp.c peuvent être tracées (pour Win 10, Mac) en tapant dans votre terminal (il faut que votre terminal soit dans le même dossier que PixelPlot)

gnuplot PixelPlot.plt

Win 7/8:
Ouvrez votre application gnuplot. Cliquez sur le symbole représentant un dossier avec 
une étoile rouge ("changer de répertoire") et naviguez vers le répertoire avec le code 
et surtout les données. Cliquez ensuite sur le symbole représentant un dossier avec 
une flèche noire ("Open script file") et choisissez PixelPlot.plt.