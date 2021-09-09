#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "partical.h"
#include "constrains.h"

class ClothEngin
{
private:

	sf::RenderWindow* window;
	sf::Clock clock;

	std::vector<partical>* particals;
	std::vector<constrains>* links;

	const sf::String title = "Cloth Simulation";
	int width = 1920;
	int heigth= 1080;
	int offSetx = 200;
	int offsety = 10;

	float gravitySize = 1000.0f;

	int particalsWidth;
	int particalsHeight;

	int particalRaduis = 1;
	int particalOffset = 5;
	float frictionCoefficient = 0.8f;
	float calculationStep = 0.0005;
	bool simulationRunning = true;

public:
	ClothEngin(int _particalWidth, int _particalsHeight)
	{
		particalsWidth = _particalWidth;
		particalsHeight = _particalsHeight;

		offSetx = (width - (particalsWidth * particalOffset)) / 2;
		offsety = (heigth - (particalsHeight * particalOffset)) / 2;

		initParticals(particalsWidth, particalsHeight);
		initConstrains(particalsWidth, particalsHeight);
	}
	ClothEngin() {}
	~ClothEngin()
	{
		delete links;
		delete particals;
		delete window;
	}
	void setWindowSize(int _width, int _height)
	{
		width = _width; heigth = _height;
	}

	void beginSimulation()
	{
		window = new sf::RenderWindow(sf::VideoMode(width, heigth), title);
		simulationLoop();
	}

	void simulationLoop()
	{
		clock.restart();
		while (window->isOpen())
		{
			getEvent();
			handleInput();
			if (simulationRunning)
				loopUpdate();
		}
	}
	void startSimulation()
	{
		simulationRunning = true;
		clock.restart();
	}
	void stopSimulation()
	{
		simulationRunning = false;
	}
private:

	void loopUpdate()
	{
		update();
		draw();
	}
	void update()
	{
		const int numSteps = static_cast<int>(getDt() / calculationStep);
		for (int i = 0; i < numSteps; i++)
		{
			apply_AirFriction_Gravity(calculationStep);
			for (constrains& link : *links)
			{
				link.solve();
			}
			for (partical& p : *particals)
			{
				p.updateDerivative(calculationStep);
			}
		}
	}

	void draw()
	{
		window->clear();
		//draw function go here
		for (auto p : (*particals))
		{
			sf::CircleShape dp(particalRaduis);
			dp.move(p.position);
			window->draw(dp);
		}

		window->display();
	}

	void handleInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			startSimulation();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			stopSimulation();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			auto position = sf::Mouse::getPosition((*window));
			applyForceToCloth(position);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			auto position = sf::Mouse::getPosition((*window));
			removeLinks(position);
		}
	}
	void removeLinks(sf::Vector2i position)
	{
		sf::Vector2f positionf = sf::Vector2f((float)position.x, (float)position.y);
		float maxLength = 5;
		for (constrains& l : (*links))
		{
			if (mathVec::distance(l.linkPosition(), positionf) < maxLength)
			{
				l.isBroken = true;
			}
		}
	}
	void applyForceToCloth(sf::Vector2i position)
	{
		sf::Vector2f positionf = sf::Vector2f((float)position.x, (float)position.y);
		sf::Vector2f force = sf::Vector2f(1.0f, 1.0f);

		float maxLength = 60;

		for (partical& p : (*particals))
		{
			if (mathVec::distance(p.position, positionf) < maxLength)
			{
				p.move(force);
			}
		}
	}
	float getDt()
	{
		return clock.restart().asSeconds();
	}
	void apply_AirFriction_Gravity(float dt)
	{
		auto gravity = sf::Vector2f(0.0f, gravitySize);

		for (partical& p : (*particals))
		{
			p.applyForce(gravity * p.mass);

			sf::Vector2f frictionForce = -p.velocity * frictionCoefficient;
			p.applyForce(frictionForce);

			p.update(dt);
		}
	}
	std::vector<sf::Event> getEvent()
	{
		std::vector<sf::Event> evts;
		sf::Event ev;
		while (window->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				window->close();
			}
			else
				evts.push_back(ev);
			//std::cout << ev.key.code << std::endl;
		}
		return evts;
	}
	void initParticals(int width, int heigth)
	{
		particals = new std::vector<partical>();
		int id = 0;
		for (int y = 0; y < heigth; y++)
		{
			for (int x = 0; x < width; x++)
			{
				//partical starting position
				float xpos = offSetx + x * particalOffset;
				float ypos = offsety + y * particalOffset;
				//create new partical
				partical newPartical(id,
					sf::Vector2f(xpos, ypos)
				);
				if (y == 0 && (x %6 ==  0 ))
					newPartical.isMoving = false;
				//add to partical array
				particals->push_back(newPartical);
				id++;
			}
		}
	}
	void initConstrains(int width, int height)
	{
		links = new std::vector<constrains>();
		int id = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (x > 0)
				{
					constrains newLink1(&(*particals)[id - 1], &(*particals)[(id)]);
					links->push_back(newLink1);
				}
				if (y > 0)
				{
					constrains newLink2(&(*particals)[id - width], &(*particals)[id]);
					links->push_back(newLink2);
				}
				id++;
			}
		}
	}
};