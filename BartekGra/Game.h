#pragma once
#include "mainMenuState.h"

class Game {

	std::stack<State*> states;
	sf::RenderWindow* window;

	int cellSize;

	void initWindow();
	void initVariables();  
	void initStates();

	void update();
	void updateSFMLEvents();
	
	void render();

public:

	Game();
	~Game();
	void run();
};

