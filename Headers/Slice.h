#pragma once

enum SliceState
{
	SliceImmobile,
	SliceFalling,
	SliceBouncingUp,
	SliceBouncingDown,
	SliceStacked,
};


class Burger;
class Player;

/**
 * @brief Tranche de hamburger
 * 
 */
class Slice : public Movable
{
	const static int _sliceWidth = 32;
	const static int _sliceHeight = 7;
	const static int _chunksCount = 4;

	const Burger* _burger;
	BurgerTechSprite _type;
	Player* _steppingPlayer;
	int _number;
	int _verticalPosition;
	int _fallingWeight;
	int _bouncePosition;
	int _stepped[_chunksCount];
	sf::Vector2i _fallingBlockPosition;
	sf::Sprite* _chunks[_chunksCount];

	void stepOnIfSameLevel(int chunk, int level, Player* player);
	void setBouncingShape(int value);

public:
	Slice(Game* game, const Burger* burger, int number, BurgerTechSprite type, int verticalPosition);
	~Slice();

	const Burger* getBurger() const;
	int getNumber() const;
	BurgerTechSprite getSliceType() const;
	int getVerticalPosition() const;
	bool playerStep(const sf::IntRect& pixelBounds, Player* player);
	void prepareForPlay();

	// surchages
	bool setState(int state);
	bool setPixelPosition(const sf::Vector2i& position, bool checkBlocks = true);
	void updateWhenMoved(int speed);
	void drawSprite(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states);
};

