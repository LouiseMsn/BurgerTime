#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>
#include <cmath>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Player.h"
#include "Headers/Slice.h"
#include "Headers/Enemy.h"

Enemy::Enemy(Game* game) :
	Movable(game),
	_targetBlockPosition(sf::Vector2i(-1, -1)),
	_beforeStopSpeed(0),
	_peppered(false),
	_fallingSlice(nullptr),
	_mashingSlice(nullptr),
	_lastOpacitySprite(nullptr)
{
	setType(MovableTypeEnemy);
	setSpeed(enemyBaseSpeed);
	setSize(sf::Vector2i(spriteSizeX, spriteSizeX));
}

Enemy::~Enemy()
{
	if (_lastOpacitySprite)
	{
		// on reset le niveau de transparence du dernier sprite utilise
		auto color = _lastOpacitySprite->getColor();
		color.a = 255;
		_lastOpacitySprite->setColor(color);
	}
}

void Enemy::setBlockPosition(const sf::Vector2i& block, bool checkBlocks)
{
	Movable::setBlockPosition(block, checkBlocks);
	_targetBlockPosition = block;
}

bool Enemy::isPeppered() const
{
	return _peppered;
}

bool Enemy::canKill() const
{
	// un ennemi ne peut pas tuer s'il est en entraine par un tranche
	if (_fallingSlice || _mashingSlice)
		return false;

	auto state = getState();
	return state == EnemyMovingLeft || state == EnemyMovingRight || state == EnemyMovingUp || state == EnemyMovingDown;
}

void Enemy::die()
{
	setSpeed(0);
	setState(EnemyDead);
}

// on est ecrase par une tranche
bool Enemy::mashBySlice(const Slice* slice)
{
	if (_mashingSlice || _fallingSlice)
		return false;

	_mashingSlice = slice;
	setSpeed(0);
	setState(EnemyMashedRight);
	setMovingDirection(None);
	getGame()->playSound(SoundEnemyMashed);
	return true;
}

// on tombe avec uen tranche
bool Enemy::fallWithSlice(const Slice* slice)
{
	if (_fallingSlice)
		return false;

	_fallingSlice = slice;
	setSpeed(0);
	setMovingDirection(None);
	getGame()->playSound(SoundEnemyPeppered);
	return true;
}

void Enemy::finishLevel()
{
	setSpeed(0);
	setMovingDirection(None);
	setState(EnemyLevelEnd);
	_opacityClock.restart();
}

bool Enemy::setState(int state)
{
	if (!Movable::setState(state))
		return false;

	setSubStateChangeDelay(0);

	// initalisation du substate pour chaque state
	switch (getState())
	{
	case EnemyMovingRight:
		return setSubState(EnemyRight1);

	case EnemyMovingLeft:
		return setSubState(EnemyLeft1);

	case EnemyMovingUp:
		return setSubState(EnemyUp1);

	case EnemyMovingDown:
		return setSubState(EnemyDown1);

	case EnemyPeppered:
		return setSubState(EnemyPeppered1);

	case EnemyMashedLeft:
		return setSubState(EnemyMashedLeft1);

	case EnemyMashedRight:
		return setSubState(EnemyMashedRight1);
	}
	return true;
}

// recuperation de la direction vers le joueur le plus proche
MoveDirection Enemy::getNearestPlayerDirection() const
{
	Player* player = nullptr;
	auto game = getGame();

	auto position = getPixelPosition();

	// cas facile
	auto count = game->getPlayersCount();
	if (count == 1)
	{
		player = game->getPlayer(0);
	}
	else
	{
		auto minDistance = std::numeric_limits<int>::max();
		for (auto i = 0; i < count; i++)
		{
			auto gp = game->getPlayer(i);

			// on utilise la distance a vol d'oiseau
			auto pos = gp->getPixelPosition();
			auto distance = Game::getDistance(pos - position);

			if (distance < minDistance)
			{
				minDistance = distance;
				player = gp;
			}
		}
	}

	if (!player) // 0 joueur?
		return None;

	auto vector = player->getPixelPosition() - position;

	// on utilise plutot les escaliers (y plutot que x)
	if (vector.y > 0)
		return Down;

	if (vector.y < 0)
		return Up;

	if (vector.x > 0)
		return Right;

	if (vector.x < 0)
		return Left;

	return None;
}

// du poivre arrive sur l'ennemi
void Enemy::setPeppered()
{
	if (_peppered)
		return;

	_peppered = true;
	// on sauvegarde la vitesse la toute premiere fois avant le stop
	_beforeStopSpeed = getSpeed();
	setSpeed(0);
	setState(EnemyPeppered);
	_pepperedClock.restart();
	getGame()->playSound(SoundEnemyPeppered);
}

