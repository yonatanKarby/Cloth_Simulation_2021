#pragma once
#include "SFML/System/Vector2.hpp"
#include "mathVector.h"

class partical
{
public:
	int particalId;
	float mass = 1;
	bool isMoving = true;
	sf::Vector2f position;
	sf::Vector2f prevPosition;
	sf::Vector2f velocity;
	sf::Vector2f force;

	partical(int id, sf::Vector2f startPos)
	{
		particalId = id;
		position = startPos;
		velocity = sf::Vector2f(0.0f, 0.0f);
		force = sf::Vector2f(0.0f, 0.0f);
		prevPosition = position;
	}
	partical() {}
	~partical() {}
	
	void applyForce(sf::Vector2f _force)
	{
		force += _force;
	}
	void update(float dt)
	{
		if (!isMoving) return;
		prevPosition = position;
		velocity += (force / mass) * dt;
		position += velocity * dt;
	}
	void updateDerivative(float dt)
	{
		velocity = (position - prevPosition) / dt;
		force = {};
	}
	void move(sf::Vector2f v)
	{
		if(isMoving)
			position += v;
	}
};