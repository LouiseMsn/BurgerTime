#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Pepper.h"
#include "Headers/Player.h"

#define SET_SPRITE_PLAYER(x) if (_number % 2) setSprite(SPRITE(Player2##x)); else setSprite(SPRITE(Player1##x));
#define SET_SUBSTATE(x) (_number % 2) ? setSubState(Player2##x) : setSubState(Player1##x)

Player::Player(Game* game, int number) :
	Movable(game)
{
	setType(MovableTypePlayer);
	_number = number;
	_score = 0;
	setSize(sf::Vector2i(spriteSizeX, spriteSizeX));
	prepareForPlay(true);
}

void Player::prepareForPlay(bool gameStart)
{
	stand(true);
	setSpeed(playerSpeed);
	setLastMovingDirection(Down); // pour que le poivre parte vers le bas par defaut
	SET_SPRITE_PLAYER(FrontWalk2);
	_lastPepper.restart();

	if (gameStart)
	{
		_lives = getGame()->getPlayerLives();
		_peppers = getGame()->getPlayerPeppers();
		_score = 0;
	}
}

void Player::addScore(int score, const sf::Vector2i& pixelPosition)
{
	auto before = _score / liveBonusScore;
	_score += score;
	if (_score / liveBonusScore > before)
	{
		_lives++;
	}

	sf::Color color;
	if (!_number)
	{
		color = sf::Color::White;
	}
	else
	{
		color = sf::Color(0, 191, 255); // deepskyblue
	}

	auto game = getGame();
	game->getCurrentLevel()->addScore(score, pixelPosition, color);
}

bool Player::isDead() const
{
	return getState() == PlayerDead;
}

int Player::getLives() const
{
	return _lives;
}

int Player::getScore() const
{
	return _score;
}

int Player::getPeppers() const
{
	return _peppers;
}

int Player::getNumber() const
{
	return _number;
}

void Player::stand(bool force)
{
	if (force)
	{
		setState(PlayerStandingFront);
	}
	else
	{
		auto state = getState();
		switch (getState())
		{
		case PlayerWins:
		case PlayerDead:
			return;

		case PlayerMovingUp:
			setState(PlayerStandingBack);
			break;

		default:
			if (state != PlayerStandingBack)
			{
				setState(PlayerStandingFront);
			}
			break;
		}
	}
	setMovingDirection(None);
}

void Player::finishLevel()
{
	setSpeed(0);
	setMovingDirection(None);

	auto game = getGame();
	auto player = game->getCurrentLevel()->getFinishingPlayer();
	if (player == this)
	{
		setState(PlayerWins);
		game->stopMusic();
		game->playSound(SoundPlayerWin);
	}
}

void Player::die()
{
	if (isDead())
		return;

	_lives--;
	setState(PlayerDead);
	setMovingDirection(None);
	getGame()->playSound(SoundPlayerDie);
}

void Player::sprayPepper()
{
	if (!_peppers || _lastPepper.getElapsedTime().asMilliseconds() < pepperSprayWait)
		return;

	auto pos = getPixelPosition();
	auto dir = getMovingDirection();
	if (dir == None)
	{
		dir = getLastMovingDirection();
	}

	auto game = getGame();
	game->getCurrentLevel()->sprayPepper(pos, dir);
	_peppers--;
	_lastPepper.restart();
	game->playSound(SoundSprayPepper);
}

bool Player::setPixelPosition(const sf::Vector2i& pixelPosition, bool checkBlocks)
{
	if (!Movable::setPixelPosition(pixelPosition, checkBlocks))
		return false;

	// on previent le niveau qu'on marche ici
	auto level = getGame()->getCurrentLevel();
	if (!level)
		return false;

	auto bounds = getPixelBounds();

	// les pieds du player sont a 4 pixel du bord du sprite
	bounds.left += playerFootX;
	bounds.width -= playerFootX * 2;

	// on ne veut pas detecter les tranches du niveau superieur
	bounds.top += playerBelowSliceY;
	bounds.height -= -playerBelowSliceY;
	level->playerStep(bounds, this);
	return true;
}