// le poivre disparait
void Enemy::setUnPeppered()
{
	if (!_peppered)
		return;

	// si on est sur une slice on reste comme ca
	if (_fallingSlice || _mashingSlice)
		return;

	// c'est reparti
	_peppered = false;
	setSpeed(_beforeStopSpeed);
	setState(EnemyMovingRight);
}

// on bouge
void Enemy::move()
{
	// on ne bouge plus si on est mort
	auto state = getState();
	if (state == EnemyDead || state == EnemyLevelEnd || state == EnemyInLimbo)
		return;

	// gestion du poivre "dans l'air"
	auto level = getGame()->getCurrentLevel();
	auto bounds = getPixelBounds();
	if (!isPeppered() && level->isPeppered(bounds))
	{
		setPeppered();
	}
	else if (isPeppered() && _pepperedClock.getElapsedTime().asSeconds() >= pepperedWait)
	{
		setUnPeppered();
	}

	if (!getSpeed())
		return;

	// est-ce qu'on est arrive a notre bloc cible?
	auto atBlock = isAtBlock(_targetBlockPosition);
	if (!atBlock) // non, on continue de bouger dans la direction precedemment choisie
		return;

	// on recupere les directions possibles
	auto directions = getAvailableDirections();
	if (!directions.size())
		return; // hein? ca ne devrait pas arriver avec les niveaux definis

	// on ne retourne pas dans la direction opposee donc on l'enleve des possibilites
	auto dir = getOppositeDirection(getMovingDirection());
	removeDirection(directions, dir);
	if (!directions.size())
	{
		// euh, si quand on n'a pas d'autre choix
		directions.push_back(dir);
	}

	MoveDirection direction;
	if (directions.size() == 1)
	{
		// pas le choix
		direction = directions.at(0);
	}
	else
	{
		// peut on aller vers le joueur?
		direction = getNearestPlayerDirection();
		if (std::find(directions.begin(), directions.end(), direction) == directions.end())
		{
			// non, alors allons au hasard
			direction = directions.at(Game::getRandom(0, (int)directions.size() - 1));
		}
	}

	// on calcule un bloc cible en fonction de la direction choisie
	_targetBlockPosition = getBlockPosition();
	switch (direction)
	{
	case Left:
		_targetBlockPosition.x--;
		break;

	case Right:
		_targetBlockPosition.x++;
		break;

	case Up:
		_targetBlockPosition.y--;
		break;

	case Down:
		_targetBlockPosition.y++;
		break;
	}

	// on ne peut pas aller vers un bloc cible invalide, on n'y arrivera jamais
	// donc on reste ou on est et au prochain coup on ira dans l'autre sens
	if (!level->isBlockOpen(_targetBlockPosition))
	{
		_targetBlockPosition = getBlockPosition();
		return;
	}

	// tout est ok, on va vers le bloc cible
	setMovingDirection(direction);
}

bool Enemy::setMovingDirection(MoveDirection direction)
{
	if (!Movable::setMovingDirection(direction))
		return false;

	switch (direction)
	{
	case Left:
		setState(EnemyMovingLeft);
		break;

	case Right:
		setState(EnemyMovingRight);
		break;

	case Up:
		setState(EnemyMovingUp);
		break;

	case Down:
		setState(EnemyMovingDown);
		break;
	}
	return true;
}

void Enemy::drawSprite(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (getState() == EnemyLevelEnd)
	{
		// on efface l'ennemi lentement...
		if (!_lastOpacitySprite)
		{
			_lastOpacitySprite = getSprite();

		}

		if (_lastOpacitySprite)
		{
			auto color = _lastOpacitySprite->getColor();
			auto ms = _opacityClock.getElapsedTime().asMilliseconds();
			color.a = 255 - (255 * ms) / (std::max(levelFinishedTime, gameOverTime) * 1000);
			_lastOpacitySprite->setColor(color);
		}
	}
	Movable::drawSprite(window, deltaTime, states);
}

