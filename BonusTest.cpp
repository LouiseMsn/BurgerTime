#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Bonus.h"

TEST_CASE("Bonus", "[Bonus]" )
{

	sf::Vector2i screen(240, 256);
	bool soundEnabled = false;
	sf::Vector2i scale(3, 3);
	int playerLives = 3;
	int playerPeppers = 5;
	int startLevel = 0;

	Game game(screen, scale, playerLives, playerPeppers, startLevel, soundEnabled);
	Bonus bonus(&game);

    REQUIRE(bonus.isScored() == false);
    bonus.setScored();
    REQUIRE(bonus.isScored()==true);
	REQUIRE(bonus.getScore() == 1000);

}