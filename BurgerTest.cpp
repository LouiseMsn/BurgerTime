#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Headers/Updatable.h"
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Movable.h"
#include "Headers/Slice.h"
#include "Headers/Burger.h"
#include "Headers/Constants.h"
#include "Headers/Level.h"
#include "Headers/Game.h"

TEST_CASE("Burger", "[Burger]" )
{
    SECTION("Constructor"){
        Burger* burger = new Burger(1);
        REQUIRE(burger->getNumber() == 1);
    }

    SECTION("Add slices"){
        sf::Vector2i screen(240, 256);
        bool soundEnabled = false;
        sf::Vector2i scale(3, 3);
        int playerLives = 3;
        int playerPeppers = 5;
        int startLevel = 0;

        Game game(screen, scale, playerLives, playerPeppers, startLevel, soundEnabled);
        Burger* burger = new Burger(1);
        auto slice = new Slice(&game, burger, 1, SliceTomato, 0) ;
        burger->addSlice(slice);
        REQUIRE(burger->getSliceCount() == 1 );
    }

} 