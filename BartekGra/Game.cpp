#include "Game.h"

Game::Game() 
{
	initVariables();
	initWindow();
	initStates();
}

Game::~Game() 
{
	while (!states.empty())
	{
		delete states.top();
		states.pop();
	}
	
	window->~RenderWindow();
	delete this->window;

}

void Game::initVariables() 
{
	cellSize = 100;
}

void Game::initWindow() 
{
	this->window = new  sf::RenderWindow(sf::VideoMode(11* cellSize, 8*cellSize), "Checkers");
	this->window->setFramerateLimit(60);

}

void Game::initStates() 
{
	states.push(new mainMenuState(window, &states));
}

void Game::update() 
{
	if (!this->states.empty())
		this->states.top()->update();
	else
	{
		try
		{
			throw std::runtime_error("State stack empty!\n");
		}
		catch (std::runtime_error& e)
		{
			std::cout << e.what();
			window->close();
		}
	}
}

void Game::render() 
{
	window->clear();

	if (!this->states.empty())
		this->states.top()->render();
	else
	{
		try
		{
			throw std::runtime_error("State stack empty!\n");
		}
		catch (std::runtime_error& e)
		{
			std::cout << e.what();
			window->close();
		}
	}

	window->display();
}

void Game::run() 
{
	while (window->isOpen()) {
		update();
		render();
	}
}



