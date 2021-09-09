#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "partical.h"
#include "constrains.h"
#include "ClothEngin.h"

//key function interface
class keyFunctionality
{
public:
	sf::Keyboard::Key key;

	virtual void onKey(){}
};

class keyBoardHandler
{
public:
	std::vector<keyFunctionality> keyFunctionalitys;
	keyBoardHandler()
	{	
		keyFunctionalitys = std::vector<keyFunctionality>();
	}

	void addKeyFunctionality(keyFunctionality keyFunctionality)
	{
		keyFunctionalitys.push_back(keyFunctionality);
	}

	void update()
	{
		for (auto key : keyFunctionalitys)
		{
			if (sf::Keyboard::isKeyPressed(key.key))
				key.onKey();
		}
	}
};


class stopGame_key : public keyFunctionality
{
public:
	sf::Keyboard::Key key;
	ClothEngin* engine;

	stopGame_key(sf::Keyboard::Key _key, ClothEngin* _engine)
	{
		key = _key;
		engine = _engine;
	}
	void onKey()
	{
		engine->stopSimulation();
	}
};


class startGame_key : public keyFunctionality
{
public:
	sf::Keyboard::Key key;
	ClothEngin* engine;

	startGame_key(sf::Keyboard::Key _key, ClothEngin* _engine)
	{
		key = _key;
		engine = _engine;
	}
	void onKey()
	{
		engine->stopSimulation();
	}
};