#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Updatable.h"
#include "Headers/Level.h"
#include "Headers/Game.h"

// valeurs par defaut
int playerLives = 3;
int playerPeppers = 5;
int startLevel = 0;
int windowStyle = sf::Style::Default;
bool soundEnabled = true;

/**
 * @brief MAIN
 * on boucle sur les arguments passés en ligne de commande:
 * -l nombre de vies
 *-p nombre de poivres
 *-s échelle
 *-ws style de fenêtre
 *-w largeur de la fenêtre
 *-h hauteur de la fenêtre
 *-t niveau de départ
 *-so 0 ou 1 (activer ou couper le son)
 * On cree le jeu avec les parametres passes ou par default
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv)
{
	// la taille de l'ecran complet avant mise a l'echelle
	sf::Vector2i screen(240, 256);

	// mise a l'echelle
	sf::Vector2i scale(3, 3);

	// on boucle sur les arguments passes en ligne de command
	std::vector<std::string> args(argv + 1, argv + argc);
	for (auto arg = args.begin(); arg != args.end(); ++arg)
	{
		if (*arg == "-h" || *arg == "-?" || *arg == "--help")
		{
			std::cout << "Syntax: BurgerTech [options]" << std::endl;
			std::cout << std::endl;
			std::cout << "Options:" << std::endl;
			std::cout << "    -l  <lives>" << std::endl;
			std::cout << "    -p  <peppers>" << std::endl;
			std::cout << "    -s  <scale>" << std::endl;
			std::cout << "    -ws <window syle>" << std::endl;
			std::cout << "    -w  <window width>" << std::endl;
			std::cout << "    -h  <window height>" << std::endl;
			std::cout << "    -t  <start level>" << std::endl;
			std::cout << "    -so <0 or 1>" << std::endl;
			return 0;
		}

		if (*arg == "-l")
		{
			// nombre de vies
			auto lives = atoi((*++arg).c_str());
			if (lives > 0)
			{
				playerLives = lives;
			}
			continue;
		}

		if (*arg == "-s")
		{
			// echelle
			auto sc = atoi((*++arg).c_str());
			if (sc >= 1)
			{
				scale.x = sc;
				scale.y = sc;
			}
			continue;
		}

		if (*arg == "-p")
		{
			// nombre de poivres
			auto peppers = atoi((*++arg).c_str());
			if (peppers > 0)
			{
				playerPeppers = peppers;
			}
			continue;
		}

		if (*arg == "-ws")
		{
			// type d'ecran (plein, etc.)
			auto ws = atoi((*++arg).c_str());
			if (ws >= 1)
			{
				windowStyle = ws;
			}
			continue;
		}

		if (*arg == "-w")
		{
			// largeur
			auto w = atoi((*++arg).c_str());
			if (w >= 200)
			{
				screen.x = w;
			}
			continue;
		}

		if (*arg == "-h")
		{
			// hauteur
			auto h = atoi((*++arg).c_str());
			if (h >= 200)
			{
				screen.x = h;
			}
			continue;
		}

		if (*arg == "-t")
		{
			// niveau de depart
			auto t = atoi((*++arg).c_str());
			if (t >= 1)
			{
				startLevel = t - 1;
			}
			continue;
		}

		if (*arg == "-so")
		{
			// son actif ou pas
			auto so = atoi((*++arg).c_str());
			if (!so)
			{
				soundEnabled = false;
			}
			continue;
		}
	}

	sf::RenderWindow window(sf::VideoMode((int)(screen.x * scale.x), (int)(screen.y * scale.y)), appTitle, windowStyle);
	window.setKeyRepeatEnabled(false); // la detection de repetition prend du temps, or on veut reagir immediatement meme quand une touche est continuellement appuyee

	// on retaille par rapport a la fenetre creee (plein ecran ou pas)
	auto ws = window.getSize();

	// on conserve le ratio
	auto ratio = (ws.y / (float)screen.y) / scale.y;
	scale.y = ws.y / screen.y;
	scale.x = (int)(scale.x * ratio);

	// on cree le jeu avec les parametres passes ou par default
	Game game(screen, scale, playerLives, playerPeppers, startLevel, soundEnabled);

	// offset entre le niveau et ecran
	sf::Transform movableTransform;
	auto tx = (float)((screen.x - levelImageWidth) / 2 * scale.x);
	auto ty = (float)((screen.y - levelImageHeight) / 2 * scale.y);
	movableTransform.translate(tx, ty);
	sf::RenderStates state(movableTransform);

	sf::Clock clock;
	while (window.isOpen())
	{
		if (game.getState() == GameQuitted)
			break;

		auto deltaTime = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				game.keyPress(event);
				break;

			case sf::Event::KeyReleased:
				game.keyRelease(event);
				break;
			}
		}

		auto level = game.getCurrentLevel();
		window.clear();
		game.processEvents();
		level->processEvents();
		game.update(window, deltaTime, state);
		level->update(window, deltaTime, state);
		window.display();
	}
	return 0;
}