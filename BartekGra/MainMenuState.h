#pragma once
#include "State.h"
class mainMenuState : public State 
{
	sf::RectangleShape background;

	void initBackground();

	sf::Event sfEvent;

	void initButtons();
	void renderButtons();
	void updateButtons();



public:
	mainMenuState(sf::RenderWindow* window, std::stack<State*>* states);
	/*virtual*/ ~mainMenuState();


	void update();
	void render();

};

