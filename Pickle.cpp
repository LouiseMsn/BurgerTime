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
#include "Headers/Pickle.h"

Pickle::Pickle(Game* game) :
	Enemy(game)
{
	setSprite(getSubStateSprite(EnemyRight1));
}

sf::Sprite* Pickle::getSubStateSprite(EnemySubState subState) const
{
	auto offset = PickleDown1;
	return SPRITE((BurgerTechSprite)(subState + offset));
}
