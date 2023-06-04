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

/**
 * @brief Construit un burger
 * 
 * @param number numéro du burger
 */
Burger::Burger(int number)
{
	_number = number;
}

/**
 * @brief Destructeur Burger
 * 
 */
Burger::~Burger()
{
	for (const auto& item : _slices)
	{
		delete item;
	}
}

/**
 * @brief vérifie si un burger est complet
 * 
 * @return true 
 * @return false 
 */
bool Burger::isComplete() const
{
	for (const auto& item : _slices)
	{
		if (item->getState() != SliceStacked)
			return false;
	}
	return true;
}

/**
 * @brief récupère le numéro d'un burger
 * 
 * @return int numéro du burger
 */
int Burger::getNumber() const
{
	return _number;
}

/**
 * @brief ajoute une tranche au burger
 * 
 * @param slice tranche à ajouter
 */
void Burger::addSlice(Slice* slice)
{
	_slices.push_back(slice);
}

/**
 * @brief récupère le nombre de tranches
 * 
 * @return int 
 */
int Burger::getSliceCount() const
{
	return (int)_slices.size();
}

/**
 * @brief récupère la tranche à l'index indiqué en paramètre
 * 
 * @param index 
 * @return Slice* 
 */
Slice* Burger::getSlice(int index) const
{
	return _slices.at(index);
}
