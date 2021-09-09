#pragma once
#include "SFML/System/Vector2.hpp"

struct mathVec
{
public:
	static float length(const sf::Vector2f& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	static float distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		float x = v1.x - v2.x;
		float y = v1.y - v2.y;
		return sqrt(x * x + y * y);
	}
	static sf::Vector2f avg(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		return sf::Vector2f((v1.x + v2.x) / 2, (v1.y + v2.y) / 2);
	}
};