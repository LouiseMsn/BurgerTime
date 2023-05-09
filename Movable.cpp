#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Headers/Constants.h"
#include "Headers/Updatable.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"

Movable::Movable(Game* game) :
	Updatable(game),
	_state(-1),
	_subState(-1),
	_subStateChangeDelay(0),
	_sprite(nullptr),
	_movingDirection(None),
	_lastMovingDirection(None),
	_type(MovableTypeUnspecified)
{
}

MoveDirection Movable::getOppositeDirection(MoveDirection direction)
{
	switch (direction)
	{
	case Left:
		return Right;

	case Right:
		return Left;

	case Up:
		return Down;

	case Down:
		return Up;

	default:
		return direction;
	}
}

bool Movable::removeDirection(std::vector<MoveDirection>& directions, MoveDirection direction)
{
	auto find = std::find(directions.begin(), directions.end(), direction);
	if (find != directions.end())
	{
		directions.erase(find);
		return true;
	}
	return false;
}

bool Movable::areOppositeDirection(MoveDirection dir1, MoveDirection dir2)
{
	if (dir1 == None || dir2 == None)
		return false;

	return dir1 == getOppositeDirection(dir2);
}

void Movable::setType(MovableType type)
{
	_type = type;
}

MovableType Movable::getType() const
{
	return _type;
}

MoveDirection Movable::getMovingDirection() const
{
	return _movingDirection;
}

bool Movable::setMovingDirection(MoveDirection direction)
{
	if (_movingDirection == direction)
		return false;

	if (_movingDirection != None)
	{
		_lastMovingDirection = _movingDirection;
	}
	_movingDirection = direction;
	return true;
}

MoveDirection Movable::getLastMovingDirection() const
{
	return _lastMovingDirection;
}

bool Movable::setLastMovingDirection(MoveDirection direction)
{
	_lastMovingDirection = direction;
	return true;
}

int Movable::getSubStateChangeDelay() const
{
	return _subStateChangeDelay;
}

void Movable::setSubStateChangeDelay(int subStateChangeDelay)
{
	_subStateChangeDelay = subStateChangeDelay;
}

void Movable::restartSubStateClock()
{
	_subStateClock.restart();
}

int Movable::getSubStateElapsedMilliseconds() const
{
	return _subStateClock.getElapsedTime().asMilliseconds();
}

int Movable::getState() const
{
	return _state;
}

int Movable::getSubState() const
{
	return _subState;
}

bool Movable::setState(int state)
{
	if (state == _state)
		return false;

	_state = state;
	return true;
}

bool Movable::setSubState(int subState)
{
	if (subState == _subState)
		return false;

	_subState = subState;
	restartSubStateClock();
	return true;
}

sf::Sprite* Movable::getSprite() const
{
	return _sprite;
}

sf::IntRect Movable::getPixelBounds() const
{
	auto pos = getPixelPosition();
	return sf::IntRect(pos, _size);
}

sf::Vector2i Movable::getPixelPosition() const
{
	return _pixelPosition;
}

sf::Vector2i Movable::getCenterPixelPosition() const
{
	auto pos = _pixelPosition;
	pos.x += _size.x / 2;
	pos.y += _size.y / 2;
	return pos;
}

sf::Vector2i Movable::getBlockPosition() const
{
	return Level::getBlockPosition(_pixelPosition);
}

bool Movable::isAtBlock(const sf::Vector2i& blockPosition) const
{
	auto blockPos = Level::getBlockPixelPosition(blockPosition);
	return blockPos.x == _pixelPosition.x && blockPos.y == _pixelPosition.y + levelBlocksPixelsY;
}

bool Movable::collides(const sf::IntRect pixelBounds, int tolerance) const
{
	auto bounds = getPixelBounds();
	if (tolerance < 0)
	{
		// on elargit le test
		bounds.left += tolerance;
		bounds.top += tolerance;
		bounds.width += 2 * tolerance;
		bounds.height += 2 * tolerance;
	}

	sf::IntRect intersection;
	auto intersects = bounds.intersects(pixelBounds, intersection);
	if (intersects && tolerance > 0)
	{
		if (intersection.width <= tolerance || intersection.height <= tolerance)
			return false;
	}
	return intersects;
}

bool Movable::collides(const Movable* movable, int tolerance) const
{
	return collides(movable->getPixelBounds(), tolerance);
}

void Movable::setSize(const sf::Vector2i& size)
{
	_size = size;
}

