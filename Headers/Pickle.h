#pragma once

/**
 * @brief Cornichon : ennemi
 * 
 */
class Pickle: public Enemy
{
public:
	Pickle(Game* game);

	// surchages
	sf::Sprite* getSubStateSprite(EnemySubState subState) const;
};

