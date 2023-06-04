#pragma once

enum EnemyState
{
	// existe mais pas encore apparu
	EnemyInLimbo,

	EnemyMovingLeft,
	EnemyMovingUp,
	EnemyMovingRight,
	EnemyMovingDown,
	
	EnemyPeppered,

	EnemyMashedRight,
	EnemyMashedLeft,

	EnemyDead,
	EnemyLevelEnd,
};

enum EnemySubState
{
	EnemyDown1,
	EnemyDown2,

	EnemyLeft1,
	EnemyLeft2,

	// flipx
	EnemyRight1,
	EnemyRight2,

	EnemyUp1,
	EnemyUp2,

	EnemyMashedRight1,
	EnemyMashedRight2,
	EnemyMashedRight3,
	EnemyMashedRight4,

	EnemyPeppered1,
	EnemyPeppered2,

	// flipx
	// le sprite n'existe que pour la saucisse
	EnemyMashedLeft1,
	EnemyMashedLeft2,
	EnemyMashedLeft3,
	EnemyMashedLeft4,
};

/**
 * @brief Ennemi (oeuf au plat, cornichon ou saucisse)
 * 
 */
class Enemy : public Movable
{
	sf::Vector2i _targetBlockPosition;
	sf::Clock _pepperedClock;
	sf::Clock _opacityClock;
	sf::Sprite* _lastOpacitySprite;
	bool _peppered;
	float _beforeStopSpeed;
	const Slice* _fallingSlice;
	const Slice* _mashingSlice;

	MoveDirection getNearestPlayerDirection() const;
	void setPeppered();
	void setUnPeppered();
	void die();

public:
	Enemy(Game* game);
	~Enemy();

	virtual sf::Sprite* getSubStateSprite(EnemySubState subState) const = 0;

	bool canKill() const;
	bool isPeppered() const;
	bool fallWithSlice(const Slice* slice);
	bool mashBySlice(const Slice* slice);

	// surchages
	void setBlockPosition(const sf::Vector2i& block, bool checkBlocks);
	bool setMovingDirection(MoveDirection direction);
	bool setState(int state);
	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
	void move();
	void finishLevel();
	void drawSprite(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states);
};

