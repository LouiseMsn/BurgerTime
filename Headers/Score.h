#pragma once

/**
 * @brief Score
 * 
 */
class Score : public Updatable
{
	int _value;
	sf::Text _text;
	sf::Vector2f _pixelPosition;
	sf::Clock _timeout;
	sf::Color _color;
	int _xIncrement;
	int _yIncrement;

	void updateText(int speed);

public:
	Score(Game* game, int value, const sf::Vector2i& pixelPosition, const sf::Color& color);

	bool timedOut();

	// surchages
	void updateWhenMoved(int speed);
	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
};

