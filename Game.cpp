#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Headers/Constants.h"
#include "Headers/Updatable.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Movable.h"
#include "Headers/Player.h"
#include "Headers/Game.h"

#define LOAD_SOUND(x) {auto buffer = new sf::SoundBuffer(); buffer->loadFromFile("Resources/" #x ".ogg");_soundBuffers[GameSound::x] = buffer; auto sound = new sf::Sound(); sound->setBuffer(*buffer); _sounds[GameSound::x] = sound;}

/**
 * @brief construit le jeu
 * 
 * @param screen écran
 * @param scale échelle
 * @param playerLives Nombres de vies des joueurs
 * @param playerPeppers Nombre de poivres
 * @param startLevel Niveau initial
 * @param soundEnabled Activation du son
 */
Game::Game(const sf::Vector2i& screen, const sf::Vector2i& scale, int playerLives, int playerPeppers, int startLevel, bool soundEnabled) :
	Updatable(this)
{
	_hiScore = baseHiScore; // a lire d'un fichier ou reseau
	_screen = screen;
	_scale = scale;
	_state = GameSplash1;
	_choices = Choice1Player;
	_currentLevelNumber = startLevel % levelsCount;
	_totalLevels = 1;
	_playerLives = playerLives;
	_playerPeppers = playerPeppers;
	_paused = false;
	_levelFinished = false;
	_soundEnabled = soundEnabled;

	// le logo
	if (!_logoTexture.loadFromFile("Resources/logo.png"))
	{
		// erreur de chargement
		_state = GameQuitted;
	}
	_logoSprite.setTexture(_logoTexture);

	if (sf::Shader::isAvailable())
	{
		if (!_splashShader.loadFromFile("Resources/wave.vert", sf::Shader::Vertex))
		{
			// erreur de chargement
			_state = GameQuitted;
		}
	}

	// les sprites
	if (!_spritesSheet.loadBurgerSprites(scale))
	{
		// erreur de chargement
		_state = GameQuitted;
	}

	// les niveaux
	if (!_levelsSheet.loadFromFile("Resources/levels.png"))
	{
		// erreur de chargement
		_state = GameQuitted;
	}
	_levelsSheet.setScale(scale);

	// la police de caracteres
	if (!_font.loadFromFile("Resources/Font.ttf"))
	{
		// erreur de chargement
		_state = GameQuitted;
	}

	// initialisations
	for (auto i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		_keysState[i] = false;
	}

	// ajout des niveaux
	for (auto number = 0; number < levelsCount; number++)
	{
		// calcul du rectangle du niveau (modulo 3)
		auto x = levelImageWidth * (number % 3) + levelImagePadding * ((number % 3) + 1) - levelImagePadding;

		// * 2 car on n'utilise pas les niveau avec les slices dessinees (on les dessine nous meme)
		auto y = levelImageHeight * (number / 3) * 2 + levelImagePadding * ((number / 3) * 2 + 1) - levelImagePadding;
		auto rect = sf::IntRect(x, y, levelImageWidth, levelImageHeight);

		auto sprite = _levelsSheet.loadSprite(rect);
		auto level = new Level(this, sprite, number);
		_levels[number] = level;
	}

	_musicWhilePlaying.openFromFile("Resources/MusicWhilePlaying.ogg");
	_musicWhilePlaying.setLoop(true);

	_musicWhileChoosing.openFromFile("Resources/MusicWhileChoosing.ogg");
	_musicWhileChoosing.setLoop(true);

	LOAD_SOUND(SoundLevelStart);

	LOAD_SOUND(SoundSliceStepped);
	LOAD_SOUND(SoundSliceStacked);
	LOAD_SOUND(SoundSliceFalling);

	LOAD_SOUND(SoundPlayerDie);
	LOAD_SOUND(SoundPlayerWin);

	LOAD_SOUND(SoundSprayPepper);

	LOAD_SOUND(SoundEnemyPeppered);
	LOAD_SOUND(SoundEnemyMashed);
	LOAD_SOUND(SoundEnemyFalling);
}

/**
 * @brief Détruit le jeu
 * 
 */