bool Player::setState(int state)
{
	if (!Movable::setState(state))
		return false;

	// initalisation du substate pour chaque state
	switch (getState())
	{
	case PlayerMovingRight:
		return SET_SUBSTATE(RightWalk1);

	case PlayerMovingLeft:
		return SET_SUBSTATE(LeftWalk1);

	case PlayerMovingUp:
		return SET_SUBSTATE(BackWalk1);

	case PlayerMovingDown:
		return SET_SUBSTATE(FrontWalk1);

	case PlayerWins:
		return SET_SUBSTATE(Death1); // oui le sprite est le meme mort ou en pleine forme

	case PlayerDead:
		return SET_SUBSTATE(Death1);
	}
	return true;
}

void Player::moveFromKey(sf::Keyboard::Key key)
{
	auto state = getState();
	if (state == PlayerWins || state == PlayerDead)
		return;

	switch (key)
	{
	case sf::Keyboard::Right:
		setState(PlayerMovingRight);
		break;

	case sf::Keyboard::Left:
		setState(PlayerMovingLeft);
		break;

	case sf::Keyboard::Up:
		setState(PlayerMovingUp);
		break;

	case sf::Keyboard::Down:
		setState(PlayerMovingDown);
		break;
	}
	Movable::moveFromKey(key);
}

