#pragma once
#include "State.h"
#include "Tile.h"
#include <fstream>
#include <sstream>
#include "King.h"

class GameState : public State 
{
// ==================================            VARIABLES             ============================
	Tile* destination, * origin, *inBetweenTile;
	sf::RectangleShape background;

	sf::Event sfEvent;

	Tile* board[8][8];


	int cellSize;
	unsigned int amountWhitePiece, amountBlackPiece;
	int amountOfPieces;


	int playerTurn; // -1 player1 turn, 1 player2 turn

	std::vector<sf::Vector2i> possibleJumps;

	//==============================           INITIALIZERS           ============================

	void initBackground();
	void initButtons();
	void initTiles();
	void initVariables();

	//==============================              UPDATES             ============================
	
	void updateButtons();
	void updateTiles();
	void updateWinCondition();
	void updatePromotions();
	
	//==============================              RENDERS             ============================

	void renderTiles();
	void renderButtons();

	//==============================               LOGIC              ============================

	void logic(Tile* tile);
	void move();
	bool checkLogicOrigin(Tile* tile);
	bool checkLogicDestination(Tile* tile);
	bool checkIfTakePossible(sf::Vector2i originCoords);

	bool intermediary(Tile* tile1, Tile* tile2);

	std::vector<sf::Vector2i> possibleCoordsToMultiJump;
	//bool checkIfMultiJump(sf::Vector2i checkAtOrigin);



	void save();
	void load();


public:
	GameState(sf::RenderWindow* window, std::stack<State*>* states);
	virtual ~GameState();
	void update();
	void render();
};