Game::~Game()
{
	for (const auto& item : _levels)
	{
		delete item;
	}

	for (const auto& item : _sounds)
	{
		delete item;
	}

	for (const auto& item : _soundBuffers)
	{
		delete item;
	}

	for (const auto& item : _players)
	{
		delete item;
	}
}

/**
 * @brief Prépare pour jouer
 * 
 * @param gameStart vraie ou faux
 * @param resetSlices Reinitialisation des tranches
 */
void Game::prepareForPlay(bool gameStart, bool resetSlices)
{
	_state = GamePlaying;
	_paused = false;
	_levelFinished = false;
	_musicWhileChoosing.stop();

	if (_soundEnabled)
	{
		_musicWhilePlaying.play();
	}

	getCurrentLevel()->prepareForPlay(gameStart, resetSlices);
}

/**
 * @brief on redémarre un jeu et on ajoute des joueurs si nécessaire
 * 
 * @param numberOfPlayers nombre de joueurs
 */
void Game::play(int numberOfPlayers)
{
	// on redemarre un jeu
	// ajout des joueurs si necessaire
	auto count = getPlayersCount();
	auto playersToAdd = numberOfPlayers - count;
	if (playersToAdd > 0)
	{
		for (auto i = 0; i < playersToAdd; i++)
		{
			auto player = new Player(this, count + i);
			_players.push_back(player);
		}
	}

	prepareForPlay(true, true);
}

/**
 * @brief récupère le nombre de vie des joueurs
 * 
 * @return int nombre de vies
 */
int Game::getPlayerLives() const
{
	return _playerLives;
}

/**
 * @brief récupère le nombre de poivres des joueurs
 * 
 * @return int nombre de poivres
 */
int Game::getPlayerPeppers() const
{
	return _playerPeppers;
}

/**
 * @brief récupère l'état du jeu
 * 
 * @return GameState état du jeu
 */
GameState Game::getState() const
{
	return _state;
}

/**
 * @brief met le score du joueur
 * 
 * @param score 
 */
void Game::setPlayerScore(int score)
{
	if (score > _hiScore)
	{
		_hiScore = score;
	}
}

/**
 * @brief vérifie si le jeu est en pause
 * 
 * @return true 
 * @return false 
 */
bool Game::isPaused() const
{
	return _paused;
}

/**
 * @brief arrête la musique
 * 
 */
void Game::stopMusic()
{
	_musicWhileChoosing.stop();
	_musicWhilePlaying.stop();
}

/**
 * @brief vérifie si le son est activé
 * 
 * @return true 
 * @return false 
 */
const bool Game::isSoundEnabled() const
{
	return _soundEnabled;
}

/**
 * @brief active le son
 * 
 * @param enabled 
 */
void Game::enableSound(bool enabled)
{
	_soundEnabled = enabled;
	if (!_soundEnabled)
	{
		_musicWhilePlaying.stop();
		_musicWhileChoosing.stop();
		for (const auto& item : _sounds)
		{
			item->stop();
		}
	}
	else
	{
		switch (_state)
		{
		case GameChoice:
			_musicWhileChoosing.play();
			break;

		case GamePlaying:
			_musicWhilePlaying.play();
			break;
		}
	}
}

/**
 * @brief la touche est appuyée
 * 
 * @param event évènement pour lequel la touche est appuyée
 */
void Game::keyPress(const sf::Event& event)
{
	_keysState[event.key.code] = true;
}

/**
 * @brief la touche est relâchée
 * 
 * @param event évènement pour lequel la touche était appuyée
 */
void Game::keyRelease(const sf::Event& event)
{
	_keysState[event.key.code] = false;
}

/**
 * @brief joue le son en paramètre
 * 
 * @param sound 
 */
void Game::playSound(GameSound sound) const
{
	if (!_soundEnabled)
		return;

	if (_sounds[sound]->getStatus() == sf::SoundSource::Playing)
		return;

	_sounds[sound]->play();
}

/**
 * @brief traitement des évènements
 * 
 */
