Voici un exemple de la manière d'utiliser et de compiler la bibliothèque maison pour 
créer une image RGB-jpeg. Naviguez dans le répertoire de MinimalExample.c et compilez 
le code avec make. Lancez l'exécutable, si la compilation est réussie. En cas de 
problème : voir ci-dessous.

Exemple de fichier Makefile
---------------------------

voir exemple pour MinimalExample.c


Si la compilation de MinimalExemple.c échoue
---------------------------

Essayez de compiler le code MinimalExemple.c. Si la compilation ne fonctionne pas :
1) allez dans le répertoire "Libraries", et supprimez jpeg.a

2) A l'intérieur de "Libraries" se trouve un autre répertoire appelé "src.x86Msys". Naviguez 
votre terminal jusqu'à ce répertoire et tapez "make" pour compiler le code qui s'y 
trouve. 

3) Le résultat de 2) est un fichier "jpeg.a" dans le répertoire "src.x86Msys". Copiez ce 
fichier dans le répertoire "Libraries", où vous avez supprimé un jpeg.a à l'étape 1)

4) recompiler MinimalExemple.c


Fonctions disponibles
---------------------

long int
JPEG_init_image (int width,
                 int height,
		 int R,
		 int G,
		 int B);

Cette fonction alloue l'espace mémoire nécessaire pour manipuler une image JPEG, "width" et "height"
correspondent au nombre de pixels de l'image, horizontalement et verticalement respectivement.
En cas de succès, la fonction retourne une valeur "long int" permettant ensuite de manipuler l'image ;
la fonction retourne 0 en cas de problème.
Les pixels de l'image sont initialisés aux couleurs "R", "G" et "B".


void
JPEG_destroy_image (long int image);

Cette fonction désalloue l'espace mémoire alloué par la fonction JPEG_init_image ; à n'utiliser
que lorsque le programme n'a plus besoin de l'image.
Le paramètre "image" correspond à la valeur retournée par la fonction JPEG_init_image.


int
JPEG_save_file (char * filename,
                long int image);

Cette fonction crée le fichier image sur le disque, "filename" est le nom du fichier à créer et
"image" est la valeur retournée par la fonction JPEG_init_image.


void
JPEG_setpixel (int i,
               int j,
               int R,
               int G,
               int B,
               long int image);

Cette fonction permet de définir la couleur d'un pixel donné dans l'image, par ses trois composantes
R, G et B. Les paramètres "i" et "j" correspondent aux positions horizontale et verticale du pixel à
modifier, "R", "G" et "B" sont évidemment les trois composantes de la couleur. Enfin le paramètre
"image" est la valeur retournée par la fonction JPEG_init_image.

