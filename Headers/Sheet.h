#pragma once

class Sheet
{
	sf::Texture _texture;
	sf::Vector2i _scale;

public:
	Sheet();

	void setScale(const sf::Vector2i& scale);
	const sf::Vector2i& getScale() const;

	bool loadFromFile(const std::string& filename, const sf::IntRect& area = sf::IntRect());
	sf::Sprite* loadSprite(const sf::IntRect& rectangle);
	sf::Sprite* loadSpriteFlipX(const sf::IntRect& rectangle);
};