void Game::processEvents()
{
	// son
	if (_keysState[sf::Keyboard::Pause])
	{
		enableSound(!isSoundEnabled());
		_keysState[sf::Keyboard::Pause] = false;
	}

	// splash
	if (_state == GameSplash1 || _state == GameSplash2)
	{
		if (_keysState[sf::Keyboard::Escape])
		{
			goToChoice();
			_keysState[sf::Keyboard::Escape] = false;
		}
		return;
	}

	// ecran de choix
	if (_state == GameChoice)
	{
		if (_keysState[sf::Keyboard::Down] || _keysState[sf::Keyboard::Up])
		{
			auto choice = (int)_choices;
			choice += _keysState[sf::Keyboard::Down] ? 1 : -1;

			if (choice == ChoicesMax)
			{
				_choices = Choice1Player;
			}
			else if (choice < 0)
			{
				_choices = ChoiceQuit;
			}
			else
			{
				_choices = (GameChoices)choice;
			}

			_keysState[sf::Keyboard::Down] = false;
			_keysState[sf::Keyboard::Up] = false;
		}
		else if (_keysState[sf::Keyboard::Escape])
		{
			_state = GameQuitted;
			_keysState[sf::Keyboard::Escape] = false;
		}
		else if (_keysState[sf::Keyboard::Num1])
		{
			play(1);
			_keysState[sf::Keyboard::Num1] = false;
		}
		else if (_keysState[sf::Keyboard::Num2])
		{
			play(2);
			_keysState[sf::Keyboard::Num2] = false;
		}
		else if (_keysState[sf::Keyboard::Enter])
		{
			switch (_choices)
			{
			case Choice1Player:
				play(1);
				break;

			case Choice2Players:
				play(2);
				break;

			default: // ChoiceQuit
				_state = GameQuitted;
				break;
			}
			_keysState[sf::Keyboard::Enter] = false;
		}
		return;
	}

	// niveau fini?
	auto level = getCurrentLevel();
	if (level->isFinished())
	{
		if (_levelFinished)
		{
			auto wait = getFirstPlayerWithNoLivesLeft() ? gameOverTime : levelFinishedTime;
			if (_state != GameLevelFinished && _finishedClock.getElapsedTime().asSeconds() >= wait)
			{
				if (getFirstPlayerWithNoLivesLeft())
				{
					goToChoice();
				}
				else
				{
					_state = GameLevelFinished;
					_finishedClock.restart();
				}
			}
		}
		else
		{
			level->finish(nullptr, false);
			_levelFinished = true;
			_finishedClock.restart();
		}
		return;
	}

	if (_paused)
	{
		if (_keysState[sf::Keyboard::Q])
		{
			goToChoice();
			_paused = false;
			_keysState[sf::Keyboard::Q] = false;
			return;
		}
	}

	if (_keysState[sf::Keyboard::Escape])
	{
		if (getFirstPlayerWithNoLivesLeft())
		{
			_paused = false;
		}
		else
		{
			_paused = !_paused;
		}
		_keysState[sf::Keyboard::Escape] = false;
	}

	auto playersCount = getPlayersCount();

	// debug
	// + => ajoute un ennemi
	// - => supprime un ennemi
	if (_keysState[sf::Keyboard::Add])
	{
		level->spawnNewEnemy();
		_keysState[sf::Keyboard::Add] = false;
	}
	else if (_keysState[sf::Keyboard::Subtract])
	{
		level->killRandomEnemy();
		_keysState[sf::Keyboard::Subtract] = false;
	}

	// debug F1 => F6 on change de niveau
	for (auto i = 0; i < 6; i++)
	{
		if (_keysState[sf::Keyboard::F1 + i])
		{
			goToLevel(i);
			prepareForPlay(false, true);
			_keysState[sf::Keyboard::F1 + i] = false;
		}
	}

	// debug F11 to finish player 1
	if (_keysState[sf::Keyboard::F11])
	{
		if (playersCount > 0)
		{
			level->finish(_players.at(0), true);
			_keysState[sf::Keyboard::F11] = false;
		}
	}

	// debug F12 to finish player 2
	if (_keysState[sf::Keyboard::F12])
	{
		if (playersCount > 1)
		{
			level->finish(_players.at(1), true);
		}
		_keysState[sf::Keyboard::F12] = false;
	}

	// 2 joueurs?
	if (playersCount > 1)
	{
		// CTRL gauche envoie du poivre pour le joueur 2
		if (_keysState[sf::Keyboard::LControl])
		{
			_players.at(1)->sprayPepper();
		}

		// CTRL droit envoie du poivre pour le joueur 1
		if (_keysState[sf::Keyboard::RControl])
		{
			_players.at(0)->sprayPepper();
		}
	}
	else if (playersCount > 0)
	{
		// si 1 joueur, CTRL ou SPACE envoie du poivre
		if (_keysState[sf::Keyboard::Space] || _keysState[sf::Keyboard::LControl] || _keysState[sf::Keyboard::RControl])
		{
			_players.at(0)->sprayPepper();
		}
	}

	if (playersCount > 0)
	{
		// arrow keys
		if (_keysState[sf::Keyboard::Right])
		{
			_players.at(0)->moveFromKey(sf::Keyboard::Right);
		}
		else if (_keysState[sf::Keyboard::Left])
		{
			_players.at(0)->moveFromKey(sf::Keyboard::Left);
		}
		else if (_keysState[sf::Keyboard::Up])
		{
			_players.at(0)->moveFromKey(sf::Keyboard::Up);
		}
		else if (_keysState[sf::Keyboard::Down])
		{
			_players.at(0)->moveFromKey(sf::Keyboard::Down);
		}
		else
		{
			_players.at(0)->stand();
		}
	}

	if (playersCount > 1)
	{
		// DQZS keys
		if (_keysState[sf::Keyboard::D])
		{
			_players.at(1)->moveFromKey(sf::Keyboard::Right);
		}
		else if (_keysState[sf::Keyboard::Q])
		{
			_players.at(1)->moveFromKey(sf::Keyboard::Left);
		}
		else if (_keysState[sf::Keyboard::Z])
		{
			_players.at(1)->moveFromKey(sf::Keyboard::Up);
		}
		else if (_keysState[sf::Keyboard::S])
		{
			_players.at(1)->moveFromKey(sf::Keyboard::Down);
		}
		else
		{
			_players.at(1)->stand();
		}
	}
}

