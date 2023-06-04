#pragma once

/**
 * @brief Hamburger composé de tranche à compléter pour terminer les niveaux
 * 
 */
class Burger
{
	int _number;
	std::vector<Slice*> _slices;

public:
	Burger(int number);
	~Burger();

	bool isComplete() const;
	int getNumber() const;
	void addSlice(Slice* slice);
	int getSliceCount() const;
	Slice* getSlice(int index) const;
};
