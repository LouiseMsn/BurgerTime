#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Updatable.h"
#include "Headers/Level.h"
#include "Headers/Game.h"

/**
 * @brief constuit un nouvel objet updatable
 * 
 * @param game 
 */
Updatable::Updatable(Game* game) :
	_speed(0),
	_speedAccumulator(0),
	_game(game)
{
}

/**
 * @brief récupère le jeu
 * 
 * @return Game* 
 */
Game* Updatable::getGame() const
{
	return _game;
}

/**
 * @brief récupère la vitesse de l'updatable
 * 
 * @return float 
 */
float Updatable::getSpeed() const
{
	return _speed;
}

/**
 * @brief setter de la vitesse de l'updatable
 * 
 * @param speed 
 */
void Updatable::setSpeed(float speed)
{
	_speed = speed;
}

/**
 * @brief mise à jour lorsque l'updatable est bougé (abstrait)
 * 
 * @param speed 
 */
void Updatable::updateWhenMoved(int speed)
{
	// rien a faire de base
}

/**
 * @brief mise à jour de l'updatable
 * On utilise le temps afin que la vitesse du jeu ne dépende pas de la vitesse de la machine
 * les évènements arrivent en microsecondes (ex: appui sur une flèche directionnelle). On les accumules avec multiplication entière
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
void Updatable::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (getGame()->isPaused())
		return;

	// on utilise le temps pour que la vitesse du jeu ne depense pas de la vitesse de la machine
	auto speed = getSpeed() * deltaTime.asMicroseconds();
	if (!speed)
		return;

	// on n'utilise que des entiers pour les pixels, etc. mais les evenements arrivent en micro secondes
	// on les accumule donc (avec multiplication entiere)
	const int integerMul = 10000; // plus c'est grand, moins on a d'erreur d'arrondis
	_speedAccumulator += (int)(speed * integerMul);
	if (_speedAccumulator > integerMul)
	{
		_speedAccumulator /= integerMul;
		updateWhenMoved(_speedAccumulator);
		_speedAccumulator = 0;
	}
}