/**
 * @brief retourne un float aléatoire entre min et max
 * 
 * @param min 
 * @param max 
 * @return float 
 */
float Game::getRandom(float min, float max)
{
	return ((((float)std::rand()) / (float)RAND_MAX) * (max - min)) + min;
}

/**
 * @brief retourne un int aléatoire entre min et max
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int Game::getRandom(int min, int max)
{
	return std::rand() % (max - min + 1) + min;
}

/**
 * @brief retourne le module d'un vecteur passé en paramètre, en int
 * 
 * @param vector 
 * @return int 
 */
int Game::getDistance(sf::Vector2i vector)
{
	return (int)std::round(std::sqrt(vector.x * vector.x + vector.y * vector.y));
}

/**
 * @brief vérifie si au moins un joueur est mort
 * 
 * @return true 
 * @return false 
 */
bool Game::isAnyPlayerDead() const
{
	for (const auto& item : _players)
	{
		if (item->isDead())
			return true;
	}
	return false;
}

/**
 * @brief récupère le premier joueur qui n'a plus de vie
 * 
 * @return Player* 
 */
Player* Game::getFirstPlayerWithNoLivesLeft() const
{
	for (const auto& item : _players)
	{
		if (!item->getLives())
			return item;
	}
	return nullptr;
}

/**
 * @brief récupère le joueur d'index passé en paramètre
 * 
 * @param index 
 * @return joueur
 */
Player* Game::getPlayer(int index) const
{
	return _players.at(index);
}

/**
 * @brief récupère le nombre de joueurs
 * 
 * @return nombre de joueurs
 */
int Game::getPlayersCount() const
{
	return (int)_players.size();
}
/**
 * @brief récupère le niveau actuel
 * 
 * @return Level* niveau actuel
 */
Level* Game::getCurrentLevel() const
{
	return getLevel(_currentLevelNumber);
}
/**
 * @brief récupère le niveau dont le numéro est passé en paramètre
 * 
 * @param number numéro du niveau
 * @return Level* 
 */
