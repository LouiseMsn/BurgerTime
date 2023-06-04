#pragma once

/**
 * @brief Niveau du jeu
 * 
 */
enum MoveDirection : int
{
	None,
	Left,
	Right,
	Up,
	Down
};

class Burger;
class Game;
class Slice;
class Movable;
class Enemy;
class Player;
class Bonus;
class Pepper;
class Score;
class Updatable;

class Level : public Updatable
{
public:

private:
	std::vector<sf::Vector2i*> _enemiesEntryPositions;
	sf::Sprite* _sprite;
	int _number;

	int _stackableSlices;
	Player* _finishingPlayer;
	sf::Clock _bonusClock;
	std::vector<Enemy*> _enemies;
	std::vector<Burger*> _burgers;
	std::vector<Pepper*> _peppers;
	std::vector<Score*> _scores;
	std::vector<Movable*> _movables;
	Bonus* _bonus;

	void addBonus();
	void removeBonus();
	void destroy(bool gameStart);
	bool isThereAnyEnemyAtBlock(const sf::Vector2i blockPosition, const Enemy* except) const;
	void spawnEnemy(Enemy* enemy) const;
	void killEnemy(Enemy* enemy, bool spawnNewOne);
	void killPepper(Pepper* pepper);
	void killScore(Score* score);
	void addMovable(Movable*);
	void removeMovable(Movable*);

public:
	Level(Game* game, sf::Sprite* sprite, int number);
	~Level();

	static int getBlockXFromPixelPositionX(int x);
	static int getBlockYFromPixelPositionY(int y);
	static sf::Vector2i getBlockPosition(const sf::Vector2i& pixelPosition);
	static sf::Vector2i getBlockPixelPosition(const sf::Vector2i& blockPosition);
	static sf::IntRect getBlockPixelBounds(const sf::Vector2i& blockPosition);

	int getNumber() const;
	bool isFinished() const;

	void processEvents();
	void prepareForPlay(bool gameStart, bool resetSlices);
	bool areAllBurgersPrepared() const;
	void finish(Player* finishingPlayer, bool resetSlices);
	void addStackedSlice(Slice* slice, Player* steppingPlayer);
	void addScore(int value, const sf::Vector2i& pixelPosition, const sf::Color& color);

	const Enemy* spawnNewEnemy();
	// debug
	void killRandomEnemy();

	std::vector<Enemy*> getEnemiesAround(const sf::IntRect pixelBounds) const;
	bool isPeppered(const sf::IntRect pixelBounds) const;
	void sprayPepper(const sf::Vector2i pixelPosition, MoveDirection direction);
	Player* getFinishingPlayer() const;
	void playerStep(const sf::IntRect& pixelPosition, Player* player) const;

	std::vector<MoveDirection> getAvailableDirections(const sf::Vector2i blockPosition) const;
	bool isBlockOpen(const sf::Vector2i& blockPosition) const;
	bool isBlockOpen(const sf::IntRect& pixelBounds, int tolerance) const;
	bool isBlockGround(const sf::Vector2i& blockPosition) const;

	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
};

