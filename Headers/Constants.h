#pragma once

/**
 * @brief Constantes diverses pour le jeu
 * 
 */
// vitesse des objets
const float playerSpeed = 0.00004f; //  0.00013f; // 0.0001f; // vitesse du joueur
const float enemyBaseSpeed = 0.00002f; // vitesse de base des enemis (elle peut augmenter)
const float enemySpeedIncrement = 0.000002f; // pour ne pas que les ennemis se chevauchent trop visuellement
const float scoreSpeed = 0.000025f; // vitesse d'un score
const float sliceSpeed = 0.00005f; // vitesse d'une tranche
const float sliceBounceSpeedFactor = 0.6f; // facteur de reduction de la vitesse d'une tranche au rebond
const float pepperSpeed = 0.00008f; // vitesse du poivre

const int pepperSprayWait = 200; // en millisecondes, temps d'attente entre 2 poivrages
const int pepperedWait = 4; // en secondes, temps de duree d'un poivrage
const int limboWaitTime = 2; // en secondes, temps d'attente dans les limbes (au demarrage)
const int bonusShowStart = 30; // secondes, temps d'attente avant de montrer un bonus
const int bonusShowTime = 10; // secondes, temps pendant lequel un bonus est affiche
const int scoreShowTime = 2; // secondes, temps d'affichage d'un score
const int levelFinishedTime = 3; // secondes, temps d'attente quand le niveau est fini
const int gameOverTime = 5; // secondes, temps d'attente quand le jeu est fini
const int levelGameReadyTime = 3; // secondes, temps d'attente pour passer au niveau suivant (gagne) ou rejouer le niveau (perdu)

// scores
const int bonus1Score = 500;
const int bonus2Score = 1000;
const int bonus3Score = 1500;
const int sliceFallScore = 50; // score quand on fait tomber une tranche
const int enemyFallWithSlice = 150; // score quand un ennemi tombe avec une tranche
const int enemyMashedBySlice = 100; // score quand un ennemi est ecrase par une tranche
const int completeBurgerScore = 200;

// score avec 2 joueurs
const int finishLevelScore = 1000;

const int baseHiScore = 28000; // hi-score de base
const int liveBonusScore = 20000; // nouvelle vie bonus tous les X

// valeurs tirees du fichier levels.png
const int levelsCount = 6; // nombre max de niveaux
const int levelImageWidth = 208; // largeur en pixel de l'image d'un niveau
const int levelImageHeight = 200; // hauteur en pixel de l'image d'un niveau
const int levelImagePadding = 8; // zone grise entre les niveaux
const int levelBlocksSizeX = 24; // largeur entre 2 echelles
const int levelBlocksDivisionX = 8; // 16 => taille d'une echelle
const int levelBlocksPixelsY = 8; // taille d'1 bloc en pixels

const int groundSizeY = 2; // epaisseur du sol
const int sliceSizeX = 32; // largeur des tranches
const int sliceSizeY = 7; // hauteur des tranches
const int sliceBounceY = 11; // hauteur du rebond des tranches

const int spriteSizeX = 16; // largeur du joueur/bonus/ennemis
const int playerFootX = 4; // position des pieds du joueur par rapport au bord de son sprite
const int playerBelowSliceY = 4; // taille de la portion qui n'est pas importante pour toucher les tranches

const float fontSize = 7; // taille de base de la police
const char appTitle[] = "Burger Tech"; // nom de l'application
