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
#include "Headers/Sausage.h"

/**
 * @brief construit une nouvelle saucisse
 * 
 * @param game 
 */
Sausage::Sausage(Game* game) :
	Enemy(game)
{
	setSprite(getSubStateSprite(EnemyRight1));
}

/**
 * @brief la saucisse meurt écrasée par une tranche qui tombe
 * 
 */
void Sausage::dieMashed()
{
	auto direction = getMovingDirection();
	if (direction == Left)
	{
		// note: les sprites "left" n'existent que pour la saucisse
		setState(EnemyMashedLeft);
	}
	else
	{
		setState(EnemyMashedRight);
	}
}

/**
 * @brief récupère le sprite correspondant au sous-état spécifié en paramètre
 * 
 * @param subState 
 * @return sf::Sprite* 
 */
sf::Sprite* Sausage::getSubStateSprite(EnemySubState subState) const
{
	auto offset = SausageDown1;
	return SPRITE((BurgerTechSprite)(subState + offset));
}
