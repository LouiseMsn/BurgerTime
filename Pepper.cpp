#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Pepper.h"

/**
 * @brief Construit un nouveau poivre
 * 
 * @param game 
 * @param direction 
 */
Pepper::Pepper(Game* game, MoveDirection direction)
	:Movable(game)
{
	setType(MovableTypePepper);
	_clock.restart();
	setSpeed(pepperSpeed);
	setSize(sf::Vector2i(spriteSizeX, spriteSizeX));
	setSubStateChangeDelay(100);
	setMovingDirection(direction);
	setState(PepperState1);
	move();
}

/**
 * @brief bouge le poivre selon la direction dans laquelle il est lancé
 * le poivre à 4 sprite et donc 4 états
 * 
 */
void Pepper::move()
{
	auto state = getState();
	if (state == PepperFinished)
	{
		setMovingDirection(None);
		return;
	}

	BurgerTechSprite sprite;
	auto dir = getMovingDirection();
	switch (dir)
	{
	case Left:
		sprite = PepperLeft1;
		break;

	case Right:
		sprite = PepperRight1;
		break;

	case Up:
		sprite = PepperUp1;
		break;

	default: // case Down:
		sprite = PepperDown1;
		break;
	}

	// PepperState est cale sur les sprites (4 etats => 4 sprites)
	auto i = (int)sprite;
	i += state;
	sprite = (BurgerTechSprite)i;
	setSprite(SPRITE(sprite));
	restartSubStateClock();
}

/**
 * @brief setter de la position du poivre
 * le poivre peut passer à travers tout, donc on ne gère pas la notion de bloc ouvert dans cette fonction
 * 
 * @param pixelPosition 
 * @param checkBlocks 
 * @return true 
 * @return false 
 */
bool Pepper::setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks)
{
	if (_pixelPosition == pixelPosition)
		return false;

	// le poivre peut passer à travers tout, donc on ne gère pas la notion de bloc ouvert
	_pixelPosition = pixelPosition;
	return true;
}

/**
 * @brief mise à jour du poivre
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
void Pepper::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (getGame()->isPaused())
	{
		Movable::update(window, deltaTime, states);
		return;
	}

	auto elapsed = getSubStateElapsedMilliseconds();
	auto delay = getSubStateChangeDelay();
	if (elapsed >= delay)
	{
		switch (getState())
		{
		case PepperState1:
			setState(PepperState2);
			break;

		case PepperState2:
			setState(PepperState3);
			break;

		case PepperState3:
			setState(PepperState4);
			break;

		case PepperState4:
			setState(PepperFinished);
			break;
		}
		move();
	}

	Movable::update(window, deltaTime, states);
}
