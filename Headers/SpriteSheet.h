#pragma once

// la liste de tous les sprites
// la plupart sont dans le fichier png
// mais certains doivent etre deduits des autres
enum BurgerTechSprite
{
	Player1FrontWalk1,
	Player1FrontWalk2,
	Player1FrontWalk3,

	Player1LeftWalk1,
	Player1LeftWalk2,
	Player1LeftWalk3,

	// flipx
	Player1RightWalk1,
	Player1RightWalk2,
	Player1RightWalk3,

	Player1BackWalk1,
	Player1BackWalk2,
	Player1BackWalk3,

	Player1PepperingFront,
	Player1PepperingLeft,
	Player1PepperingBack,

	// flipx
	Player1PepperingRight,

	Player1Death1,
	Player1Death2,
	Player1Death3,
	Player1Death4,
	Player1Death5,
	Player1Death6,

	Player2FrontWalk1,
	Player2FrontWalk2,
	Player2FrontWalk3,

	Player2LeftWalk1,
	Player2LeftWalk2,
	Player2LeftWalk3,

	// flipx
	Player2RightWalk1,
	Player2RightWalk2,
	Player2RightWalk3,

	Player2BackWalk1,
	Player2BackWalk2,
	Player2BackWalk3,

	Player2PepperingFront,
	Player2PepperingLeft,
	Player2PepperingBack,

	// flipx
	Player2PepperingRight,

	Player2Death1,
	Player2Death2,
	Player2Death3,
	Player2Death4,
	Player2Death5,
	Player2Death6,

	SausageDown1,
	SausageDown2,

	SausageLeft1,
	SausageLeft2,

	// flipx
	SausageRight1,
	SausageRight2,

	SausageUp1,
	SausageUp2,

	SausageMashedRight1,
	SausageMashedRight2,
	SausageMashedRight3,
	SausageMashedRight4,

	SausagePeppered1,
	SausagePeppered2,

	EggDown1,
	EggDown2,

	EggLeft1,
	EggLeft2,

	// flipx
	EggRight1,
	EggRight2,

	EggUp1,
	EggUp2,

	EggMashed1,
	EggMashed2,
	EggMashed3,
	EggMashed4,

	EggPeppered1,
	EggPeppered2,

	PickleDown1,
	PickleDown2,

	PickleLeft1,
	PickleLeft2,

	// flipx
	PickleRight1,
	PickleRight2,

	PickleUp1,
	PickleUp2,

	PickleMashed1,
	PickleMashed2,
	PickleMashed3,
	PickleMashed4,

	PicklePeppered1,
	PicklePeppered2,

	Bonus1,
	Bonus2,
	Bonus3,

	SliceBreadTop,
	SliceBreadBottom,
	SliceCheese,
	SliceSteak,
	SliceTomato,
	SliceSalad,

	PepperRight1,
	PepperRight2,
	PepperRight3,
	PepperRight4,

	// flipx
	PepperLeft1,
	PepperLeft2,
	PepperLeft3,
	PepperLeft4,

	PepperDown1,
	PepperDown2,
	PepperDown3,
	PepperDown4,

	PepperUp1,
	PepperUp2,
	PepperUp3,
	PepperUp4,

	// flipx
	SausageMashedLeft1,
	SausageMashedLeft2,
	SausageMashedLeft3,
	SausageMashedLeft4,

	PlayerIcon,
	BurgerIcon,
	Burger5Icon,
	Burger10Icon,
	PepperIcon,

	// permet de reperer le nombre max de sprites
	SpritesMax,

	// ici ce ne sont pas des sprites mais des valeurs speciales
	NoSlice,
	Plate,
};

class SpriteSheet : public Sheet
{
	sf::Sprite* _sprites[SpritesMax];

public:
	~SpriteSheet();
	bool loadBurgerSprites(const sf::Vector2i& scale = sf::Vector2i(1, 1));

	sf::Sprite* getSprite(BurgerTechSprite sprite) const;
};

