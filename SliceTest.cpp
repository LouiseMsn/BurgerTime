#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Headers/Constants.h"
#include "Headers/Sheet.h"
#include "Headers/Updatable.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Level.h"
#include "Headers/Game.h"
#include "Headers/Movable.h"
#include "Headers/Player.h"
#include "Headers/Enemy.h"
#include "Headers/Slice.h"
#include "Headers/Burger.h"

TEST_CASE("Slice", "[Slice]")
{
    SECTION("Gets"){
        sf::Vector2i screen(240, 256);
        bool soundEnabled = false;
        sf::Vector2i scale(3, 3);
        int playerLives = 3;
        int playerPeppers = 5;
        int startLevel = 0;

        Game game(screen, scale, playerLives, playerPeppers, startLevel, soundEnabled);
        Burger* burger = new Burger(1);
        auto slice = new Slice(&game, burger, 1, SliceTomato, 0) ;
        
        REQUIRE(slice->getVerticalPosition() == 0);
        REQUIRE(slice->getNumber() == 1);

        // Les tests avec des objets ou types propres au programme ne fonctionnent pas
        
        //REQUIRE(slice->getBurger == burger);
        //REQUIRE(slice->getSliceType == SliceTomato);
    }
}