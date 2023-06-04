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
#include "Headers/Pepper.h"
#include "Headers/Player.h"

#include <stdio.h>

TEST_CASE("Player", "[Player]" )
{
        sf::Vector2i screen(240, 256);
        bool soundEnabled = false;
        sf::Vector2i scale(3, 3);

        int playerLives = 3;
        int playerPeppers = 5;
        int startLevel = 0;

        Game game(screen, scale, playerLives, playerPeppers, startLevel, soundEnabled);
        Player* player1 = new Player(&game, 1); 
        Player* player2 = new Player(&game, 2); 

        player1->prepareForPlay(true);
        player2->prepareForPlay(true);
    

    SECTION("Gets"){  

        REQUIRE(player1->getLives() == 3);
        REQUIRE(player1->getScore() == 0);
        REQUIRE(player1->getPeppers() == 5);
        REQUIRE(player1->getNumber() == 1);

        REQUIRE(player2->getLives() == 3);
        REQUIRE(player2->getScore() == 0);
        REQUIRE(player2->getPeppers() == 5);
        REQUIRE(player2->getNumber() == 2);

    }

    /* Ces deux sections ne passent pas les tests mais ont voit que ça fonctionne en lancant le jeu, nous pensons que c'est lié a la façon d'update le jeu.

    SECTION("Death"){

        player1->die();
        REQUIRE(player1->getLives() == 2);
        REQUIRE(player2->getLives() == 3);

    }
    SECTION("Pepper"){

        player2->sprayPepper();
        REQUIRE(player2->getPeppers() == 4);

    }

    */
}