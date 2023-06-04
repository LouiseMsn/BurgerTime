#include <SFML/Graphics.hpp>
#include "Headers/Sheet.h"

/**
 * @brief Construit un nouveau Sheet
 * 
 */
Sheet::Sheet()
	:_scale(1, 1)
{
}

/**
 * @brief setter de l'échelle
 * 
 * @param scale 
 */
void Sheet::setScale(const sf::Vector2i& scale)
{
	_scale = scale;
}

/**
 * @brief récupère l'échelle
 * 
 * @return const sf::Vector2i& 
 */
const sf::Vector2i& Sheet::getScale() const
{
	return _scale;
}

/**
 * @brief charge le sheet à partir d'un fichier spécifié en paramètre 
 * 
 * @param filename 
 * @param area 
 * @return true 
 * @return false 
 */
bool Sheet::loadFromFile(const std::string& filename, const sf::IntRect& area)
{
	_texture.loadFromFile(filename, area);
	return true;
}

/**
 * @brief charge le sprite
 * 
 * @param rectangle 
 * @return sf::Sprite* 
 */
sf::Sprite* Sheet::loadSprite(const sf::IntRect& rectangle)
{
	auto sprite = new sf::Sprite(_texture, rectangle);
	sprite->setScale((float)_scale.x, (float)_scale.y);
	return sprite;
}

/**
 * @brief charge et retourne le sprite
 * 
 * @param rectangle 
 * @return sf::Sprite* 
 */
sf::Sprite* Sheet::loadSpriteFlipX(const sf::IntRect& rectangle)
{
	auto sprite = loadSprite(rectangle);
	sprite->setOrigin({ sprite->getLocalBounds().width, 0 });
	sprite->scale({ -1, 1 });
	return sprite;
}
