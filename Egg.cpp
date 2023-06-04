#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Enemy.h"
#include "Headers/Egg.h"

/**
 * @brief Construit un nouvel oeuf
 * 
 * @param game 
 */
Egg::Egg(Game* game) :
	Enemy(game)
{
	setSprite(getSubStateSprite(EnemyRight1));
}

/**
 * @brief récupère le sprite correspondant au sous-état de l'oeuf précisé en paramètre
 * 
 * @param subState sous-état de l'oeuf
 * @return sf::Sprite* 
 */
sf::Sprite* Egg::getSubStateSprite(EnemySubState subState) const
{
	auto offset = EggDown1;
	return SPRITE((BurgerTechSprite)(subState + offset));
}