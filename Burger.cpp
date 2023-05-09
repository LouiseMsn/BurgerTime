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

Burger::Burger(int number)
{
	_number = number;
}

Burger::~Burger()
{
	for (const auto& item : _slices)
	{
		delete item;
	}
}

bool Burger::isComplete() const
{
	for (const auto& item : _slices)
	{
		if (item->getState() != SliceStacked)
			return false;
	}
	return true;
}

int Burger::getNumber() const
{
	return _number;
}

void Burger::addSlice(Slice* slice)
{
	_slices.push_back(slice);
}

int Burger::getSliceCount() const
{
	return (int)_slices.size();
}

Slice* Burger::getSlice(int index) const
{
	return _slices.at(index);
}
