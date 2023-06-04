#pragma once

/**
 * @brief Le jeu
 * 
 */
enum textAlignment
{
	TextCenter,
	TextNear,
	TextFar,
};

enum GameSound
{
	SoundLevelStart,

	SoundSliceStepped,
	SoundSliceStacked,
	SoundSliceFalling,

	SoundPlayerDie,
	SoundPlayerWin,
	
	SoundSprayPepper,

	SoundEnemyPeppered,
	SoundEnemyFalling,
	SoundEnemyMashed,

	// permet de reperer le nombre max de sons
	SoundMax,
};

enum GameState
{
	GameSplash1,
	GameSplash2,
	GameChoice,
	GamePlaying,
	GameLevelFinished,
	GameQuitted,
};

enum GameChoices
{
	Choice1Player,
	Choice2Players,
	ChoiceQuit,

	// permet de reperer le nombre max de choix
	ChoicesMax,
};

class Updatable;
class Sheet;
class SpriteSheet;
class Level;
class Player;

class Game : public Updatable
{
	bool _keysState[sf::Keyboard::KeyCount];
	bool _soundEnabled;
	int _playerLives;
	int _playerPeppers;
	Level* _levels[levelsCount];
	sf::Vector2i _scale;
	sf::Vector2i _screen;
	sf::Font _font;
	sf::Texture _logoTexture;
	sf::Sprite _logoSprite;
	sf::Shader _splashShader;
	sf::SoundBuffer* _soundBuffers[SoundMax];
	sf::Sound* _sounds[SoundMax];
	sf::Music _musicWhilePlaying;
	sf::Music _musicWhileChoosing;
	std::vector<Player*> _players;
	SpriteSheet _spritesSheet;
	Sheet _levelsSheet;
	sf::Clock _splashClock;

	GameState _state;
	GameChoices _choices;
	sf::Clock _finishedClock;
	bool _paused;
	bool _levelFinished;
	int _hiScore;
	int _currentLevelNumber;
	int _totalLevels;

	void updatePlayerInfo(sf::RenderWindow& window, Player* player) const;
	void updatePlayerLives(sf::RenderWindow& window, Player* player) const;
	void updateTotalLevels(sf::RenderWindow& window) const;
	void goToChoice();
	Level* goToNextLevel();
	Level* goToLevel(int number);
	void prepareForPlay(bool gameStart, bool resetSlices);
	void play(int numberOfPlayers);

public:
	Game(const sf::Vector2i& screen, const sf::Vector2i& scale, int playerLives, int playerPeppers, int startLevel, bool soundEnabled);
	~Game();

	static int getRandom(int min, int max);
	static float getRandom(float min, float max);
	static int getDistance(sf::Vector2i vector);

	const sf::Font& getFont() const;
	const sf::Vector2i& getScale() const;

	const bool isSoundEnabled() const;
	void enableSound(bool enabled);
	void playSound(GameSound sound) const;

	void keyPress(const sf::Event& event);
	void keyRelease(const sf::Event& event);
	void processEvents();
	void setPlayerScore(int score);

	void stopMusic();
	bool isPaused() const;
	bool isAnyPlayerDead() const;
	Player* getFirstPlayerWithNoLivesLeft() const;
	Player* getPlayer(int index) const;
	int getPlayersCount() const;
	int getPlayerLives() const;
	int getPlayerPeppers() const;
	GameState getState() const;
	Level* getCurrentLevel() const;
	const SpriteSheet& getSpriteSheet() const;
	Level* getLevel(int number) const;

	sf::FloatRect drawText(sf::RenderWindow& window,
		sf::String text,
		sf::Color fillColor,
		textAlignment horizontalAlignment,
		textAlignment verticalAlignment,
		int horizontalMargin = 1,
		int verticalMargin = 1,
		float fontScale = 1) const;

	// debug
	void drawBlock(sf::RenderWindow& window,
		const sf::RenderStates& states,
		const char* text,
		float fontScale,
		const sf::Vector2i& blockPosition,
		const sf::Color& textColor,
		const sf::Color& backColor = sf::Color::Transparent) const;
	void update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states = sf::RenderStates::Default);
};