Level* Game::getLevel(int number) const
{
	return _levels[number];
}
/**
 * @brief récupère l'échelle
 * 
 * @return const sf::Vector2i& 
 */
const sf::Vector2i& Game::getScale() const
{
	return _scale;
}
/**
 * @brief récupère la police de caractère
 * 
 * @return const sf::Font& 
 */
const sf::Font& Game::getFont() const
{
	return _font;
}
/**
 * @brief récupère le sprite sheet
 * 
 * @return const SpriteSheet& 
 */
const SpriteSheet& Game::getSpriteSheet() const
{
	return _spritesSheet;
}
/**
 * @brief va à un niveau dont le numéro est donné en paramètre
 * 
 * @param number numéro du niveau
 * @return Level* 
 */
Level* Game::goToLevel(int number)
{
	number = number % levelsCount;
	if (_currentLevelNumber != number)
	{
		// on detruit les ennemis du niveau precedent
		getCurrentLevel()->prepareForPlay(false, true);
		_currentLevelNumber = number;
	}
	return getCurrentLevel();
}

/**
 * @brief va au niveau suivant
 * 
 * @return Level* 
 */
Level* Game::goToNextLevel()
{
	return goToLevel(_currentLevelNumber + 1);
}

/**
 * @brief va au choix sélectionné
 * 
 */
void Game::goToChoice()
{
	_state = GameChoice;
	goToLevel(0);
	_musicWhilePlaying.stop();
	if (_soundEnabled)
	{
		_musicWhileChoosing.play();
	}
}

/**
 * @brief debug, affichage d'un bloc
 * 
 */
void Game::drawBlock(sf::RenderWindow& window, const sf::RenderStates& states, const char* text, float scale, const sf::Vector2i& blockPosition, const sf::Color& textColor, const sf::Color& backColor) const
{
	auto bounds = Level::getBlockPixelBounds(blockPosition);

	if (backColor != sf::Color::Transparent)
	{
		sf::RectangleShape rect;
		rect.setPosition((float)(bounds.left * _scale.x), (float)(bounds.top * _scale.y));
		rect.setSize(sf::Vector2f((float)(bounds.width * _scale.x), (float)(bounds.height * _scale.y)));
		rect.setFillColor(backColor);
		window.draw(rect, states);
	}

	// le texte est centre
	sf::Text t(text, _font, (int)(fontSize * scale * _scale.y));
	auto textBounds = t.getLocalBounds();

	t.setPosition(bounds.left * _scale.x + (bounds.width * _scale.x - textBounds.width) / 2, bounds.top * _scale.y + (bounds.height * _scale.y - textBounds.height) / 2);
	t.setFillColor(textColor);
	window.draw(t, states);
}

/**
 * @brief affichage d'un texte sur l'écran
 * 
 */
sf::FloatRect Game::drawText(sf::RenderWindow& window,
	sf::String text,
	sf::Color fillColor,
	textAlignment horizontalAlignment,
	textAlignment verticalAlignment,
	int horizontalMargin,
	int verticalMargin,
	float fontScale) const
{
	sf::Text t(text, _font, (int)(fontSize * _scale.y * fontScale));
	auto scaledBounds = t.getLocalBounds();

	float x;
	float y;
	switch (horizontalAlignment)
	{
	case TextNear: // a gauche du niveau
		x = (float)((horizontalMargin + (_screen.x - levelImageWidth) / 2) * _scale.x);
		break;

	case TextFar: // a droite du niveau
		x = (_screen.x - (_screen.x - levelImageWidth) / 2 - horizontalMargin) * _scale.x - scaledBounds.width;
		break;

	default: // case TextCenter:
		x = (_scale.x * (_screen.x + horizontalMargin) - scaledBounds.width) / 2;
		break;
	}

	switch (verticalAlignment)
	{
	case TextNear: // en haut de l'ecran
		y = (float)(_scale.y * verticalMargin);
		break;

	case TextFar: // en bas de l'ecran
		y = (_screen.y - verticalMargin) * _scale.y - scaledBounds.height;
		break;

	default: // case TextCenter:
		y = ((_screen.y + verticalMargin) * _scale.y - scaledBounds.height) / 2;
		break;

	}

	t.setPosition(x, y);
	t.setFillColor(fillColor);
	window.draw(t);
	return sf::FloatRect(x, y, scaledBounds.width, scaledBounds.height);
}

