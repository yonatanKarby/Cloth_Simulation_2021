#pragma once
#include <iostream>
#include "partical.h"
#include "SFML/System/Vector2.hpp"
#include "mathVector.h"

class constrains
{
public:
	partical* partical1;
	partical* partical2;

	float length;
	float length_coefficient = 1.4f;
	float strength = 0.8f;
	bool isBroken = false;

	constrains(partical* _p1, partical* _p2)
	{
		partical1 = _p1;
		partical2 = _p2;

		length = mathVec::length(_p1->position - _p2->position);
	}
	~constrains() {}

	void solve()
	{
		if (isBroken) return;
		sf::Vector2f diff_vec = partical1->position - partical2->position;
		float diff_len = mathVec::length(diff_vec);
		if (diff_len > length)
		{
			isBroken = diff_len > (length * length_coefficient);

			const sf::Vector2f n = diff_vec / diff_len;
			const float c = length - diff_len;
			const sf::Vector2f p = -(c * strength) / (partical1->mass + partical2->mass) * n;

			partical1->move(-p / partical1->mass);
			partical2->move(p / partical2->mass);
		}
	}
	sf::Vector2f linkPosition()
	{
		return mathVec::avg(partical1->position, partical2->position);
	}
};
