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

Sausage::Sausage(Game* game) :
	Enemy(game)
{
	setSprite(getSubStateSprite(EnemyRight1));
}

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

sf::Sprite* Sausage::getSubStateSprite(EnemySubState subState) const
{
	auto offset = SausageDown1;
	return SPRITE((BurgerTechSprite)(subState + offset));
}
