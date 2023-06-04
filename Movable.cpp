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

/**
 * @brief Construit un nouveau Movable (entité qui peut bouger dans le jeu)
 * 
 * @param game 
 */
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

/**
 * @brief récupère la direction opposée à celle passée en paramètre
 * 
 * @param direction 
 * @return MoveDirection 
 */
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

/**
 * @brief enlève une direction spécifiée en paramètre ou retourne false
 * 
 * @param directions 
 * @param direction 
 * @return true 
 * @return false 
 */
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

/**
 * @brief vérifie que 2 directions passées en paramètre sont opposée
 * 
 * @param dir1 
 * @param dir2 
 * @return true 
 * @return false 
 */
bool Movable::areOppositeDirection(MoveDirection dir1, MoveDirection dir2)
{
	if (dir1 == None || dir2 == None)
		return false;

	return dir1 == getOppositeDirection(dir2);
}

/**
 * @brief setter du type d'objet Movable
 * 
 * @param type 
 */
void Movable::setType(MovableType type)
{
	_type = type;
}

/**
 * @brief récupère le type d'objet movable
 * 
 * @return MovableType 
 */
MovableType Movable::getType() const
{
	return _type;
}

/**
 * @brief récupère la direction du mouvement
 * 
 * @return MoveDirection 
 */
MoveDirection Movable::getMovingDirection() const
{
	return _movingDirection;
}

/**
 * @brief setter de la direction du mouvement 
 * retourne false si la movingDirection est déjà égale à la direction spécifiée en paramètre
 * retourne true sinon
 * 
 * @param direction 
 * @return true 
 * @return false 
 */
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

/**
 * @brief récupère la dernière direction du mouvement
 * 
 * @return MoveDirection 
 */
MoveDirection Movable::getLastMovingDirection() const
{
	return _lastMovingDirection;
}

/**
 * @brief setter de la dernière direction du mouvement
 * retourne true 
 * 
 * @param direction 
 * @return true 
 * @return false 
 */
bool Movable::setLastMovingDirection(MoveDirection direction)
{
	_lastMovingDirection = direction;
	return true;
}

/**
 * @brief récupère le délai du changement de sous-état
 * 
 * @return int 
 */
int Movable::getSubStateChangeDelay() const
{
	return _subStateChangeDelay;
}

/**
 * @brief setter du délai du changement de sous-état
 * 
 * @param subStateChangeDelay 
 */
void Movable::setSubStateChangeDelay(int subStateChangeDelay)
{
	_subStateChangeDelay = subStateChangeDelay;
}

/**
 * @brief redémarre la clock des sous-état
 * 
 */
void Movable::restartSubStateClock()
{
	_subStateClock.restart();
}

/**
 * @brief retourne le temps écoulé en millisecodndes pour le sous état
 * 
 * @return int 
 */
int Movable::getSubStateElapsedMilliseconds() const
{
	return _subStateClock.getElapsedTime().asMilliseconds();
}

/**
 * @brief récupère l'état
 * 
 * @return int 
 */
int Movable::getState() const
{
	return _state;
}

/**
 * @brief récupère le sous état (plusieurs animations pour un état donné)
 * 
 * @return int 
 */
int Movable::getSubState() const
{
	return _subState;
}

/**
 * @brief setter de l'état
 * retourne false si l'état était déjà le bon
 * retourne true sinon
 * 
 * @param state 
 * @return true 
 * @return false 
 */
bool Movable::setState(int state)
{
	if (state == _state)
		return false;

	_state = state;
	return true;
}

/**
 * @brief setter du sous-état 
 * une fois fait, la clock redémarre
 * retourne false si l'état était déjà le bon
 * retourne true sinon
 * 
 * @param state 
 * @return true 
 * @return false 
 */
bool Movable::setSubState(int subState)
{
	if (subState == _subState)
		return false;

	_subState = subState;
	restartSubStateClock();
	return true;
}

/**
 * @brief récupère le sprite
 * 
 * @return sf::Sprite* 
 */
sf::Sprite* Movable::getSprite() const
{
	return _sprite;
}

/**
 * @brief récupère les bords
 * 
 * @return sf::IntRect 
 */
sf::IntRect Movable::getPixelBounds() const
{
	auto pos = getPixelPosition();
	return sf::IntRect(pos, _size);
}

/**
 * @brief récupère la position en pixels
 * 
 * @return sf::Vector2i 
 */
sf::Vector2i Movable::getPixelPosition() const
{
	return _pixelPosition;
}

/**
 * @brief récupère la position en pixel du centre
 * 
 * @return sf::Vector2i 
 */
sf::Vector2i Movable::getCenterPixelPosition() const
{
	auto pos = _pixelPosition;
	pos.x += _size.x / 2;
	pos.y += _size.y / 2;
	return pos;
}

/**
 * @brief récupère la position du bloc
 * 
 * @return sf::Vector2i 
 */
sf::Vector2i Movable::getBlockPosition() const
{
	return Level::getBlockPosition(_pixelPosition);
}

/**
 * @brief vérifie que l'objet Movable se trouve au bloc spécifié par blockPosition
 * 
 * @param blockPosition 
 * @return true 
 * @return false 
 */
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

/**
 * @brief vérifie si il y à collision entre deux movable avec une tolérance spécifée en paramètre
 * 
 * @param movable avec lequel on vérfie la collision
 * @param tolerance tolérance qui détermine à quelle moment on considère qu'il y a collision
 * @return true 
 * @return false 
 */
bool Movable::collides(const Movable* movable, int tolerance) const
{
	return collides(movable->getPixelBounds(), tolerance);
}

/**
 * @brief setter de la taille de l'objet Movable
 * 
 * @param size 
 */
void Movable::setSize(const sf::Vector2i& size)
{
	_size = size;
}

/**
 * @brief setter du sprite du Movable
 * 
 * @param sprite 
 */
void Movable::setSprite(sf::Sprite* sprite)
{
	_sprite = sprite;
	setPixelPosition(_pixelPosition);
}

/**
 * @brief change la position en pixel à celle spécifiée en paramètre
 * retourne false si la position actuelle est la même que celle spécifiée 
 * ceci inclus notamment de se replacer lorsque le Movable se trouve dans des endroits interdis à cause de la tolérence
 * 
 * @param pixelPosition 
 * @param checkBlocks 
 * @return true 
 * @return false 
 */
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
		// est on bien placés sur une une echelle?
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

/**
 * @brief récupère toutes les directions disponibles selon la position du Movable
 * 
 * @return std::vector<MoveDirection> 
 */
std::vector<MoveDirection> Movable::getAvailableDirections() const
{
	auto level = getGame()->getCurrentLevel();
	auto pos = getBlockPosition();
	return level->getAvailableDirections(pos);
}

/**
 * @brief mouvement en fonction de la touche appuyée par l'utilisateur, passée en paramètre
 * 
 * @param key appuyée
 */
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

/**
 * @brief dessine le sprite sur la fenêtre
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
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

/**
 * @brief met à jour lorsque le Movable bouge
 * se termine si il n'y à pas de movingDirection (qu'elle vaut none)
 * 
 * @param speed 
 */
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

/**
 * @brief mise à jour du Movable
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
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
