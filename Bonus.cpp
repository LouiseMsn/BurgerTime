#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Bonus.h"

Bonus::Bonus(Game* game) :
	Movable(game),
	_score(0),
	_scored(false)
{
	setSpeed(0);
	setSize(sf::Vector2i(spriteSizeX, spriteSizeX));

	auto rnd = Game::getRandom(0, 2);
	switch (rnd)
	{
	case 0:
		_score = bonus1Score;
		setSprite(SPRITE(Bonus1));
		break;

	case 1:
		_score = bonus2Score;
		setSprite(SPRITE(Bonus2));
		break;

	case 2:
		_score = bonus3Score;
		setSprite(SPRITE(Bonus3));
		break;
	}
}

int Bonus::getScore() const
{
	return _score;
}

bool Bonus::isScored() const
{
	return _scored;
}

void Bonus::setScored()
{
	_scored = true;
}