/**
 * @brief affiche les informations pour un joueur
 * 
 */
void Game::updatePlayerInfo(sf::RenderWindow& window, Player* player) const
{
	auto m = player->getNumber() % 2;
	auto horizontalAlignment = m ? TextFar : TextNear;
	char up[] = "1UP";
	up[0] = m ? '2' : '1';

	drawText(window, up, sf::Color::Red, horizontalAlignment, TextNear);
	drawText(window, std::to_string(player->getScore()).c_str(), sf::Color::White, horizontalAlignment, TextNear, 1, (int)(fontSize + 1));

	char peppers[32];
	snprintf(peppers, sizeof(peppers), "PEPPER %i", player->getPeppers());
	drawText(window, peppers, sf::Color::Green, horizontalAlignment, TextNear, 1, 2 * (int)(fontSize + 1), 0.5f);

	updatePlayerLives(window, player);
}

/**
 * @brief affiche les vies pour un joueur
 * 
 */
void Game::updatePlayerLives(sf::RenderWindow& window, Player* player) const
{
	auto m = player->getNumber() % 2;
	auto livesIcon = _spritesSheet.getSprite(PlayerIcon);
	auto liveBounds = livesIcon->getLocalBounds();
	for (auto i = 0; i < player->getLives(); i++)
	{
		auto x = _scale.x * ((_screen.x - levelImageWidth) / 2 - liveBounds.width);
		if (m)
		{
			x += (levelImageWidth + liveBounds.width) * _scale.x;
		}

		auto y = ((_screen.y - levelImageHeight) / 2 + levelImageHeight - liveBounds.height * i - 5) * _scale.y;
		livesIcon->setPosition(x, y);
		window.draw(*livesIcon);
	}
}

/**
 * @brief on affiche les niveaux horizontalement (note: meme a 2 joueurs, le nombre de niveaux est le meme)
 * 
 * @param window 
 */
void Game::updateTotalLevels(sf::RenderWindow& window) const
{
	auto burgerIcon = _spritesSheet.getSprite(BurgerIcon);

	// normalement il faudrait utiliser les burger5Icon et burger10Icon...
	auto burgerBounds = burgerIcon->getLocalBounds();
	auto y = (float)(((_screen.y - levelImageHeight) / 2 + levelImageHeight + 5) * _scale.y);
	for (auto i = 0; i < _totalLevels; i++)
	{
		auto x = _scale.x * ((_screen.x - levelImageWidth) / 2 + burgerBounds.width * i);
		burgerIcon->setPosition(x, y);
		window.draw(*burgerIcon);
	}
}

/**
 * @brief mise à jouer du jeu
 * 
 * @param window 
 * @param deltaTime 
 * @param states 
 */
