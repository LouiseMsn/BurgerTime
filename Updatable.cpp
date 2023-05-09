#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Updatable.h"
#include "Headers/Level.h"
#include "Headers/Game.h"

Updatable::Updatable(Game* game) :
	_speed(0),
	_speedAccumulator(0),
	_game(game)
{
}

Game* Updatable::getGame() const
{
	return _game;
}

float Updatable::getSpeed() const
{
	return _speed;
}

void Updatable::setSpeed(float speed)
{
	_speed = speed;
}

void Updatable::updateWhenMoved(int speed)
{
	// rien a faire de base
}

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
