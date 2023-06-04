#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Sheet.h"
#include "Headers/Constants.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Score.h"

/**
 * @brief construit un nouveau score
 * 
 * @param game 
 * @param value 
 * @param pixelPosition 
 * @param color 
 */
Score::Score(Game* game, int value, const sf::Vector2i& pixelPosition, const sf::Color& color) :
	Updatable(game)
{
	_pixelPosition = sf::Vector2f((float)pixelPosition.x, (float)pixelPosition.y);
	_value = value;
	_color = color;
	_xIncrement = Game::getRandom(-1, 1);
	_yIncrement = Game::getRandom(-1, 1);
	setSpeed(scoreSpeed);

	auto scale = game->getScale();
	_text = sf::Text(std::to_string(value), game->getFont(), (int)(fontSize * scale.y * 0.6f));
	_text.setFillColor(color);
	updateText(0);
}

/**
 * @brief mise à jour du score
 * 
 * @param speed 
 */
void Score::updateText(int speed)
{
	auto scale = getGame()->getScale();
	_pixelPosition.x += _xIncrement * speed / 2.0f;
	_pixelPosition.y += _yIncrement * speed / 2.0f;

	auto x = _pixelPosition.x * scale.x;
	auto y = _pixelPosition.y * scale.y;
	_text.setPosition(x, y);

	auto ms = _timeout.getElapsedTime().asMilliseconds();
	auto color = _text.getFillColor();
	color.a = (256 - (256 * ms)) / (scoreShowTime * 1000);
	_text.setFillColor(color);
}

/**
 * @brief true si le temps d'affichage du score est écoulé
 * 
 * @return true 
 * @return false 
 */
bool Score::timedOut()
{
	return _timeout.getElapsedTime().asSeconds() >= scoreShowTime;
}

/**
 * @brief met à jour lorsqu'il bouge
 * 
 * @param speed 
 */
void Score::updateWhenMoved(int speed)
{
	if (timedOut())
		return;

	updateText(speed);
}

/**
 * @brief mise à jour du score
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
void Score::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	Updatable::update(window, deltaTime, states);
	window.draw(_text, states);
}
