Akash HOSSAIN
Astyax NOUREL
L3MI

Sauf pour la commande formatage, on suppose que les fichiers d'entree sont bien
formates. On ne le verifie pas, ou sinon partiellement, quand ca peut
generer des bugs. Par exemple, si je veux trier un csm selon le 2 eme champ,
le programme ne verifiera pas que le csm est formate, mais il verifiera au moins
que chaque ligne a au moins 2 champs.

Les fichiers csm d'entree doivent toujours se finir par un saut de ligne de fin de fichier.
Par defaut, on supposera le fichier de sortie "a.csm" s'il n'est pas specifie dans la commande.

Lorsqu'une commande qui peut lire un csm a partir
de l'entree standard est lancee sur l'entree standard (sans redirection),
Il est requis d'indiquer le caractere separateur au debut. On ne peut
pas ne rien mettre en disant implicitement que ce caractere sera ; par defaut.
Dans les fonctions de lecture de fichier, quand ce cas est atteint on revient en arriere
avec fseek. On ne peut pas le faire avec stdin.
Il n'y a pas de probleme a declarer implicitement le caractere separateur
si les commandes sont lancees sur des fichiers ou avec une redirection.

La commande formatage est implementee sans probleme.

La commande tri est implementee. Si le champ selon lequel faire le tri
n'est pas indique ou n'est pas un nombre, le tri se fera selon le champ 0
par defaut. On autorise exceptionnellement les options inconnues pour cette commande.

La commande fusion est implementee. J'ajoute une option -f qui change
la methode selon laquelle fusionner des lignes. Par defaut,
on fait juste leur concatenation.
Avec l'option -f, on fait une fusion moins redondante
que j'explique dans le code de fusion.c .

La commande union est implementee. On supposera qu'au moins un fichier d'entree doit etre specifie et que l'entree standart ne sera pas consideree comme un fichier d'entree.

La commande select est implementee sans probleme.

La commande moyenne_ponderee est implementee. On supposera qu'une note sera comprise entre 0.00 et 20.00 dans le but d'eviter une allocation de memoire consequente.
