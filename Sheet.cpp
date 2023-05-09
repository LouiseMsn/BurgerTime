#include <SFML/Graphics.hpp>
#include "Headers/Sheet.h"

Sheet::Sheet()
	:_scale(1, 1)
{
}

void Sheet::setScale(const sf::Vector2i& scale)
{
	_scale = scale;
}

const sf::Vector2i& Sheet::getScale() const
{
	return _scale;
}

bool Sheet::loadFromFile(const std::string& filename, const sf::IntRect& area)
{
	_texture.loadFromFile(filename, area);
	return true;
}

sf::Sprite* Sheet::loadSprite(const sf::IntRect& rectangle)
{
	auto sprite = new sf::Sprite(_texture, rectangle);
	sprite->setScale((float)_scale.x, (float)_scale.y);
	return sprite;
}

sf::Sprite* Sheet::loadSpriteFlipX(const sf::IntRect& rectangle)
{
	auto sprite = loadSprite(rectangle);
	sprite->setOrigin({ sprite->getLocalBounds().width, 0 });
	sprite->scale({ -1, 1 });
	return sprite;
}
