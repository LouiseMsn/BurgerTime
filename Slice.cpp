#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Player.h"
#include "Headers/Enemy.h"
#include "Headers/Slice.h"
#include "Headers/Burger.h"

/**
 * @brief construit une nouvelle tranche de hamburger
 * 
 * @param game jeu
 * @param burger burger auquel la tranche appartient
 * @param number numéro
 * @param type type de tranche (viande, salade, fromage..)
 * @param verticalPosition position verticale de la tranche
 */
Slice::Slice(Game* game, const Burger* burger, int number, BurgerTechSprite type, int verticalPosition) :
	Movable(game)
{
	_number = number;
	_burger = burger;
	_type = type;
	_bouncePosition = 0;
	_fallingWeight = 0;
	_verticalPosition = verticalPosition;
	for (auto i = 0; i < _chunksCount; i++)
	{
		_stepped[i] = 0;
		_chunks[i] = nullptr;
	}
	setState(SliceImmobile);
	setSpeed(sliceSpeed);
	setSize(sf::Vector2i(_sliceWidth, _sliceHeight));

	if (_type != Plate)
	{
		// on coupe la tranche en 4 morceaux
		auto scale = game->getScale();
		auto sprite = game->getSpriteSheet().getSprite(_type);
		auto rect = sprite->getTextureRect();
		rect.width = rect.width / _chunksCount + 1;
		for (auto i = 0; i < _chunksCount; i++)
		{
			_chunks[i] = new sf::Sprite(*sprite->getTexture(), rect);
			_chunks[i]->setScale((float)scale.x, (float)scale.y);
			rect.left += sliceSizeX / _chunksCount;
		}
	}
}

/**
 * @brief détruit la tranche
 * 
 */
Slice::~Slice()
{
	for (const auto& item : _chunks)
	{
		delete item;
	}
}

/**
 * @brief récupère la position verticale de la tranche
 * 
 * @return int 
 */
int Slice::getVerticalPosition() const
{
	return _verticalPosition;
}

/**
 * @brief récupère le type de tranche (fromage, salade, viande..)
 * 
 * @return BurgerTechSprite 
 */
BurgerTechSprite Slice::getSliceType() const
{
	return _type;
}

/**
 * @brief récupère le numéro de la tranche
 * 
 * @return int numéro de la tranche
 */
int Slice::getNumber() const
{
	return _number;
}

/**
 * @brief récupère le burger auquel apparatient la tranche
 * 
 * @return const Burger* 
 */
const Burger* Slice::getBurger() const
{
	return _burger;
}

/**
 * @brief prépare pour joueur
 * initialise la vitesse, position, le rebondissement de la tranche au début du niveau
 * 
 */
void Slice::prepareForPlay()
{
	_bouncePosition = 0;
	_fallingWeight = 0;
	for (auto i = 0; i < _chunksCount; i++)
	{
		_stepped[i] = 0;
	}
	setState(SliceImmobile);
	setSpeed(sliceSpeed);
}

/**
 * @brief place la tranche à une position passée en paramètre en pixel
 * 
 * @param pixelPosition 
 * @param checkBlocks 
 * @return true 
 * @return false 
 */
bool Slice::setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks)
{
	if (_pixelPosition == pixelPosition)
		return false;

	// les slices peuvent passer a travers tout donc on ne gere pas la notion de block ouvert
	_pixelPosition = pixelPosition;
	return true;
}

/**
 * @brief met la tranche dans un état 
 * 
 * @param state immobile, entrain de tomber, entrain de rebondir vers le haut ou vers le bas, empilée
 * @return true 
 * @return false 
 */
bool Slice::setState(int state)
{
	if (!Movable::setState(state))
		return false;

	if (state == SliceStacked && _type != Plate)
	{
		getGame()->playSound(SoundSliceStacked);
		getGame()->getCurrentLevel()->addStackedSlice(this, _steppingPlayer);
	}

	if (state == SliceFalling)
	{
		// on se souvient d'ou on part
		_fallingBlockPosition = getBlockPosition();
		_fallingWeight = 0;

		// son et scores
		getGame()->playSound(SoundSliceFalling);
		_steppingPlayer->addScore(sliceFallScore, getCenterPixelPosition());

		// enemmis autour?
		auto enemies = getGame()->getCurrentLevel()->getEnemiesAround(getPixelBounds());
		for (auto& item : enemies)
		{
			// on l'entraine avec nous
			if (item->fallWithSlice(this))
			{
				// ca ajoute du poids
				_fallingWeight++;
				_steppingPlayer->addScore(enemyFallWithSlice, getCenterPixelPosition());
			}
		}
	}
	return true;
}

/**
 * @brief dessine le sprite de la tranche sur la fenêtre
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
void Slice::drawSprite(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (_type == Plate)
		return;

	// on utilise des entiers pour coller aux pixels
	auto scale = getGame()->getScale();
	auto x = _pixelPosition.x * scale.x;
	auto y = (_pixelPosition.y + 2 * groundSizeY) * scale.y;
	for (auto i = 0; i < _chunksCount; i++)
	{
		_chunks[i]->setPosition((float)x, (float)(y + _stepped[i] * scale.y));
		window.draw(*_chunks[i], states);
		x += (sliceSizeX / _chunksCount) * scale.x;
	}
}

/**
 * @brief on descend les morceaux voisins qui sont au même niveau
 * les tranches sont divisées en morceaux, un appui jusqu'à 6 permet de faire tomber la tranche
 * 
 * @param chunk 
 * @param level 
 * @param player 
 */
