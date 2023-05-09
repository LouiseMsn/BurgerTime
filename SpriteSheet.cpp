#include <SFML/Graphics.hpp>
#include "Headers/Sheet.h"
#include "Headers/SpriteSheet.h"

SpriteSheet::~SpriteSheet()
{
	for (const auto& item : _sprites)
	{
		delete item;
	}
}

sf::Sprite* SpriteSheet::getSprite(BurgerTechSprite sprite) const
{
	return _sprites[sprite];
}

bool SpriteSheet::loadBurgerSprites(const sf::Vector2i& scale)
{
	auto b = loadFromFile("Resources/sprites.png");
	if (!b)
		return b;

	setScale(scale);
	_sprites[BurgerTechSprite::Player1FrontWalk1] = loadSprite(sf::IntRect(0, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1FrontWalk1] = loadSprite(sf::IntRect(0, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1FrontWalk2] = loadSprite(sf::IntRect(16, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1FrontWalk3] = loadSprite(sf::IntRect(32, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1LeftWalk1] = loadSprite(sf::IntRect(48, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1LeftWalk2] = loadSprite(sf::IntRect(64, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1LeftWalk3] = loadSprite(sf::IntRect(80, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1RightWalk1] = loadSpriteFlipX(sf::IntRect(48, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1RightWalk2] = loadSpriteFlipX(sf::IntRect(64, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1RightWalk3] = loadSpriteFlipX(sf::IntRect(80, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1BackWalk1] = loadSprite(sf::IntRect(96, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1BackWalk2] = loadSprite(sf::IntRect(112, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1BackWalk3] = loadSprite(sf::IntRect(128, 0, 16, 16));
	_sprites[BurgerTechSprite::Player1PepperingFront] = loadSprite(sf::IntRect(0, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1PepperingLeft] = loadSprite(sf::IntRect(16, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1PepperingBack] = loadSprite(sf::IntRect(32, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1PepperingRight] = loadSpriteFlipX(sf::IntRect(16, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1Death1] = loadSprite(sf::IntRect(48, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1Death2] = loadSprite(sf::IntRect(64, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1Death3] = loadSprite(sf::IntRect(80, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1Death4] = loadSprite(sf::IntRect(96, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1Death5] = loadSprite(sf::IntRect(112, 16, 16, 16));
	_sprites[BurgerTechSprite::Player1Death6] = loadSprite(sf::IntRect(128, 16, 16, 16));

	_sprites[BurgerTechSprite::Player2FrontWalk1] = loadSprite(sf::IntRect(0, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2FrontWalk1] = loadSprite(sf::IntRect(0, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2FrontWalk2] = loadSprite(sf::IntRect(16, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2FrontWalk3] = loadSprite(sf::IntRect(32, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2LeftWalk1] = loadSprite(sf::IntRect(48, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2LeftWalk2] = loadSprite(sf::IntRect(64, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2LeftWalk3] = loadSprite(sf::IntRect(80, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2RightWalk1] = loadSpriteFlipX(sf::IntRect(48, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2RightWalk2] = loadSpriteFlipX(sf::IntRect(64, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2RightWalk3] = loadSpriteFlipX(sf::IntRect(80, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2BackWalk1] = loadSprite(sf::IntRect(96, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2BackWalk2] = loadSprite(sf::IntRect(112, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2BackWalk3] = loadSprite(sf::IntRect(128, 176, 16, 16));
	_sprites[BurgerTechSprite::Player2PepperingFront] = loadSprite(sf::IntRect(0, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2PepperingLeft] = loadSprite(sf::IntRect(16, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2PepperingBack] = loadSprite(sf::IntRect(32, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2PepperingRight] = loadSpriteFlipX(sf::IntRect(16, 16, 16, 16));
	_sprites[BurgerTechSprite::Player2Death1] = loadSprite(sf::IntRect(48, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2Death2] = loadSprite(sf::IntRect(64, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2Death3] = loadSprite(sf::IntRect(80, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2Death4] = loadSprite(sf::IntRect(96, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2Death5] = loadSprite(sf::IntRect(112, 192, 16, 16));
	_sprites[BurgerTechSprite::Player2Death6] = loadSprite(sf::IntRect(128, 192, 16, 16));

	_sprites[BurgerTechSprite::SausageDown1] = loadSprite(sf::IntRect(0, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageDown2] = loadSprite(sf::IntRect(16, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageLeft1] = loadSprite(sf::IntRect(32, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageLeft2] = loadSprite(sf::IntRect(48, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageRight1] = loadSpriteFlipX(sf::IntRect(32, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageRight2] = loadSpriteFlipX(sf::IntRect(48, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageUp1] = loadSprite(sf::IntRect(64, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageUp2] = loadSprite(sf::IntRect(80, 32, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedRight1] = loadSprite(sf::IntRect(0, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedRight2] = loadSprite(sf::IntRect(16, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedRight3] = loadSprite(sf::IntRect(32, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedRight4] = loadSprite(sf::IntRect(48, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedLeft1] = loadSpriteFlipX(sf::IntRect(0, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedLeft2] = loadSpriteFlipX(sf::IntRect(16, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedLeft3] = loadSpriteFlipX(sf::IntRect(32, 48, 16, 16));
	_sprites[BurgerTechSprite::SausageMashedLeft4] = loadSpriteFlipX(sf::IntRect(48, 48, 16, 16));
	_sprites[BurgerTechSprite::SausagePeppered1] = loadSprite(sf::IntRect(64, 48, 16, 16));
	_sprites[BurgerTechSprite::SausagePeppered2] = loadSprite(sf::IntRect(80, 48, 16, 16));

	_sprites[BurgerTechSprite::PickleDown1] = loadSprite(sf::IntRect(0, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleDown2] = loadSprite(sf::IntRect(16, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleLeft1] = loadSprite(sf::IntRect(32, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleLeft2] = loadSprite(sf::IntRect(48, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleRight1] = loadSpriteFlipX(sf::IntRect(32, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleRight2] = loadSpriteFlipX(sf::IntRect(48, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleUp1] = loadSprite(sf::IntRect(64, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleUp2] = loadSprite(sf::IntRect(80, 64, 16, 16));
	_sprites[BurgerTechSprite::PickleMashed1] = loadSprite(sf::IntRect(0, 80, 16, 16));
	_sprites[BurgerTechSprite::PickleMashed2] = loadSprite(sf::IntRect(16, 80, 16, 16));
	_sprites[BurgerTechSprite::PickleMashed3] = loadSprite(sf::IntRect(32, 80, 16, 16));
	_sprites[BurgerTechSprite::PickleMashed4] = loadSprite(sf::IntRect(48, 80, 16, 16));
	_sprites[BurgerTechSprite::PicklePeppered1] = loadSprite(sf::IntRect(64, 80, 16, 16));
	_sprites[BurgerTechSprite::PicklePeppered2] = loadSprite(sf::IntRect(80, 80, 16, 16));

	_sprites[BurgerTechSprite::EggDown1] = loadSprite(sf::IntRect(0, 96, 16, 16));
	_sprites[BurgerTechSprite::EggDown2] = loadSprite(sf::IntRect(16, 96, 16, 16));
	_sprites[BurgerTechSprite::EggLeft1] = loadSprite(sf::IntRect(32, 96, 16, 16));
	_sprites[BurgerTechSprite::EggLeft2] = loadSprite(sf::IntRect(48, 96, 16, 16));
	_sprites[BurgerTechSprite::EggRight1] = loadSpriteFlipX(sf::IntRect(32, 96, 16, 16));
	_sprites[BurgerTechSprite::EggRight2] = loadSpriteFlipX(sf::IntRect(48, 96, 16, 16));
	_sprites[BurgerTechSprite::EggUp1] = loadSprite(sf::IntRect(64, 96, 16, 16));
	_sprites[BurgerTechSprite::EggUp2] = loadSprite(sf::IntRect(80, 96, 16, 16));
	_sprites[BurgerTechSprite::EggMashed1] = loadSprite(sf::IntRect(0, 112, 16, 16));
	_sprites[BurgerTechSprite::EggMashed2] = loadSprite(sf::IntRect(16, 112, 16, 16));
	_sprites[BurgerTechSprite::EggMashed3] = loadSprite(sf::IntRect(32, 112, 16, 16));
	_sprites[BurgerTechSprite::EggMashed4] = loadSprite(sf::IntRect(48, 112, 16, 16));
	_sprites[BurgerTechSprite::EggPeppered1] = loadSprite(sf::IntRect(64, 112, 16, 16));
	_sprites[BurgerTechSprite::EggPeppered2] = loadSprite(sf::IntRect(80, 112, 16, 16));

	_sprites[BurgerTechSprite::Bonus1] = loadSprite(sf::IntRect(0, 128, 16, 16));
	_sprites[BurgerTechSprite::Bonus2] = loadSprite(sf::IntRect(16, 128, 16, 16));
	_sprites[BurgerTechSprite::Bonus3] = loadSprite(sf::IntRect(32, 128, 16, 16));

	_sprites[BurgerTechSprite::SliceBreadTop] = loadSprite(sf::IntRect(112, 49, 31, 7));
	_sprites[BurgerTechSprite::SliceBreadBottom] = loadSprite(sf::IntRect(112, 57, 31, 7));
	_sprites[BurgerTechSprite::SliceCheese] = loadSprite(sf::IntRect(112, 65, 31, 7));
	_sprites[BurgerTechSprite::SliceSteak] = loadSprite(sf::IntRect(112, 73, 31, 7));
	_sprites[BurgerTechSprite::SliceTomato] = loadSprite(sf::IntRect(112, 81, 31, 7));
	_sprites[BurgerTechSprite::SliceSalad] = loadSprite(sf::IntRect(112, 89, 31, 7));

	_sprites[BurgerTechSprite::PepperRight1] = loadSprite(sf::IntRect(176, 16, 16, 16));
	_sprites[BurgerTechSprite::PepperRight2] = loadSprite(sf::IntRect(192, 16, 16, 16));
	_sprites[BurgerTechSprite::PepperRight3] = loadSprite(sf::IntRect(208, 16, 16, 16));
	_sprites[BurgerTechSprite::PepperRight4] = loadSprite(sf::IntRect(226, 16, 16, 16));

	_sprites[BurgerTechSprite::PepperLeft1] = loadSpriteFlipX(sf::IntRect(176, 16, 16, 16));
	_sprites[BurgerTechSprite::PepperLeft2] = loadSpriteFlipX(sf::IntRect(192, 16, 16, 16));
	_sprites[BurgerTechSprite::PepperLeft3] = loadSpriteFlipX(sf::IntRect(208, 16, 16, 16));
	_sprites[BurgerTechSprite::PepperLeft4] = loadSpriteFlipX(sf::IntRect(226, 16, 16, 16));

	_sprites[BurgerTechSprite::PepperDown1] = loadSprite(sf::IntRect(176, 32, 16, 16));
	_sprites[BurgerTechSprite::PepperDown2] = loadSprite(sf::IntRect(192, 32, 16, 16));
	_sprites[BurgerTechSprite::PepperDown3] = loadSprite(sf::IntRect(208, 32, 16, 16));
	_sprites[BurgerTechSprite::PepperDown4] = loadSprite(sf::IntRect(226, 32, 16, 16));

	_sprites[BurgerTechSprite::PepperUp1] = loadSprite(sf::IntRect(176, 48, 16, 16));
	_sprites[BurgerTechSprite::PepperUp2] = loadSprite(sf::IntRect(192, 48, 16, 16));
	_sprites[BurgerTechSprite::PepperUp3] = loadSprite(sf::IntRect(208, 48, 16, 16));
	_sprites[BurgerTechSprite::PepperUp4] = loadSprite(sf::IntRect(226, 48, 16, 16));

	_sprites[BurgerTechSprite::PlayerIcon] = loadSprite(sf::IntRect(201, 0, 7, 8));
	_sprites[BurgerTechSprite::BurgerIcon] = loadSprite(sf::IntRect(209, 1, 7, 8));
	_sprites[BurgerTechSprite::Burger5Icon] = loadSprite(sf::IntRect(217, 1, 7, 8));
	_sprites[BurgerTechSprite::Burger10Icon] = loadSprite(sf::IntRect(225, 1, 7, 8));
	_sprites[BurgerTechSprite::PepperIcon] = loadSprite(sf::IntRect(233, 1, 7, 8));
	return true;
}