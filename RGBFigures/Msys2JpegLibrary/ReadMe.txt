Voici un exemple de la mani�re d'utiliser et de compiler la biblioth�que maison pour 
cr�er une image RGB-jpeg. Naviguez dans le r�pertoire de MinimalExample.c et compilez 
le code avec make. Lancez l'ex�cutable, si la compilation est r�ussie. En cas de 
probl�me : voir ci-dessous.

Exemple de fichier Makefile
---------------------------

voir exemple pour MinimalExample.c


Si la compilation de MinimalExemple.c �choue
---------------------------

Essayez de compiler le code MinimalExemple.c. Si la compilation ne fonctionne pas :
1) allez dans le r�pertoire "Libraries", et supprimez jpeg.a

2) A l'int�rieur de "Libraries" se trouve un autre r�pertoire appel� "src.x86Msys". Naviguez 
votre terminal jusqu'� ce r�pertoire et tapez "make" pour compiler le code qui s'y 
trouve. 

3) Le r�sultat de 2) est un fichier "jpeg.a" dans le r�pertoire "src.x86Msys". Copiez ce 
fichier dans le r�pertoire "Libraries", o� vous avez supprim� un jpeg.a � l'�tape 1)

4) recompiler MinimalExemple.c


Fonctions disponibles
---------------------

long int
JPEG_init_image (int width,
                 int height,
		 int R,
		 int G,
		 int B);

Cette fonction alloue l'espace m�moire n�cessaire pour manipuler une image JPEG, "width" et "height"
correspondent au nombre de pixels de l'image, horizontalement et verticalement respectivement.
En cas de succ�s, la fonction retourne une valeur "long int" permettant ensuite de manipuler l'image ;
la fonction retourne 0 en cas de probl�me.
Les pixels de l'image sont initialis�s aux couleurs "R", "G" et "B".


void
JPEG_destroy_image (long int image);

Cette fonction d�salloue l'espace m�moire allou� par la fonction JPEG_init_image ; � n'utiliser
que lorsque le programme n'a plus besoin de l'image.
Le param�tre "image" correspond � la valeur retourn�e par la fonction JPEG_init_image.


int
JPEG_save_file (char * filename,
                long int image);

Cette fonction cr�e le fichier image sur le disque, "filename" est le nom du fichier � cr�er et
"image" est la valeur retourn�e par la fonction JPEG_init_image.


void
JPEG_setpixel (int i,
               int j,
               int R,
               int G,
               int B,
               long int image);

Cette fonction permet de d�finir la couleur d'un pixel donn� dans l'image, par ses trois composantes
R, G et B. Les param�tres "i" et "j" correspondent aux positions horizontale et verticale du pixel �
modifier, "R", "G" et "B" sont �videmment les trois composantes de la couleur. Enfin le param�tre
"image" est la valeur retourn�e par la fonction JPEG_init_image.