void Movable::setSprite(sf::Sprite* sprite)
{
	_sprite = sprite;
	setPixelPosition(_pixelPosition);
}

bool Movable::setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks)
{
	if (_pixelPosition == pixelPosition)
		return false;

	if (!checkBlocks)
	{
		_pixelPosition.x = pixelPosition.x;
		_pixelPosition.y = pixelPosition.y;
		return true;
	}

	auto level = getGame()->getCurrentLevel();
	auto bounds = sf::IntRect(pixelPosition, sf::Vector2i(_size.x, levelBlocksPixelsY));
	auto tolerance = 3; // faire varier la tolerance en fonction de la vitesse?
	if (!level->isBlockOpen(bounds, tolerance))
		return false;

	auto dir = None;
	if (pixelPosition.x > _pixelPosition.x)
	{
		dir = Right;
	}
	else if (pixelPosition.x < _pixelPosition.x)
	{
		dir = Left;
	}
	else if (pixelPosition.y > _pixelPosition.y)
	{
		dir = Down;
	}
	else if (pixelPosition.y < _pixelPosition.y)
	{
		dir = Up;
	}

	// si on n'est pas sur le sol et pas sur une echelle (a cause de la tolerance) on se replace
	auto finalPos = pixelPosition;
	if (dir == Left || dir == Right)
	{
		// est on bien place le sol?
		auto m = finalPos.y % levelBlocksPixelsY;
		if (m < levelBlocksPixelsY / 2)
		{
			finalPos.y -= m;
		}
		else
		{
			finalPos.y += levelBlocksPixelsY - m;
		}
	}

	if (dir == Up || dir == Down)
	{
		// est on bien places une echelle?
		auto m = finalPos.x % levelBlocksSizeX;
		if (m < levelBlocksSizeX / 2)
		{
			finalPos.x -= m;
		}
		else
		{
			finalPos.x += levelBlocksSizeX - m;
		}
	}

	// la tolerance peut nous faire viser un bloc non autorise
	sf::Vector2i finalBlock;
	switch (dir)
	{
	case Left:
	case Up:
		// coin gauche / haut
		finalBlock = Level::getBlockPosition(finalPos);
		break;

	case Right:
		finalBlock = Level::getBlockPosition(sf::Vector2i(finalPos.x + _size.x - 1, finalPos.y));
		break;

	case Down:
		finalBlock = Level::getBlockPosition(sf::Vector2i(finalPos.x, finalPos.y + levelBlocksPixelsY - 1));
		break;
	}
	if (!level->isBlockOpen(finalBlock))
		return false;

	_pixelPosition.x = finalPos.x;
	_pixelPosition.y = finalPos.y;
	return true;
}

std::vector<MoveDirection> Movable::getAvailableDirections() const
{
	auto level = getGame()->getCurrentLevel();
	auto pos = getBlockPosition();
	return level->getAvailableDirections(pos);
}

void Movable::moveFromKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Left:
		setMovingDirection(Left);
		break;

	case sf::Keyboard::Right:
		setMovingDirection(Right);
		break;

	case sf::Keyboard::Up:
		setMovingDirection(Up);
		break;

	case sf::Keyboard::Down:
		setMovingDirection(Down);
		break;

	default:
		setMovingDirection(None);
		break;
	}
}

void Movable::drawSprite(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (!_sprite)
		return;

	auto scale = getGame()->getScale();
	auto x = _pixelPosition.x * scale.x;
	auto y = _pixelPosition.y * scale.y;
	_sprite->setPosition((float)x, (float)y);
	window.draw(*_sprite, states);
}

void Movable::updateWhenMoved(int speed)
{
	if (_movingDirection == None)
		return;

	auto position = _pixelPosition;
	switch (_movingDirection)
	{
	case Left:
		position.x += -speed;
		break;

	case Right:
		position.x += speed;
		break;

	case Up:
		position.y += -speed;
		break;

	case Down:
		position.y += speed;
		break;
	}
	setPixelPosition(position);
}

void Movable::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	Updatable::update(window, deltaTime, states);
	drawSprite(window, deltaTime, states);
}

void Movable::setBlockPosition(const sf::Vector2i& block, bool checkBlocks)
{
	auto pos = Level::getBlockPixelPosition(block);
	pos.y -= levelBlocksPixelsY;
	setPixelPosition(pos, checkBlocks);
}

void Movable::finishLevel()
{
}