void Player::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	if (getGame()->isPaused())
	{
		Movable::update(window, deltaTime, states);
		return;
	}

	auto elapsed = getSubStateElapsedMilliseconds();
	auto delay = getSubStateChangeDelay();
	auto state = getState();

	if (elapsed >= delay)
	{
		switch (state)
		{
		case PlayerStandingFront:
			SET_SPRITE_PLAYER(FrontWalk2);
			break;

		case PlayerStandingBack:
			SET_SPRITE_PLAYER(BackWalk2);
			break;

		case PlayerDead:
			switch (getSubState())
			{
			case Player1Death1:
				setSprite(SPRITE(Player1Death1));
				setSubState(Player1Death2);
				break;

			case Player1Death2:
				setSprite(SPRITE(Player1Death2));
				setSubState(Player1Death3);
				break;

			case Player1Death3:
				setSprite(SPRITE(Player1Death3));
				setSubState(Player1Death4);
				break;

			case Player1Death4:
				setSprite(SPRITE(Player1Death4));
				setSubState(Player1Death5);
				break;

			case Player1Death5:
				setSprite(SPRITE(Player1Death5));
				setSubState(Player1Death6);
				break;

			case Player1Death6:
				setSprite(SPRITE(Player1Death6));
				setSubState(Player1Death5); // on boucle sur 5
				break;

			case Player2Death1:
				setSprite(SPRITE(Player2Death1));
				setSubState(Player2Death2);
				break;

			case Player2Death2:
				setSprite(SPRITE(Player2Death2));
				setSubState(Player2Death3);
				break;

			case Player2Death3:
				setSprite(SPRITE(Player2Death3));
				setSubState(Player2Death4);
				break;

			case Player2Death4:
				setSprite(SPRITE(Player2Death4));
				setSubState(Player2Death5);
				break;

			case Player2Death5:
				setSprite(SPRITE(Player2Death5));
				setSubState(Player2Death6);
				break;

			case Player2Death6:
				setSprite(SPRITE(Player2Death6));
				setSubState(Player2Death5); // on boucle sur 5
				break;
			}
			break;

		case PlayerWins:
			switch (getSubState())
			{
			case Player1Death1:
				setSprite(SPRITE(Player1Death1));
				setSubState(Player1FrontWalk2);
				break;

			case Player1FrontWalk2:
				setSprite(SPRITE(Player1FrontWalk2));
				setSubState(Player1Death1);
				break;

			case Player2Death1:
				setSprite(SPRITE(Player2Death1));
				setSubState(Player2FrontWalk2);
				break;

			case Player2FrontWalk2:
				setSprite(SPRITE(Player2FrontWalk2));
				setSubState(Player2Death1);
				break;
			}
			break;

		case PlayerMovingRight:
			switch (getSubState())
			{
			case Player1RightWalk1:
				setSprite(SPRITE(Player1RightWalk1));
				setSubState(Player1RightWalk2);
				break;

			case Player1RightWalk2:
				setSprite(SPRITE(Player1RightWalk2));
				setSubState(Player1RightWalk3);
				break;

			case Player1RightWalk3:
				setSprite(SPRITE(Player1RightWalk3));
				setSubState(Player1RightWalk1);
				break;

			case Player2RightWalk1:
				setSprite(SPRITE(Player2RightWalk1));
				setSubState(Player2RightWalk2);
				break;

			case Player2RightWalk2:
				setSprite(SPRITE(Player2RightWalk2));
				setSubState(Player2RightWalk3);
				break;

			case Player2RightWalk3:
				setSprite(SPRITE(Player2RightWalk3));
				setSubState(Player2RightWalk1);
				break;
			}
			break;

		case PlayerMovingLeft:
			switch (getSubState())
			{
			case Player1LeftWalk1:
				setSprite(SPRITE(Player1LeftWalk1));
				setSubState(Player1LeftWalk2);
				break;

			case Player1LeftWalk2:
				setSprite(SPRITE(Player1LeftWalk2));
				setSubState(Player1LeftWalk3);
				break;

			case Player1LeftWalk3:
				setSprite(SPRITE(Player1LeftWalk3));
				setSubState(Player1LeftWalk1);
				break;

			case Player2LeftWalk1:
				setSprite(SPRITE(Player2LeftWalk1));
				setSubState(Player2LeftWalk2);
				break;

			case Player2LeftWalk2:
				setSprite(SPRITE(Player2LeftWalk2));
				setSubState(Player2LeftWalk3);
				break;

			case Player2LeftWalk3:
				setSprite(SPRITE(Player2LeftWalk3));
				setSubState(Player2LeftWalk1);
				break;
			}
			break;

		case PlayerMovingUp:
			switch (getSubState())
			{
			case Player1BackWalk1:
				setSprite(SPRITE(Player1BackWalk1));
				setSubState(Player1BackWalk2);
				break;

			case Player1BackWalk2:
				setSprite(SPRITE(Player1BackWalk2));
				setSubState(Player1BackWalk3);
				break;

			case Player1BackWalk3:
				setSprite(SPRITE(Player1BackWalk3));
				setSubState(Player1BackWalk1);
				break;

			case Player2BackWalk1:
				setSprite(SPRITE(Player2BackWalk1));
				setSubState(Player2BackWalk2);
				break;

			case Player2BackWalk2:
				setSprite(SPRITE(Player2BackWalk2));
				setSubState(Player2BackWalk3);
				break;

			case Player2BackWalk3:
				setSprite(SPRITE(Player2BackWalk3));
				setSubState(Player2BackWalk1);
				break;
			}
			break;

		case PlayerMovingDown:
			switch (getSubState())
			{
			case Player1FrontWalk1:
				setSprite(SPRITE(Player1FrontWalk1));
				setSubState(Player1FrontWalk2);
				break;

			case Player1FrontWalk2:
				setSprite(SPRITE(Player1FrontWalk2));
				setSubState(Player1FrontWalk3);
				break;

			case Player1FrontWalk3:
				setSprite(SPRITE(Player1FrontWalk3));
				setSubState(Player1FrontWalk1);
				break;

			case Player2FrontWalk1:
				setSprite(SPRITE(Player2FrontWalk1));
				setSubState(Player2FrontWalk2);
				break;

			case Player2FrontWalk2:
				setSprite(SPRITE(Player2FrontWalk2));
				setSubState(Player2FrontWalk3);
				break;

			case Player2FrontWalk3:
				setSprite(SPRITE(Player2FrontWalk3));
				setSubState(Player2FrontWalk1);
				break;
			}
			break;
		}
	}

	Movable::update(window, deltaTime, states);

	switch (state)
	{
	case PlayerWins:
		setSubStateChangeDelay(200);
		break;

	case PlayerDead:
		setSubStateChangeDelay(300);
		break;

	default:
		setSubStateChangeDelay(100);
		break;
	}
}
