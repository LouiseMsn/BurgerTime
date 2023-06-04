#pragma once

/**
 * @brief Entités pouvant être mise à jour dans le jeu
 * 
 */
class Game;

class Updatable
{
	Game* _game;
	float _speed;
	int _speedAccumulator;

public:
	Updatable(Game* game);

	Game* getGame() const;
	float getSpeed() const;
	void setSpeed(float speed);

	virtual void updateWhenMoved(int speed);
	virtual void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
};