void Game::update(sf::RenderWindow& window, const sf::Time& deltaTime, const sf::RenderStates& states)
{
	sf::Color blueColor(88, 172, 230);

	switch (_state)
	{
	case GameSplash1:
	{
		// affichage du splash, etape 1
		const int splashTime1 = 2; // sec
		auto ms = _splashClock.getElapsedTime().asMilliseconds();
		if (splashTime1 <= 0 || ms >= (splashTime1 * 1000))
		{
			// passage a l'etape 2
			_splashClock.restart();
			_state = GameSplash2;
			break;
		}

		if (_splashShader.isAvailable())
		{
			// on utilise un effet shader exemple de SFML...
			_splashShader.setUniform("wave_phase", _splashClock.getElapsedTime().asSeconds() * 2);

			const float baseAmp = 50;
			auto amp = baseAmp - (baseAmp * ms) / (splashTime1 * 1000);
			_splashShader.setUniform("wave_amplitude", sf::Vector2f(amp, amp));
		}

		auto color = _logoSprite.getColor();
		color.a = (256 * ms) / (splashTime1 * 1000);
		_logoSprite.setColor(color);
		_logoSprite.setPosition((_screen.x * _scale.x - _logoSprite.getLocalBounds().width) / 2, (_screen.y * _scale.y - _logoSprite.getLocalBounds().height) / 2);
		
		if (_splashShader.isAvailable())
		{
			window.draw(_logoSprite, &_splashShader);
		}
		else
		{
			window.draw(_logoSprite);
		}
	}
	break;

	case GameSplash2:
	{
		// affichage du splash, etape 2 (on stabilise)
		auto ms = _splashClock.getElapsedTime().asMilliseconds();
		const int splashTime2 = 2; // sec
		if (splashTime2 <= 0 || ms >= (splashTime2 * 1000))
		{
			// on peut jouer
			_splashClock.restart();
			goToChoice();
			break;
		}

		_logoSprite.setPosition((_screen.x * _scale.x - _logoSprite.getLocalBounds().width) / 2, (_screen.y * _scale.y - _logoSprite.getLocalBounds().height) / 2);

		if (_splashShader.isAvailable())
		{
			window.draw(_logoSprite, &_splashShader);
		}
		else
		{
			window.draw(_logoSprite);
		}
		drawText(window, appTitle, blueColor, TextCenter, TextFar, 1, 80, 2.1f);
	}
	break;

	case GameLevelFinished:
	{
		// le jeu est fini, on affiche un ecran d'attente
		auto anyDead = isAnyPlayerDead();
		if (_finishedClock.getElapsedTime().asSeconds() >= levelGameReadyTime)
		{
			if (!anyDead)
			{
				auto finishingPlayer = getCurrentLevel()->getFinishingPlayer();
				_totalLevels++;
				goToNextLevel();
			}
			prepareForPlay(false, false);
		}

		drawText(window, "GAME READY", sf::Color::White, TextCenter, TextCenter, 0, -10);
		char level[32];
		auto levelNumber = _totalLevels;
		if (!anyDead)
		{
			levelNumber++;
		}
		snprintf(level, sizeof(level), "LEVEL %i", levelNumber);
		drawText(window, level, sf::Color::White, TextCenter, TextCenter, 0, 10);
	}
	break;

	case GameChoice:
	{
		// choix de jeu
		drawText(window, appTitle, blueColor, TextCenter, TextNear, 0, 10);

		// on affiche les options qui doivent correspondre exactement a la liste des valeurs GameChoices dans le meme ordre
		auto choicePos = drawText(window, "1 Player ", blueColor, TextCenter, TextNear, 0, 50);
		drawText(window, "2 Players", blueColor, TextCenter, TextNear, 0, 70);
		drawText(window, "Quit     ", blueColor, TextCenter, TextNear, 0, 90);

		auto choiceOffset = (int)_choices;
		auto mod = (_finishedClock.getElapsedTime().asMilliseconds() / 100 % 3) - 1;
		drawText(window, L"\u25B6", sf::Color::White, TextNear, TextNear, 60 + mod, (int)(20 * choiceOffset + choicePos.top / _scale.y));

		drawText(window, L"Keyboard", blueColor, TextCenter, TextNear, 0, 130);

		drawText(window, L"Player 1 Moves: \u2190 \u2191 \u2192 \u2193", blueColor, TextCenter, TextNear, 0, 150);
		drawText(window, L"Player 2 Moves: Q Z D S", blueColor, TextCenter, TextNear, 0, 170);
		drawText(window, L"Spray Pepper: Left/Right CTRL", blueColor, TextCenter, TextNear, 0, 190);
		drawText(window, L"Pause: ESC", blueColor, TextCenter, TextNear, 0, 210);
	}
	break;

	default: // GamePlaying
		// en cours de jeu
		drawText(window, "HI-SCORE", sf::Color::Red, TextCenter, TextNear);
		drawText(window, std::to_string(_hiScore).c_str(), sf::Color::White, TextCenter, TextNear, 1, (int)(fontSize + 1));

		auto count = getPlayersCount();
		if (count > 0)
		{
			updatePlayerInfo(window, getPlayer(0));
			if (count > 1)
			{
				updatePlayerInfo(window, getPlayer(1));
			}
		}
		updateTotalLevels(window);
	}
}