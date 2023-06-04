#pragma once


enum PepperState
{
	PepperState1,
	PepperState2,
	PepperState3,
	PepperState4,

	PepperFinished,
};

/**
 * @brief Poivre pour attaquer les ennemis
 * 
 */
class Pepper : public Movable
{
	sf::Clock _clock;

	void move();

public:
	Pepper(Game* game, MoveDirection direction);

	// surchages
	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
	bool setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks = true);
};

