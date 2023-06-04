#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Headers/Constants.h"
#include "Headers/Updatable.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Movable.h"
#include "Headers/Slice.h"
#include "Headers/Burger.h"
#include "Headers/Enemy.h"
#include "Headers/Egg.h"
#include "Headers/Sausage.h"
#include "Headers/Pickle.h"
#include "Headers/Player.h"
#include "Headers/Bonus.h"
#include "Headers/Pepper.h"
#include "Headers/Score.h"
#include "Headers/Level.h"
#include "Headers/Game.h"

const int blocksCountX = 17;
const int blocksCountY = 23;
const int arrayBlocksX = blocksCountX + 2;
const int arrayBlocksY = blocksCountY + 2;

const int burgerMaxCount = 4; // nombre max de burgers par niveau
const int burgerMaxSlices = 12; // nombre max de tranches par burger


/**
 * @brief niveaux de 1 à 6
 * ' ' => on peut y aller, 'X' => impossible d'y aller (voir levels.png)
 * ce tableau contient les bords (qu'on ne voit pas a l'ecran)
 */
char blocks[levelsCount][arrayBlocksY][arrayBlocksX + 1] = { // + 1 car on utilise une chaine de caractere terminee par un "invisible" zero
	{
	"XXXXXXXXXXXXXXXXXXX",
	"X                 X",
	"X XXX X X XXX XXX X",
	"X XXX X X XXX XXX X",
	"X XXX X X XXX XXX X",
	"X     X X         X",
	"XXX X X X X X XXX X",
	"XXX X     X X XXX X",
	"XXX X XXX X X XXX X",
	"X     XXX X X     X",
	"X X X XXX X X X XXX",
	"X X X         X XXX",
	"X X X XXX XXX X XXX",
	"X X X XXX XXX     X",
	"X X X XXX XXX X X X",
	"X             X X X",
	"X XXX XXX XXX X X X",
	"X XXX XXX XXX X X X",
	"X XXX XXX XXX X X X",
	"X                 X",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	},
	{
	"XXXXXXXXXXXXXXXXXXX",
	"X                 X",
	"X X X X X X X X X X",
	"X         X X X X X",
	"X X X X X X X X X X",
	"X     X X         X",
	"X X X X X X X X X X",
	"X X X     X X     X",
	"X X X X X X X X X X",
	"X                 X",
	"XXXXXXX X X XXXXXXX",
	"XXXXXXX X     XXXXX",
	"XXXXXXX X X XXXXXXX",
	"XXXXX     X XXXXXXX",
	"XXXXXXX X X XXXXXXX",
	"XXXXXXX X     XXXXX",
	"XXXXXXX X X XXXXXXX",
	"XXXXX         XXXXX",
	"XXXXXXXXX XXXXXXXXX",
	"XXXXXXXXX XXXXXXXXX",
	"XXXXXXXXX XXXXXXXXX",
	"XXXXXXXXX XXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	},
	{
	"XXXXXXXXXXXXXXXXXXX",
	"X                 X",
	"X XXX X XXX XXXXX X",
	"X                 X",
	"X XXX XXX XXX XXX X",
	"X         XXX     X",
	"X XXXXX X XXX XXX X",
	"X                 X",
	"XXXXX X X X XXXXXXX",
	"XXXXX X X     XXXXX",
	"XXXXX X X XXX XXXXX",
	"XXXXX         XXXXX",
	"XXXXX XXXXXXX XXXXX",
	"XXXXX XXXXXXX XXXXX",
	"XXXXX XXXXXXX XXXXX",
	"X     XXXXXXX     X",
	"X X X XXXXXXX XXX X",
	"X     XXXXXXX     X",
	"X X X XXXXXXX X X X",
	"X     XXXXXXX     X",
	"XXXXX XXXXXXX XXXXX",
	"XXXXX         XXXXX",
	"XXXXX X X X X XXXXX",
	"XXXXX         XXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	},
	{
	"XXXXXXXXXXXXXXXXXXX",
	"X                 X",
	"X XXX XXX XXX XXX X",
	"X                 X",
	"XXX XXX XXX XXX XXX",
	"X                 X",
	"X XXX XXX XXX XXX X",
	"X                 X",
	"XXX XXX X X XXX XXX",
	"X                 X",
	"X XXX XXX XXX XXX X",
	"X                 X",
	"X X XXX XXX XXX X X",
	"X                 X",
	"X XXX XXX XXX XXX X",
	"X                 X",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	},
	{
	"XXXXXXXXXXXXXXXXXXX",
	"X                 X",
	"X XXXXXXXXXXXXX X X",
	"X                 X",
	"XXXXXXXXX XXX X X X",
	"X             X X X",
	"X XXXXXXXXXXXXX X X",
	"X             X X X",
	"X XXXXXXXXXXX X X X",
	"X                 X",
	"X XXX XXXXXXXXX X X",
	"X             X X X",
	"X XXXXXXXXXXX X X X",
	"X             X X X",
	"X XXXXXXXXXXX X X X",
	"X             X X X",
	"X XXXXXXXXXXXXX X X",
	"X XXXXXXXXXXX     X",
	"X XXXXXXXXXXX XXX X",
	"X XXXXXXXXXXX XXX X",
	"X XXXXXXXXXXX XXX X",
	"X XXXXXXXXXXX XXX X",
	"X XXXXXXXXXXX XXX X",
	"X XXXXXXXXXXX     X",
	"XXXXXXXXXXXXXXXXXXX",
	},
	{
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXX     XXX     X",
	"XXXXX X XXXXX XXXXX",
	"X     X X     XXXXX",
	"XXXXX X XXX XXXXXXX",
	"XXXXX     X X     X",
	"XXXXX X X X X XXXXX",
	"X     X X     XXXXX",
	"XXXXXXX X XXX XXXXX",
	"XXXXX     XXX     X",
	"XXXXX XXX XXX XXXXX",
	"X     XXX     XXXXX",
	"XXXXX XXXXX XXXXXXX",
	"XXXXX     X X     X",
	"XXXXX X X X X XXXXX",
	"X     X X     XXXXX",
	"XXXXXXX XXX X XXXXX",
	"XXXXX     X X     X",
	"XXXXX XXX X X XXXXX",
	"X     XXX     XXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXX",
	},
};

// tranches et assiettes par niveau
BurgerTechSprite slices[levelsCount][burgerMaxSlices][burgerMaxCount] = {
	{
		{NoSlice, SliceBreadTop, SliceBreadTop, SliceBreadTop},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{SliceBreadTop, NoSlice, SliceSalad, SliceSalad},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{SliceSalad, NoSlice, NoSlice, SliceSteak},
		{NoSlice, SliceSalad, SliceSteak, NoSlice},
		{NoSlice, NoSlice, NoSlice, SliceBreadBottom},
		{SliceSteak, SliceSteak, NoSlice, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{SliceBreadBottom, SliceBreadBottom, SliceBreadBottom, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{Plate, Plate, Plate, Plate},
	},
	{
		{SliceBreadTop, SliceBreadTop, SliceBreadTop, SliceBreadTop},
		{SliceSalad, SliceCheese, NoSlice, NoSlice},
		{SliceCheese, NoSlice, NoSlice, SliceSalad},
		{NoSlice, SliceSalad, NoSlice, SliceCheese},
		{SliceBreadBottom, NoSlice, NoSlice, SliceBreadBottom},
		{NoSlice, NoSlice, SliceCheese, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, NoSlice, SliceSalad, NoSlice},
		{Plate, SliceBreadBottom, SliceBreadBottom, Plate},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, Plate, Plate, NoSlice},
	},
	{
		{SliceBreadTop, SliceBreadTop, SliceBreadTop, SliceBreadTop},
		{NoSlice, SliceTomato, SliceSteak, NoSlice},
		{SliceSteak, NoSlice, NoSlice, SliceTomato},
		{SliceBreadBottom, SliceBreadBottom, SliceBreadBottom, SliceBreadBottom},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{Plate, NoSlice, NoSlice, Plate},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{SliceBreadTop, NoSlice, NoSlice, SliceBreadTop},
		{SliceTomato, Plate, Plate, SliceSteak},
		{SliceBreadBottom, NoSlice, NoSlice, SliceBreadBottom},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{Plate, NoSlice, NoSlice, Plate},
	},
	{
		{SliceBreadTop, SliceBreadTop, SliceBreadTop, SliceBreadTop},
		{SliceCheese, SliceSalad, SliceTomato, SliceCheese},
		{SliceTomato, SliceTomato, SliceSalad, SliceSalad},
		{SliceSalad, SliceSteak, SliceTomato, SliceSteak},
		{SliceSteak, SliceTomato, SliceSteak, SliceTomato},
		{SliceSalad, SliceCheese, SliceCheese, SliceSalad},
		{SliceTomato, SliceSalad, SliceSalad, SliceTomato},
		{SliceBreadBottom, SliceBreadBottom, SliceBreadBottom, SliceBreadBottom},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{Plate, Plate, Plate, Plate},
	},
	{
		{NoSlice, SliceBreadTop, SliceBreadTop, NoSlice},
		{NoSlice, SliceTomato, SliceTomato, NoSlice},
		{NoSlice, SliceSalad, SliceSteak, NoSlice},
		{NoSlice, SliceSteak, SliceSalad, NoSlice},
		{NoSlice, SliceSalad, SliceSteak, NoSlice},
		{NoSlice, SliceSteak, SliceTomato, NoSlice},
		{NoSlice, SliceTomato, SliceSalad, NoSlice},
		{NoSlice, SliceBreadBottom, SliceBreadBottom, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{NoSlice, Plate, Plate, NoSlice},
	},
	{
		{NoSlice, SliceBreadTop, NoSlice, SliceBreadTop},
		{SliceBreadTop, NoSlice, SliceBreadTop, NoSlice},
		{NoSlice, SliceCheese, NoSlice, SliceSteak},
		{SliceCheese, NoSlice, SliceCheese, NoSlice},
		{NoSlice, SliceSteak, NoSlice, SliceCheese},
		{SliceSteak, NoSlice, SliceCheese, NoSlice},
		{NoSlice, SliceCheese, NoSlice, SliceBreadBottom},
		{NoSlice, NoSlice, SliceSteak, NoSlice},
		{NoSlice, SliceBreadBottom, NoSlice, NoSlice},
		{SliceBreadBottom, NoSlice, SliceBreadBottom, NoSlice},
		{NoSlice, NoSlice, NoSlice, NoSlice},
		{Plate, Plate, Plate, Plate},
	}
};

int bonusEntryPosition[levelsCount][2] = {
	{9, 7},
	{9, 13},
	{9, 13},
	{9, 1},
	{13, 5},
	{9, 9},
};

int onePlayerEntryPosition[levelsCount][2] = {
	{9, 19},
	{9, 21},
	{9, 23},
	{9, 15},
	{13, 23},
	{9, 19},
};

int twoPlayersEntryPosition[levelsCount * 2][2] = {
	{7, 19}, {11, 19},
	{7, 17}, {11, 17},
	{7, 23}, {11, 23},
	{7, 15}, {11, 15},
	{13, 23}, {15, 23},
	{9, 19}, {9, 17},
};

// les points d'entree possible pour les ennemis pour chaque niveau
int enemyEntryPosition[][2] = {
	// niveau 0
	{1, 1},
	{17, 1},
	{1, 19},
	{17, 19},
	{-1, -1},

	// niveau 1
	{1, 1},
	{17, 1},
	{1, 9},
	{17, 9},
	{-1, -1},

	// niveau 2
	{1, 1},
	{17, 1},
	{1, 7},
	{17, 7},
	{1, 15},
	{17, 15},
	{1, 21},
	{17, 21},
	{-1, -1},

	// niveau 3
	{1, 1},
	{17, 1},
	{1, 15},
	{17, 15},
	{-1, -1},

	// niveau 4
	{1, 1},
	{17, 1},
	{1, 23},
	{17, 17},
	{-1, -1},

	// niveau 5
	{1, 3},
	{17, 1},
	{1, 15},
	{17, 13},
	{1, 7},
	{17, 17},
	{-1, -1},

	// fin
};

// le nombre d'ennemis par niveau
int eggs[levelsCount] = { 1, 1,	1,	0,	4,	2 };
int sausages[levelsCount] = { 3, 3,	2,	4,	0,	2 };
int pickles[levelsCount] = { 0,	0,	3,	2,	2,	2 };

// il y a un leger decalage du sprite du level avec la taille des blocs
sf::Vector2i offset(0, 3);

/**
 * @brief construit un nouveau niveau
 * 
 * @param game jeu
 * @param sprite sprite du niveau
 * @param number numéro du niveau
 */
Level::Level(Game* game, sf::Sprite* sprite, int number) :
	Updatable(game)
{
	_bonus = nullptr;
	_finishingPlayer = nullptr;
	_sprite = sprite;
	_number = number;
	_stackableSlices = 0;
	auto scale = game->getScale();
	_sprite->setPosition((float)(offset.x * scale.x), (float)(offset.y * scale.y));

	// on cree les position d'entree possibles pour les ennemis
	// les { -1, -1 } marquent la fin d'un niveau
	auto skipped = 0;
	for (const auto& item : enemyEntryPosition)
	{
		if (skipped == _number)
		{
			if (item[0] < 0)
				break;

			auto v = new sf::Vector2i(item[0], item[1]);
			_enemiesEntryPositions.push_back(v);
		}
		else
		{
			if (item[0] < 0)
			{
				skipped++;
			}
		}
	}
}

/**
 * @brief détruit le niveau
 * 
 */
Level::~Level()
{
	destroy(false);
	for (const auto& item : _enemiesEntryPositions)
	{
		delete item;
	}
}

/**
 * @brief on détruit les burger si gameStart est true
 * 
 * @param gameStart 
 */
void Level::destroy(bool gameStart)
{
	for (const auto& item : _enemies)
	{
		delete item;
	}
	_enemies.clear();

	if (gameStart)
	{
		for (const auto& item : _burgers)
		{
			delete item;
		}
		_burgers.clear();
	}

	for (const auto& item : _peppers)
	{
		delete item;
	}
	_peppers.clear();

	for (const auto& item : _scores)
	{
		delete item;
	}
	_scores.clear();

	if (_bonus)
	{
		delete _bonus;
		_bonus = nullptr;
	}
	_movables.clear();
}

/**
 * @brief prépare pour le jeu
 * 
 * @param gameStart 
 * @param resetSlices 
 */
void Level::prepareForPlay(bool gameStart, bool resetSlices)
{
	destroy(gameStart);

	if (!_burgers.size())
	{
		// premiere fois par niveau, on calcule les burgers avec les tranches associees
		for (auto burgerX = 0; burgerX < burgerMaxCount; burgerX++)
		{
			Burger* burger = nullptr;
			for (auto sliceY = 0; sliceY < burgerMaxSlices; sliceY++)
			{
				auto type = slices[_number][sliceY][burgerX];
				if (type == NoSlice) // pas de tranche ici
					continue;

				// haut de burger
				if (type == SliceBreadTop)
				{
					burger = new Burger((int)_burgers.size());
				}

				// nouvelle tranche
				auto slice = new Slice(getGame(), burger, burger->getSliceCount(), type, sliceY);
				burger->addSlice(slice);

				sf::Vector2i pos(2 + 4 * burgerX, 2 + 2 * sliceY);
				if (type == Plate)
				{
					// l'assiette est plus loin
					pos.y++;
					slice->setState(SliceStacked); // deja empilee
				}
				else
				{
					_stackableSlices++;
				}

				slice->setBlockPosition(pos);

				// c'est l'assiette (derniere tranche), on ajoute le burger a la liste
				if (type == Plate)
				{
					_burgers.push_back(burger);
				}
			}
		}
	}
	else if (resetSlices)
	{
		// on remet les tranches a zero
		_stackableSlices = 0;
		for (const auto& item : _burgers)
		{
			for (auto i = 0; i < item->getSliceCount(); i++)
			{
				auto slice = item->getSlice(i);
				slice->prepareForPlay();
				sf::Vector2i pos(2 + 4 * slice->getBurger()->getNumber(), 2 + 2 * slice->getVerticalPosition());
				if (slice->getSliceType() == Plate)
				{
					// l'assiette est plus loin
					pos.y++;
					slice->setState(SliceStacked); // deja empilee
				}
				else
				{
					_stackableSlices++;
				}
				slice->setBlockPosition(pos);
			}
		}
	}

	// on cree les ennemis
	auto speed = enemyBaseSpeed;
	for (auto i = 0; i < sausages[_number]; i++)
	{
		auto item = new Sausage(getGame());
		_enemies.push_back(item);
		_movables.push_back(item);

		spawnEnemy(item);
		item->setSpeed(speed);
		speed += enemySpeedIncrement;
	}

	for (auto i = 0; i < eggs[_number]; i++)
	{
		auto item = new Egg(getGame());
		_enemies.push_back(item);
		_movables.push_back(item);

		spawnEnemy(item);
		item->setSpeed(speed);
		speed += enemySpeedIncrement;
	}

	for (auto i = 0; i < pickles[_number]; i++)
	{
		auto item = new Pickle(getGame());
		_enemies.push_back(item);
		_movables.push_back(item);

		spawnEnemy(item);
		item->setSpeed(speed);
		speed += enemySpeedIncrement;
	}

	_bonusClock.restart();
	_finishingPlayer = nullptr;

	// on place les joueurs aux emplacements prevus pour le niveau
	auto count = getGame()->getPlayersCount();
	for (auto i = 0; i < count; i++)
	{
		auto player = getGame()->getPlayer(i);
		int* pos;
		if (count != 2)
		{
			pos = onePlayerEntryPosition[_number];
		}
		else
		{
			pos = twoPlayersEntryPosition[i % 2 + _number * 2];
		}

		player->setBlockPosition(sf::Vector2i(pos[0], pos[1]), false);
		player->prepareForPlay(gameStart);
		_movables.push_back(player);
	}
}

/**
 * @brief récupère le numéro du niveau
 * 
 * @return numéro du niveau
 */
int Level::getNumber() const
{
	return _number;
}

/**
 * @brief retourne le joueur qui termine
 * 
 * @return Player* 
 */
Player* Level::getFinishingPlayer() const
{
	return _finishingPlayer;
}

/**
 * @brief ajoute au niveau un objet de classe Movable passé en paramètre 
 * 
 * @param movable 
 */
void Level::addMovable(Movable* movable)
{
	_movables.push_back(movable);
}

/**
 * @brief enlève un objet de classe Movable du niveau, passé en paramètre
 * 
 * @param movable 
 */
void Level::removeMovable(Movable* movable)
{
	_movables.erase(std::remove(_movables.begin(), _movables.end(), movable), _movables.end());
}

/**
 * @brief vérifie si tout les burgers ont été complétés
 * 
 * @return true 
 * @return false 
 */
bool Level::areAllBurgersPrepared() const
{
	return _stackableSlices == 0;
}

/**
 * @brief vérifie si le niveau est terminé (si tout les burgers sont finis, ou qu'un joueur est mort)
 * 
 * @return true 
 * @return false 
 */
bool Level::isFinished() const
{
	return areAllBurgersPrepared() || getGame()->isAnyPlayerDead();
}

/**
 * @brief termine le niveau
 * 
 * @param finishingPlayer 
 * @param resetSlices 
 */
void Level::finish(Player* finishingPlayer, bool resetSlices)
{
	if (resetSlices)
	{
		_stackableSlices = 0;
	}

	if (finishingPlayer)
	{
		_finishingPlayer = finishingPlayer;
	}

	for (const auto& item : _movables)
	{
		item->finishLevel();
	}
}

/**
 * @brief enlève le score passé en paramètre
 * 
 * @param score 
 */
void Level::killScore(Score* score)
{
	_scores.erase(std::remove(_scores.begin(), _scores.end(), score), _scores.end());
	delete score;
}

/**
 * @brief enlève le poivre passé en paramètre
 * 
 * @param pepper 
 */
void Level::killPepper(Pepper* pepper)
{
	removeMovable(pepper);
	_peppers.erase(std::remove(_peppers.begin(), _peppers.end(), pepper), _peppers.end());
	delete pepper;
}

/**
 * @brief enlève l'ennemi passé en paramètre et fait apparaître un nouvel ennemi si spawNewOne est true
 * 
 * @param enemy 
 * @param spawnNewOne 
 */
void Level::killEnemy(Enemy* enemy, bool spawnNewOne)
{
	removeMovable(enemy);
	_enemies.erase(std::remove(_enemies.begin(), _enemies.end(), enemy), _enemies.end());
	delete enemy;
	if (spawnNewOne)
	{
		auto newEnemy = spawnNewEnemy();
	}
}

/**
 * @brief enlève un ennemi au hasard dans ceux présents
 * 
 */
void Level::killRandomEnemy()
{
	if (!_enemies.size())
		return;

	auto index = Game::getRandom(0, (int)_enemies.size() - 1);
	killEnemy(_enemies.at(index), false);
}

/**
 * @brief fait apparaître un nouvel ennemi
 * 
 * @return const Enemy* 
 */
const Enemy* Level::spawnNewEnemy()
{
	auto rnd = Game::getRandom(0, 2);
	Enemy* enemy;
	switch (rnd)
	{
	case 0:
		enemy = new Pickle(getGame());
		break;

	case 1:
		enemy = new Egg(getGame());
		break;

	default:
		enemy = new Sausage(getGame());
		break;
	}

	_enemies.push_back(enemy);
	_movables.push_back(enemy);

	spawnEnemy(enemy);
	enemy->setSpeed(enemyBaseSpeed);
	enemy->setState(EnemyInLimbo);
	return enemy;
}

/**
 * @brief vérifie si un ennemi se trouve sur un certain bloc passé en paramètre
 * 
 * @param blockPosition 
 * @param except 
 * @return true 
 * @return false 
 */
bool Level::isThereAnyEnemyAtBlock(const sf::Vector2i blockPosition, const Enemy* except) const
{
	auto bounds = Level::getBlockPixelBounds(blockPosition);
	for (const auto& item : _enemies)
	{
		if (item == except)
			continue;

		if (item->collides(bounds))
			return true;
	}
	return false;
}


/**
 * @brief fait apparaître un ennemi
 * on essaye de ne pas mettre 2 ennemis au même endroit
 * 
 * @param enemy 
 */
void Level::spawnEnemy(Enemy* enemy) const
{
	// on essaie de ne pas mettre 2 ennemis au meme endroit
	auto count = (int)_enemiesEntryPositions.size();
	for (auto i = 0; i < count; i++)
	{
		auto pos = *_enemiesEntryPositions.at(i);
		if (!isThereAnyEnemyAtBlock(pos, enemy))
		{
			enemy->setBlockPosition(pos, false);
			return;
		}
	}

	// pas trouve, on met au hasard
	auto index = Game::getRandom(0, count - 1);
	enemy->setBlockPosition(*_enemiesEntryPositions.at(index), false);
}

/**
 * @brief ajoute une tranche
 * si le burger est complet, on gagne des points
 * à 2 joueur, le joueur qui finit le niveau à des points en plus
 * 
 * @param slice 
 * @param player 
 */
void Level::addStackedSlice(Slice* slice, Player* player)
{
	_stackableSlices--;

	// si le burger est complet, on gagne des points
	if (slice->getBurger()->isComplete())
	{
		player->addScore(completeBurgerScore, slice->getCenterPixelPosition());
	}

	// a 2 joueur, le joueur qui finit le niveau a des points en plus
	if (!_stackableSlices)
	{
		_finishingPlayer = player;
		if (player && getGame()->getPlayersCount() > 1)
		{
			player->addScore(finishLevelScore, slice->getCenterPixelPosition());
		}
	}
}

/**
 * @brief parcours du joueur
 * on vérifie si on marche sur un bonus
 * on parcourt les burgers et les tranches pour savoir si on a marché dessus
 * 
 * @param pixelBounds 
 * @param player 
 */
void Level::playerStep(const sf::IntRect& pixelBounds, Player* player) const
{
	// est ce qu'on marche sur un bonus?
	if (_bonus && player)
	{
		if (_bonus->collides(player))
		{
			player->addScore(_bonus->getScore(), _bonus->getCenterPixelPosition());
			_bonus->setScored(); // on ne supprime pas le bonus, on le marque car on est dans l'update et on ne peut pas supprimer des movables en plein milieu
		}
	}

	// on parcourt les burgers et les tranches pour savoir si on a marche dessus
	for (const auto& item : _burgers)
	{
		for (auto i = 0; i < item->getSliceCount(); i++)
		{
			auto slice = item->getSlice(i);
			if (slice->playerStep(pixelBounds, player))
				return;
		}
	}
}

/**
 * @brief récupère la position d'un bloc
 * 
 * @param pixelPosition 
 * @return sf::Vector2i 
 */
sf::Vector2i Level::getBlockPosition(const sf::Vector2i& pixelPosition)
{
	return sf::Vector2i(getBlockXFromPixelPositionX(pixelPosition.x), getBlockYFromPixelPositionY(pixelPosition.y));
}


/**
 * @brief récupère le bloc en X
 * en X, le rapport entre les blocs et les pixels n'est pas linéaire, car les blocs sont de 16 (blocs visibles) et 8 pixels (blocs "murs" intermédiaires)
 * index pixel (208):  0..15 16..23 24..39 40..47 .... 183..191 192..207     
 * index bloc   (19):  1     2      3      4      .... 15       16     (0 et 17 sont les bords, non definis dans l'image)
 * @param x 
 * @return int 
 */
int Level::getBlockXFromPixelPositionX(int x)
{
	if (x < 0) return 0;
	return 1 + (x / levelBlocksDivisionX / 3) * 2 + ((x / levelBlocksDivisionX) % 3) / 2;
}

/**
 * @brief récupère le bloc en Y
 * 
 * @param y 
 * @return int 
 */
int Level::getBlockYFromPixelPositionY(int y)
{
	if (y < 0) return 0;
	return  1 + y / levelBlocksPixelsY;
}

/**
 * @brief récupère la position en pixel d'un bloc
 * 
 * @param blockPosition 
 * @return sf::Vector2i 
 */
sf::Vector2i Level::getBlockPixelPosition(const sf::Vector2i& blockPosition)
{
	auto x = blockPosition.x / 2 * 3 * levelBlocksDivisionX;
	if (blockPosition.x % 2 == 0)
	{
		x -= levelBlocksDivisionX;
	}

	auto y = levelBlocksPixelsY * blockPosition.y;
	return sf::Vector2i(x, y);
}

/**
 * @brief récupère les bords d'un block
 * 
 * @param blockPosition 
 * @return sf::IntRect 
 */
sf::IntRect Level::getBlockPixelBounds(const sf::Vector2i& blockPosition)
{
	int width;
	auto x = blockPosition.x / 2 * 3 * levelBlocksDivisionX;
	if (blockPosition.x % 2 == 1)
	{
		width = 2 * levelBlocksDivisionX;
	}
	else
	{
		width = levelBlocksDivisionX;
		x -= levelBlocksDivisionX;
	}

	auto y = levelBlocksPixelsY * blockPosition.y;
	return sf::IntRect(x, y, width, levelBlocksPixelsY);
}

/**
 * @brief récupère les directions disponibles (dans lequelles on peut se rendre) selon ou on se trouve
 * 
 * @param blockPosition 
 * @return std::vector<MoveDirection> 
 */
std::vector<MoveDirection> Level::getAvailableDirections(const sf::Vector2i blockPosition) const
{
	std::vector<MoveDirection> directions;

	if (blocks[_number][blockPosition.y][blockPosition.x - 1] == ' ')
	{
		directions.push_back(Left);
	}

	if (blocks[_number][blockPosition.y][blockPosition.x + 1] == ' ')
	{
		directions.push_back(Right);
	}

	if (blocks[_number][blockPosition.y - 1][blockPosition.x] == ' ')
	{
		directions.push_back(Up);
	}

	if (blocks[_number][blockPosition.y + 1][blockPosition.x] == ' ')
	{
		directions.push_back(Down);
	}
	return directions;
}

/**
 * @brief est ce que le bloc est du sol (où le joueur peut marcher)
 * 
 * un sol est un bloc vide avec: soit un bloc plein en dessous, soit un bloc vide en dessous et les 2 blocs à côté du bloc en dessous pleins (milieu d'échelle)
 * 
 * @param blockPosition 
 * @return true 
 * @return false 
 */
bool Level::isBlockGround(const sf::Vector2i& blockPosition) const
{
	if (blockPosition.x <= 0 || blockPosition.y <= 0)
		return false;

	if (blockPosition.x >= arrayBlocksX || blockPosition.y >= arrayBlocksY)
		return false;

	if (!isBlockOpen(blockPosition))
		return false;

	auto pos = blockPosition;
	pos.y++;
	if (!isBlockOpen(pos))
		return true;

	return !isBlockOpen(sf::Vector2i(pos.x - 1, pos.y)) && !isBlockOpen(sf::Vector2i(pos.x + 1, pos.y));
}

/**
 * @brief est ce que le bloc est ouvert
 * 
 * @param blockPosition 
 * @return true 
 * @return false 
 */
bool Level::isBlockOpen(const sf::Vector2i& blockPosition) const
{
	return blocks[_number][blockPosition.y][blockPosition.x] == ' ';
}

/**
 * @brief surcharge de la fonction précédente
 * la tolérance sert à pouvoir passer facilement d'une échelle au sol ou inversement
 * on teste chaque coin du rectangle moyennant la tolérance
 * 
 * @param pixelBounds 
 * @param tolerance 
 * @return true 
 * @return false 
 */
bool Level::isBlockOpen(const sf::IntRect& pixelBounds, int tolerance) const
{

	if (!isBlockOpen(Level::getBlockPosition(sf::Vector2i(pixelBounds.left + tolerance, pixelBounds.top + tolerance))))
		return false;

	if (!isBlockOpen(Level::getBlockPosition(sf::Vector2i(pixelBounds.left + pixelBounds.width - 1 - tolerance, pixelBounds.top + tolerance))))
		return false;

	if (!isBlockOpen(Level::getBlockPosition(sf::Vector2i(pixelBounds.left + tolerance, pixelBounds.top + pixelBounds.height - 1 - tolerance))))
		return false;

	if (!isBlockOpen(Level::getBlockPosition(sf::Vector2i(pixelBounds.left + pixelBounds.width - 1 - tolerance, pixelBounds.top + pixelBounds.height - 1 - tolerance))))
		return false;

	return true;
}


/**
 * @brief récupère les ennemis dans le coin
 * 
 * @param pixelBounds 
 * @return std::vector<Enemy*> 
 */
std::vector<Enemy*> Level::getEnemiesAround(const sf::IntRect pixelBounds) const
{
	std::vector<Enemy*> enemies;
	for (const auto& item : _enemies)
	{
		if (item->collides(pixelBounds))
		{
			enemies.push_back(item);
		}
	}
	return enemies;
}

/**
 * @brief est ce qu'un ennemi est empoivré
 * 
 * @param pixelBounds 
 * @return true 
 * @return false 
 */
bool Level::isPeppered(const sf::IntRect pixelBounds) const
{
	for (const auto& item : _peppers)
	{
		if (item->collides(pixelBounds))
			return true;
	}
	return false;
}

/**
 * @brief jette du poivre
 * 
 * @param pixelPosition 
 * @param direction 
 */
void Level::sprayPepper(const sf::Vector2i pixelPosition, MoveDirection direction)
{
	auto pepper = new Pepper(getGame(), direction);
	pepper->setPixelPosition(pixelPosition, false);
	_peppers.push_back(pepper);
	addMovable(pepper);
}

/**
 * @brief ajoute le score de la valeur passée en paramètre
 * 
 * @param value 
 * @param pixelPosition 
 * @param color 
 */
void Level::addScore(int value, const sf::Vector2i& pixelPosition, const sf::Color& color)
{
	auto score = new Score(getGame(), value, pixelPosition, color);
	_scores.push_back(score);
}

/**
 * @brief ajoute un bonus
 * 
 */
void Level::addBonus()
{
	if (_bonus)
		return;

	_bonus = new Bonus(getGame());
	_bonus->setBlockPosition(sf::Vector2i(bonusEntryPosition[_number][0], bonusEntryPosition[_number][1]));
	addMovable(_bonus);
	_bonusClock.restart();
}

/**
 * @brief enlève un bonus
 * 
 */
void Level::removeBonus()
{
	if (!_bonus)
		return;

	removeMovable(_bonus);
	delete _bonus;
	_bonus = nullptr;
	_bonusClock.restart();
}

/**
 * @brief traite les évènements 
 * 
 */
void Level::processEvents()
{
	if (getGame()->isPaused())
		return;

	// on verifie que les joueurs ne sont pas morts
	for (auto i = 0; i < getGame()->getPlayersCount(); i++)
	{
		auto player = getGame()->getPlayer(i);

		// on parcourt les ennemis
		for (const auto& item : _enemies)
		{
			if (item->canKill() && item->collides(player))
			{
				player->die();
			}
		}
	}

	// on bouge les ennemis
	for (const auto& item : _enemies)
	{
		item->move();
	}

	// gestion du bonus qui apparait puis disparait
	if (!_bonus)
	{
		if (_bonusClock.getElapsedTime().asSeconds() > bonusShowStart)
		{
			addBonus();
		}
	}
	else if (_bonusClock.getElapsedTime().asSeconds() > bonusShowTime)
	{
		removeBonus();
	}
}

/**
 * @brief mise à jour du niveau
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
void Level::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (getGame()->getState() != GamePlaying)
		return;

	window.draw(*_sprite, states);
	auto paused = getGame()->isPaused();

	// affichage des tranches de burger
	for (const auto& item : _burgers)
	{
		for (auto sx = 0; sx < item->getSliceCount(); sx++)
		{
			item->getSlice(sx)->update(window, deltaTime, states);
		}
	}

	// recuperation des vieux poivres et enemmis morts
	std::vector<Pepper*> finishedPeppers;
	std::vector<Enemy*> deadEnemies;
	for (auto& item : _movables)
	{
		if (item->getType() == MovableTypePepper && item->getState() == PepperFinished)
		{
			finishedPeppers.push_back(static_cast<Pepper*>(item));
			continue;
		}

		if (item->getType() == MovableTypeEnemy && item->getState() == EnemyDead)
		{
			deadEnemies.push_back(static_cast<Enemy*>(item));
			continue;
		}

		// affichage des movables
		item->update(window, deltaTime, states);
	}

	std::vector<Score*> timedOutScores;
	for (auto& item : _scores)
	{
		if (item->timedOut())
		{
			timedOutScores.push_back(item);
			continue;
		}
		item->update(window, deltaTime, states);
	}

	// suppression des vieux poivres, ennemis morts et score eteints
	for (auto& item : finishedPeppers)
	{
		killPepper(item);
	}

	for (auto& item : deadEnemies)
	{
		killEnemy(item, !isFinished());
	}

	for (auto& item : timedOutScores)
	{
		killScore(item);
	}

	// suppression des bonus
	if (_bonus && _bonus->isScored())
	{
		removeBonus();
	}

	if (paused || getGame()->getFirstPlayerWithNoLivesLeft())
	{
		auto losingPlayer = getGame()->getFirstPlayerWithNoLivesLeft();
		if (losingPlayer)
		{
			char text[32] = "GAME OVER!";
			if (getGame()->getPlayersCount() > 1)
			{
				// on imagine qu'il n'y a que 2 joueurs max
				auto number = losingPlayer->getNumber();
				if (number)
				{
					number = 1;
				}
				else
				{
					number = 2;
				}
				snprintf(text, sizeof(finishedPeppers), "PLAYER %i WINS!", number);
			}
			getGame()->drawText(window, text, sf::Color::Red, TextCenter, TextCenter);
		}
		else
		{
			getGame()->drawText(window, "PAUSED", sf::Color::Red, TextCenter, TextCenter);
		}
	}

	// debug
	//return;
	//for (auto by = 0; by < arrayBlocksY; by++)
	//{
	//	for (auto bx = 0; bx < arrayBlocksX; bx++)
	//	{
	//		char s[10];
	//		snprintf(s, sizeof(s), "%i\n%i", bx, by);

	//		sf::Color color;
	//		auto blockPos = sf::Vector2i(bx, by);
	//		if (!isBlockOpen(blockPos))
	//		{
	//			color = sf::Color::Red;
	//			color.a = 140;
	//		}
	//		else
	//		{
	//			color = sf::Color::Transparent;
	//		}

	//		getGame()->drawBlock(window, states, s, .4f, blockPos, sf::Color::Yellow, color);
	//	}
	//}
}
