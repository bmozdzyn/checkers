#include "gui.h"



gui::gui(sf::Vector2f size, sf::Vector2f position, sf::Color color, std::string buttonText, int textSize, sf::Color textColor, sf::Color outlineColor)
{
	Button.setSize(size);
	Button.setPosition(position);
	Button.setFillColor(color);
	Button.setOutlineThickness(5);
	Button.setOutlineColor(outlineColor);

	gameFont.loadFromFile("../Resources/font/font.ttf");
	text.setFont(gameFont);
	text.setString(buttonText);
	text.setCharacterSize(textSize);
	text.setFillColor(textColor);

	text.setPosition(((position.x + Button.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
		((position.y + Button.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2)));

}

bool gui::isButtonPressed(sf::Vector2f mousePos) 
{
	if (Button.getGlobalBounds().contains(mousePos))
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			return true;

	return false;
	
}

void gui::render(sf::RenderWindow* window)
{
	window->draw(Button);
	window->draw(text);
}
	