void Slice::stepOnIfSameLevel(int chunk, int level, Player* player)
{
	if (chunk < 0 || chunk >= _chunksCount)
		return;

	if (_stepped[chunk] != level)
		return;

	_stepped[chunk]++;

	// on descend les morceaux voisins qui sont au meme niveau
	stepOnIfSameLevel(chunk - 1, _stepped[chunk], player);
	stepOnIfSameLevel(chunk + 1, _stepped[chunk], player);

	// un appui jusqu'a 6 permet de faire tomber la tranche
	if (_stepped[chunk] == 6)
	{
		_steppingPlayer = player;
		setState(SliceFalling);
		getGame()->playSound(SoundSliceFalling);
	}
}

/**
 * @brief pas du joueur
 * si la position du joueur interesecte notre tranche, on détermine le morceau et on écrase à partir du morceau le plus bas si il à déjà été écrasé
 * 
 * @param pixelBounds 
 * @param player 
 * @return true 
 * @return false 
 */
bool Slice::playerStep(const sf::IntRect& pixelBounds, Player* player)
{
	if (getState() != SliceImmobile)
		return false;

	// est-ce que la position intersecte notre tranche?
	auto sliceBounds = getPixelBounds();
	sf::IntRect intersection;
	if (!sliceBounds.intersects(pixelBounds, intersection))
		return false;

	// oui, on determine le morceau
	auto chunk = (intersection.left - sliceBounds.left) / (_sliceWidth / _chunksCount);

	// on ecrase a partir du morceau le plus bas (si deja ecrase)
	auto m = std::min_element(_stepped, _stepped + _chunksCount);
	stepOnIfSameLevel(chunk, *m, player);
	getGame()->playSound(SoundSliceStepped);
	return true;
}

/**
 * @brief forme du rebondissement de la tranche
 * 
 * @param value valeur de la hauteur du rebondissement
 */
void Slice::setBouncingShape(int value)
{
	_stepped[0] = value;
	_stepped[1] = 0;
	_stepped[2] = 0;
	_stepped[3] = value;
}

/**
 * @brief mise à jour lorsque la tranche bouge
 * 
 * @param speed 
 */
void Slice::updateWhenMoved(int speed)
{
	if (_type == Plate)
		return;

	int y;
	int offset;
	Slice* nextSlice;
	std::vector<Enemy*> enemies;
	switch (getState())
	{
	case SliceImmobile:
	case SliceStacked:
		_steppingPlayer = nullptr;
		break;

	case SliceFalling:
		enemies = getGame()->getCurrentLevel()->getEnemiesAround(getPixelBounds());
		for (auto& item : enemies)
		{
			if (item->mashBySlice(this) && _steppingPlayer)
			{
				_steppingPlayer->addScore(enemyMashedBySlice, getCenterPixelPosition());
			}
		}

		y = _pixelPosition.y + speed;
		nextSlice = _burger->getSlice(_number + 1);

		offset = 0;
		// on empile les tranches deja stockees
		if (nextSlice->_type != Plate && nextSlice->getState() == SliceStacked)
		{
			offset = -(sliceSizeY + 1);
		}

		if (y >= (nextSlice->_pixelPosition.y - 2 * groundSizeY + offset))
		{
			setBouncingShape(1);
			setState(SliceBouncingUp);
			_bouncePosition = y - sliceBounceY;

			if (nextSlice->_type == Plate)
			{
				_bouncePosition -= groundSizeY;
			}

			// on fait tomber la tranche suivante
			if (nextSlice->getState() != SliceStacked)
			{
				nextSlice->_steppingPlayer = _steppingPlayer;
				_steppingPlayer = nullptr;
				nextSlice->setState(SliceFalling);
			}
		}
		else
		{
			// si on est sur un sol on s'arrete
			// sauf si on est lourd car des ennemis sont sur la tranche
			auto level = getGame()->getCurrentLevel();
			auto blockPos = getBlockPosition();
			if (blockPos != _fallingBlockPosition)
			{
				blockPos.y--;
				if (level->isBlockGround(blockPos))
				{
					// pas de poids (ennemis) on s'arrete
					if (!_fallingWeight)
					{
						setBouncingShape(0);
						setState(SliceImmobile);
						return;
					}

					// on tombe en fonction du nombre d'ennemis sur la tranche
					_fallingWeight--;
					blockPos.y++;
					_fallingBlockPosition = blockPos;
				}
			}

			_pixelPosition.y = y;
		}
		break;

	case SliceBouncingUp:
		y = _pixelPosition.y - speed;
		if (y <= _bouncePosition)
		{
			setBouncingShape(-1);
			setState(SliceBouncingDown);
			_bouncePosition = y + sliceBounceY + 2 * groundSizeY + 1;

			// on ralentit la vitesse quand on rebondit
			setSpeed(getSpeed() * sliceBounceSpeedFactor);
		}
		else
		{
			_pixelPosition.y = y;
		}
		break;

	case SliceBouncingDown:
		y = _pixelPosition.y + speed;
		if (y >= _bouncePosition)
		{
			setBouncingShape(0);
			setSpeed(sliceSpeed);

			// on stacke la tranche sur la tranche d'en dessous
			nextSlice = _burger->getSlice(_number + 1);
			if (nextSlice->getState() == SliceStacked)
			{
				setState(SliceStacked);
			}
			else
			{
				setState(SliceImmobile);
			}
		}
		else
		{
			_pixelPosition.y = y;
		}
		break;
	}
}
