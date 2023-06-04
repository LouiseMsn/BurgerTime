#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
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


TEST_CASE("Game", "[Game]" )
{
    sf::Vector2i screen(240, 256);
    bool soundEnabled = false;
    sf::Vector2i scale(3, 3);
    int playerLives = 3;
    int playerPeppers = 5;
    int startLevel = 0;

    Game game(screen, scale, playerLives, playerPeppers, startLevel, soundEnabled);

    SECTION("Gets"){

        REQUIRE(game.getPlayerLives() == 3);
        REQUIRE(game.getPlayerPeppers() == 5);
        REQUIRE(game.getPlayersCount() == 0);
    }

    SECTION("Randoms")
    {
        int rand1;
        float rand2;
        float min = 0.0;
        float max = 20.0;
        
        for (int i = 0; i < 10; i++)
        {
            rand1 = game.getRandom(0,20);
            REQUIRE(rand1 <= 20);
            REQUIRE(rand1 >= 0);

            rand2 = game.getRandom(min,max);
            REQUIRE(rand2 <= 20);
            REQUIRE(rand2 >= 0);
        }
    
    }
}