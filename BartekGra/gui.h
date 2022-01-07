#pragma once
#include<SFML/Graphics.hpp>
#include <iostream>

class gui 
{
	sf::RectangleShape Button;
	sf::Font gameFont;
	sf::Text text;

public:
	gui(sf::Vector2f size, sf::Vector2f position, sf::Color color, std::string buttonText, int textSize, sf::Color textColor, sf::Color outlineColor);
	~gui() {};

	bool isButtonPressed(sf::Vector2f mousePos);
	void render(sf::RenderWindow* window);
};

