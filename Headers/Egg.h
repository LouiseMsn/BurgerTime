#pragma once

class Egg : public Enemy
{
public:
	Egg(Game* game);

	// surchages
	sf::Sprite* getSubStateSprite(EnemySubState subState) const;
};