void Enemy::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	auto state = getState();
	// si on est mort il n'y a rien a faire
	if (getGame()->isPaused() || state == EnemyDead || state == EnemyLevelEnd)
	{
		Movable::update(window, deltaTime, states);
		return;
	}

	// les limbes c'est avant d'apparaitre et d'etre dangereux
	if (state == EnemyInLimbo)
	{
		auto color = getSprite()->getColor();

		// quand on est dans les limbes, l'opacite est fonction de la duree d'attente
		auto ms = _opacityClock.getElapsedTime().asMilliseconds();
		if (limboWaitTime <= 0 || ms >= (limboWaitTime * 1000))
		{
			color.a = 255;
			setState(EnemyMovingLeft);
			return;
		}

		color.a = (255 * ms) / (limboWaitTime * 1000);
		getSprite()->setColor(color);
		Movable::update(window, deltaTime, states);
		return;
	}

	if (_mashingSlice)
	{
		// on s'ecrase en tombant avec la tranche
		auto enemyPos = getPixelBounds();
		auto pixelPos = _mashingSlice->getPixelPosition();
		pixelPos.x = enemyPos.left;
		pixelPos.y -= sliceSizeY;
		setPixelPosition(pixelPos, false);
	}
	else if (_fallingSlice)
	{
		// quand la tranche est tout en bas on s'ecrase
		auto fallingSliceState = _fallingSlice->getState();
		if (fallingSliceState == SliceStacked || fallingSliceState == SliceImmobile)
		{
			// on ne peut pas etre en meme temps mashed & falling
			auto slice = _fallingSlice;
			_fallingSlice = nullptr;
			mashBySlice(slice);
			return;
		}

		// on tombe avec la tranche
		auto enemyPos = getPixelBounds();
		auto pixelPos = _fallingSlice->getPixelPosition();
		pixelPos.x = enemyPos.left;
		pixelPos.y -= sliceSizeY;
		setPixelPosition(pixelPos, false);
		Movable::update(window, deltaTime, states);
		return;
	}

	auto elapsed = getSubStateElapsedMilliseconds();
	auto delay = getSubStateChangeDelay();
	if (elapsed >= delay)
	{
		switch (getState())
		{
		case EnemyMovingRight:
			switch (getSubState())
			{
			case EnemyRight1:
				setSprite(getSubStateSprite(EnemyRight1));
				setSubState(EnemyRight2);
				break;

			case EnemyRight2:
				setSprite(getSubStateSprite(EnemyRight2));
				setSubState(EnemyRight1);
				break;
			}
			break;

		case EnemyMovingLeft:
			switch (getSubState())
			{
			case EnemyLeft1:
				setSprite(getSubStateSprite(EnemyLeft1));
				setSubState(EnemyLeft2);
				break;

			case EnemyLeft2:
				setSprite(getSubStateSprite(EnemyLeft2));
				setSubState(EnemyLeft1);
				break;
			}
			break;

		case EnemyMovingUp:
			switch (getSubState())
			{
			case EnemyUp1:
				setSprite(getSubStateSprite(EnemyUp1));
				setSubState(EnemyUp2);
				break;

			case EnemyUp2:
				setSprite(getSubStateSprite(EnemyUp2));
				setSubState(EnemyUp1);
				break;
			}
			break;

		case EnemyMovingDown:
			switch (getSubState())
			{
			case EnemyDown1:
				setSprite(getSubStateSprite(EnemyDown1));
				setSubState(EnemyDown2);
				break;

			case EnemyDown2:
				setSprite(getSubStateSprite(EnemyDown2));
				setSubState(EnemyDown1);
				break;
			}
			break;

		case EnemyPeppered:
			switch (getSubState())
			{
			case EnemyPeppered1:
				setSprite(getSubStateSprite(EnemyPeppered1));
				setSubState(EnemyPeppered2);
				break;

			case EnemyPeppered2:
				setSprite(getSubStateSprite(EnemyPeppered2));
				setSubState(EnemyPeppered1);
				break;
			}
			break;

		case EnemyMashedRight:
			switch (getSubState())
			{
			case EnemyMashedRight1:
				setSprite(getSubStateSprite(EnemyMashedRight1));
				setSubState(EnemyMashedRight2);
				break;

			case EnemyMashedRight2:
				setSprite(getSubStateSprite(EnemyMashedRight2));
				setSubState(EnemyMashedRight3);
				break;

			case EnemyMashedRight3:
				setSprite(getSubStateSprite(EnemyMashedRight3));
				setSubState(EnemyMashedRight4);
				break;

			case EnemyMashedRight4:
				setSprite(getSubStateSprite(EnemyMashedRight4));
				die();
				break;
			}
			break;

		case EnemyMashedLeft:
			switch (getSubState())
			{
			case EnemyMashedLeft1:
				setSprite(getSubStateSprite(EnemyMashedLeft1));
				setSubState(EnemyMashedLeft2);
				break;

			case EnemyMashedLeft2:
				setSprite(getSubStateSprite(EnemyMashedLeft2));
				setSubState(EnemyMashedLeft3);
				break;

			case EnemyMashedLeft3:
				setSprite(getSubStateSprite(EnemyMashedLeft3));
				setSubState(EnemyMashedLeft4);
				break;

			case EnemyMashedLeft4:
				setSprite(getSubStateSprite(EnemyMashedLeft4));
				die();
				break;
			}
			break;
		}
	}

	Movable::update(window, deltaTime, states);
	setSubStateChangeDelay(200);
}
