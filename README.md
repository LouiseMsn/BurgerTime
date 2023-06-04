# BurgerTech
![Screenshot from 2023-05-09 10-53-45](https://user-images.githubusercontent.com/108003990/237046133-a7b44b22-62d4-43a8-8105-92abe2f17c17.png)

**Louise MANSON, Alice MOURIER**

## Installation
Installation de la biliothèque SFML avec: <br>
``
sudo apt-get install libsfml-dev
``
<br>
Puis se placer dans le dossier BurgerTech
Taper la commande ``make``
puis la commande ``./BurgerTech`` pour exécuter le programme

**Options de lancement** (à ajouter apres ``./BurgerTech``):

**-l** _nombre de vies_ <br>
**-p** _nombre de poivres_ <br>
**-s** _échelle_ <br>
**-ws** _style de fenêtre_<br>
**-w** _largeur de la fenêtre_<br>
**-h** _hauteur de la fenêtre_<br>
**-t** _niveau de départ_<br>
**-so 0** ou **1** (_activer ou couper le son_)<br>


## Jouer à BurgerTech
BurgerTech est un jeu basé sur le jeu d'arcade BurgerTime. 
Le but du jeu est de marcher sur les différentes tranches du burger pour les faire tomber jusqu'à l'assiette tout en évitant les saucisses, les oeufs et les cornichons. <br>
Un niveau est complété lorsque tous les burgers sont complétés. <br>
Lancer du poivre sur un ennemi paralyse l'ennemi pendant quelques secondes. <br>
Des bonus apparaissent aussi sur la carte sous la forme de frites, glace ou café. Les attraper vous ajoute des points. <br>

### Commandes pour 1 joueur
Le personnage se contrôle en utilisant les touches [←]	[↑]	[→] et [↓].
Utiliser [Ctrl Droit] pour lancer du poivre sur les ennemis 

### Commandes pour 2 joueurs
- Joueur 1 :
  - Le personnage se contrôle en utilisant les touches [←]	[↑]	[→] et [↓].
  - Utiliser [Ctrl Droit] pour lancer du poivre sur les ennemis.
- Joueur 2 :
  - Le personnage se contrôle en utilisant les touches [Z]	[Q]	[S] et [D].
  - Utiliser [Ctrl Gauche] pour lancer du poivre sur les ennemis.

## Fonctionnalités additionnelles / debug
### Depuis le jeu
**F1** à **F6** : _changer de niveau_<br>
\+  : _ajoute des ennemis_<br>
\- : _supprime des ennemis_<br>
**F11** : _termine le joueur 1_<br>
**F12** : _termine le joueur 2_<br>

### Lancement des tests
Utiliser ``make [NOM DE LA CLASSE]Test`` pour générer l'éxecutable de test de la classe puis le lancer avec ``./[NOM DE LA CLASSE]Test``
Il est aussi possible de régénérer ces exécutables en utilisant ``make clean_tests`` puis la commande make précédente.

## Documentation
Nous avons réalisé la documentation à l'aide de Doxygen sur Linux. Afin d'y accéder, télécharger le dossier intitulé **Documentation**, puis ouvrir le fichier **index.html**

## Commentaires sur le projet

**Ce qui ne marche pas dans le jeu**
- Les tranches ont parfois du mal à tomber lorsqu'on marche dessus

**Points d'interêt**
- Il a été difficile de régler la vitesse du jeu pour que la vitesse du jeu ne dépende pas de la vitesse de la machine sur laquelle on joue d'oû la méthode "update" de la classe Updatable. On utilise des entier pour les pixels, mais les événements (comme le fait d'appuyer sur une flèche directionnelle) arrvient en microsecondes et dépendent de la machine. Donc on les accumule juqu'à ce qu'ils dépassent un seuil, et on met à jour à ce moment là.

**Répartition du travail dans le binôme**
- Nous avons fait l'UML ensemble sur un draw.io partagé.
- Nous nous sommes réparti les classes à programmer à avons soumis les changements sur GitHub, nous avons cependant perdu une partie de l'historique de commits car nous avons par erreur créé une branche 'main' et une branche 'master' et faute de réussir à les merger nous avons du supprimer l'ancienne branche et son historique de commits.
- Nous tenons à préciser que nous avons reçu de l'aide pour la programmation (d'un parent fan de jeux d'arcade - et de programmation !), sans quoi nous n'auriont pas réussi à programmer un jeu aussi complet !
- Alice à réalisé la documentation sur Doxygen
- Louise à recolorisé les sprites d'origine et écrit les tests unitaires : ils sont relativement réduits faute de temps
