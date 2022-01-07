#include "mainMenuState.h"
#include "GameState.h"

mainMenuState::mainMenuState(sf::RenderWindow* window, std::stack<State*>* states)
	:  State(window, states) 
{
	initBackground();
	initButtons();
}

mainMenuState::~mainMenuState() 
{
	std::cout << "MainMenuState destructor!\n";
	for (auto& it : buttons)
	{
		delete it.second;
	}
}

void mainMenuState::initBackground() 
{
	background.setSize((sf::Vector2f)window->getSize());
	background.setFillColor(sf::Color::Black);
}

void mainMenuState::initButtons() 
{
	this->buttons["START"] = new gui(sf::Vector2f(400, 150), sf::Vector2f(350, 100), sf::Color::White, std::string("START"), 50, sf::Color::Black, sf::Color::Yellow);
	this->buttons["EXIT"] = new gui(sf::Vector2f(400, 150), sf::Vector2f(350, 600), sf::Color::White, std::string("EXIT"), 50, sf::Color::Black, sf::Color::Yellow);
}

void mainMenuState::renderButtons() 
{
	for (auto it : buttons)
		it.second->render(window);
}

void mainMenuState::updateButtons() 
{
	if (buttons["START"]->isButtonPressed(mousePos))
		states->push(new GameState(window, states));

	if (buttons["EXIT"]->isButtonPressed(mousePos))
		delete states->top();
}

void mainMenuState::update() 
{
	updateSFMLEvents();
	updateMousePosition();
	updateDT();
	updateButtons();
}

void mainMenuState::render() 
{
	window->draw(background);
	renderButtons();
}
