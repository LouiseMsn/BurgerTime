#pragma once

/**
 * @brief Saucisse : ennmemi
 * 
 */
class Sausage : public Enemy
{
public:
	Sausage(Game* game);

	// surchages
	void dieMashed();
	sf::Sprite* getSubStateSprite(EnemySubState subState) const;
};
