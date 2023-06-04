#pragma once

#define SPRITE(x) (getGame()->getSpriteSheet().getSprite(x))

enum MovableType
{
	MovableTypeUnspecified,
	MovableTypePlayer,
	MovableTypeEnemy,
	MovableTypePepper,
};

class Game;
enum MoveDirection : int;

/**
 * @brief Objet qui bouge sur l'écran
 * 
 */
class Movable : public Updatable
{
	int _state;
	int _subState;
	int _subStateChangeDelay; // milliseconds
	sf::Sprite* _sprite;
	sf::Clock _subStateClock;
	MoveDirection _movingDirection;
	MoveDirection _lastMovingDirection;
	MovableType _type;

protected:
	sf::Vector2i _pixelPosition;
	sf::Vector2i _size;
	virtual void drawSprite(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states);

public:
	Movable(Game* game);

	static MoveDirection getOppositeDirection(MoveDirection direction);
	static bool areOppositeDirection(MoveDirection dir1, MoveDirection dir2);
	static bool removeDirection(std::vector<MoveDirection>& directions, MoveDirection direction);

	void restartSubStateClock();
	int getSubStateElapsedMilliseconds() const;
	int getSubStateChangeDelay() const;
	void setSubStateChangeDelay(int subStateChangeDelay);
	int getSubState() const;
	virtual bool setSubState(int subState);

	MoveDirection getLastMovingDirection() const;
	bool setLastMovingDirection(MoveDirection direction);
	MoveDirection getMovingDirection() const;
	virtual bool setMovingDirection(MoveDirection direction);
	virtual std::vector<MoveDirection> getAvailableDirections() const;

	void setType(MovableType type);
	MovableType getType() const;
	int getState() const;
	virtual bool setState(int state);

	sf::Sprite* getSprite() const;
	void setSprite(sf::Sprite* sprite);
	void setSize(const sf::Vector2i& size);

	virtual bool collides(const sf::IntRect pixelBounds, int tolerance = 4) const;
	virtual bool collides(const Movable* movable, int tolerance = 4) const;
	bool isAtBlock(const sf::Vector2i& blockPosition) const;
	sf::Vector2i getBlockPosition() const;
	virtual void setBlockPosition(const sf::Vector2i& block, bool checkBlocks = true);

	sf::IntRect getPixelBounds() const;
	sf::Vector2i getPixelPosition() const;
	sf::Vector2i getCenterPixelPosition() const;
	virtual bool setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks = true);

	virtual void updateWhenMoved(int speed);
	virtual void moveFromKey(sf::Keyboard::Key key);
	virtual void finishLevel();
	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
};

