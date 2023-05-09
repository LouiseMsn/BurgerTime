#pragma once

class Bonus : public Movable
{
	int _score;
	bool _scored;

public:
	Bonus(Game* game);

	bool isScored() const;
	void setScored();
	int getScore() const;
};

