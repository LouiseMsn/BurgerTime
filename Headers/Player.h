#pragma once

enum PlayerState
{
	PlayerStandingFront,
	PlayerStandingBack,

	PlayerMovingLeft,
	PlayerMovingUp,
	PlayerMovingRight,
	PlayerMovingDown,
	
	PlayerPeppering,

	PlayerDead,

	PlayerWins,
};

/**
 * @brief Joueur
 * 
 */
class Player : public Movable
{
	int _score;
	int _peppers;
	int _number;
	int _lives;
	sf::Clock _lastPepper;

public:
	Player(Game* game, int number);

	void prepareForPlay(bool gameStart);
	bool isDead() const;
	void addScore(int score, const sf::Vector2i& pixelPosition);

	int getLives() const;
	int getPeppers() const;
	int getScore() const;
	int getNumber() const;
	void sprayPepper();
	void die();
	void stand(bool force = false);

	// surchages
	bool setState(int state);
	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
	void moveFromKey(sf::Keyboard::Key key);
	bool setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks = true);
	void finishLevel();
};
