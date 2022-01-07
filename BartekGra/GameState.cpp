#include "GameState.h"
#include <typeinfo>

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states)
	: State(window, states) 
{
	initTiles();
	initButtons();
	initBackground();
	initVariables();
}

GameState::~GameState() 
{
	std::cout << "Gamestate destructor!\n";
	for (auto& it : buttons)
	{
		delete it.second;
	}

	delete inBetweenTile;
	delete origin;
	delete destination;
	inBetweenTile = origin = destination = nullptr;



	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			delete board[x][y];
		}
	}

}

void GameState::initBackground() 
{
	background.setSize((sf::Vector2f)window->getSize());
	background.setFillColor(sf::Color::Black);
}

void GameState::initTiles() 
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
			this->board[y][x] = new Tile(x, y);
	}
}

void GameState::initButtons() 
{
	buttons["SAVE"] = new gui(sf::Vector2f(200, 100), sf::Vector2f(850, 200), sf::Color::White, std::string("SAVE"), 35, sf::Color::Black, sf::Color::Yellow);
	buttons["LOAD"] = new gui(sf::Vector2f(200, 100), sf::Vector2f(850, 350), sf::Color::White, std::string("LOAD"), 35, sf::Color::Black, sf::Color::Yellow);
	buttons["EXIT"] = new gui(sf::Vector2f(200, 100), sf::Vector2f(850, 600), sf::Color::White, std::string("EXIT"), 35, sf::Color::Black, sf::Color::Yellow);
}

void GameState::initVariables() 
{
	cellSize = 100;

	amountOfPieces = 24;
	playerTurn = -1;
	amountWhitePiece = amountBlackPiece = 12;

	origin = destination = inBetweenTile = nullptr;
}

void GameState::update() 
{
	updateMousePosition(); // Mouse positions
	updateSFMLEvents(); // SFML Event objects
	updateTiles(); // update tiles, which update pawns
	updateButtons();
	updateKeytime(); // timing for pressing buttons
	updateDT();
	updateWinCondition(); //...
}

void GameState::updatePromotions() 
{
	if (typeid(*destination->GetPawn()) == typeid(King))
		return;

	destination->killTilePawn();
	King* king = new King("../Resources/Image/checkers.png", destination->getRectangle(), origin->getTiletype() < 0 ? 1 : 5);
	destination->setPawn(king);
	king = nullptr;
	std::cout << typeid(*destination->GetPawn()).name();

}

void GameState::updateTiles() 
{
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++) 
		{
			board[y][x]->updateTile(mousePos);

			if (board[y][x]->isButtonPressed() && getKeytime()) 
			{
				logic(board[y][x]);
			}
		}
}

void GameState::updateButtons() 
{
	if (buttons["SAVE"]->isButtonPressed(mousePos) && getKeytime())
		save();
	if (buttons["LOAD"]->isButtonPressed(mousePos) && getKeytime())
		load();
	if (buttons["EXIT"]->isButtonPressed(mousePos) && getKeytime())
		delete states->top();
}

void GameState::updateWinCondition() 
{
	if (amountBlackPiece <= 0) {
		std::cout << "Whites won!\n";
		window->close();
	}
	if (amountWhitePiece <= 0) {
		std::cout << "Blacks won!\n";
		window->close();
	}
}


void GameState::render() 
{
	window->draw(background);
	renderTiles();
	renderButtons();
}

void GameState::renderTiles() 
{
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			board[y][x]->renderTile(*window);
}

void GameState::renderButtons() 
{
	for (auto& it : buttons)
		it.second->render(window);
}


void GameState::logic(Tile* tile) 
{
	//if origin doesnt already exist, check if clicked tile is a valid origin
	if (!origin || origin == tile) 
	{
		if (checkLogicOrigin(tile));
	}

	if (origin)  // check if origin exist, without it no destination
	{	
		if (checkLogicDestination(tile));
	}

	// All logic checked. MOVE

	if (origin && destination) 
	{
		move();

		origin->setPawn(nullptr);
		destination->setTileActive(false);
		origin = destination = inBetweenTile = nullptr;
		playerTurn *= -1;
	}
}

bool GameState::checkLogicOrigin(Tile* tile) 
{
	//If there is no pawn on origin tile, it's not a valid origin
	if (!tile->GetPawn())
		return false;

	if (playerTurn == tile->GetPawn()->getPawnColor()) 
	{ // if -1 play black if 1 play white
		if (!origin) { // if there is no origin
			origin = tile;
			tile->setTileActive(true);
		}
		else if (origin == tile) 
		{// check if origin has been "unclicked"
			origin = nullptr;
			tile->setTileActive(false);
			return false;
		}
	}
	else 
	{
		return false;
	}

	return true;
}

bool GameState::checkLogicDestination(Tile* tile) 
{
	if (tile->GetPawn()) // IF IT HAS A PAWN - CANNOT MOVE ONTO THAT 
		return false;
	
	sf::Vector2i difference = origin->getCoords() - tile->getCoords();

	//LOAD POSSIBLE JUMPS
	checkIfTakePossible(origin->getCoords());

	//std::cout << "Desired jump:\n" << tile->getCoords().x << ", " << tile->getCoords().y << "\n";
	if (!possibleJumps.empty()) 
	{
		//std::cout << "Possible jumps:\n";

		int index = 0;
		bool isDestination = false;

		for (int index = 0; index < possibleJumps.size(); index++)
		{

		//	std::cout << possibleJumps[index].x << ", " << possibleJumps[index].y << "\n";

			if (possibleJumps[index] == tile->getCoords()) 
			{
			//	std::cout << " I AM TRUE";
				isDestination = true;
			}
		}

		if (isDestination == false)
			return false;
	}



	if (playerTurn == 1 && difference.y < 0 && typeid(*origin->GetPawn()) == typeid(Pawn)) 
	{
		//std::cout << "can't go back!\n";
		return false;
	}
	if (playerTurn == -1 && difference.y > 0 && typeid(*origin->GetPawn()) == typeid(Pawn)) 
	{
		//std::cout << "can't go back!\n";
		return false;
	}



	/*Check for diagonality of jump*/

	if (std::abs(difference.x) != std::abs(difference.y)) {
		//std::cout << "NOT DIAGONAL\n";
		return false;
	}

	/*Check for length of a jump*/

	int length = difference.x * difference.x + difference.y * difference.y;


	if (length > 8) {
		//std::cout << "Jump length too long!\n";
		return false;
	}

	//if there is an intermediate enemy pawn you can jump > 4
	if (length > 4) {
		//std::cout << "[INFO]Checking intermediary!\n";
		if (!intermediary(origin, tile)) // if doesnt exist, cant jump that far
			return false;
	}


	destination = tile;
	tile->setTileActive(tile);

	return true;
}

bool GameState::intermediary(Tile* tile1, Tile* tile2) 
{
	sf::Vector2i difference = (tile2->getCoords() - tile1->getCoords()) / 2;

	sf::Vector2i tilePosition = tile1->getCoords() + difference;

	if (board[tilePosition.y][tilePosition.x]->GetPawn()) 
	{
		if (playerTurn == board[tilePosition.y][tilePosition.x]->GetPawn()->getPawnColor()) 
		{
			//std::cout << "You cant jump over your own pawns!\n";
			return false;
		}
		else 
		{
			return inBetweenTile = board[tilePosition.y][tilePosition.x];
			return true;
		}
	}

	return false;
}

void GameState::move() 
{
	sf::Vector2i difference = origin->getCoords() - destination->getCoords();
	int length = difference.x * difference.x + difference.y * difference.y;

	origin->setTileActive(false);

	destination->setPawn(origin->GetPawn());

	origin->setPawn(nullptr);

	if (length > 4) 
	{ // if long jump, must jump over so kill
		if (inBetweenTile->GetPawn()->getPawnColor() == -1)
			amountBlackPiece--;
		if (inBetweenTile->GetPawn()->getPawnColor() == 1)
			amountWhitePiece--;

		inBetweenTile->killTilePawn();
	}


	if (destination->getCoords().y == 0 && destination->GetPawn()->getPawnColor() == 1) 
	{
		updatePromotions();
	}
	else if (destination->getCoords().y == 7 && destination->GetPawn()->getPawnColor() == -1)
	{
		updatePromotions();
	}
}

bool GameState::checkIfTakePossible(sf::Vector2i originCoords) 
{
	//PREPARE THE VECTOR 
	possibleJumps.clear();

	// CHECK AROUND THE PIECE
	for (int y = -1; y <= 1; y += 2) 
	{
		for (int x = -1; x <= 1; x += 2) 
		{
			//CHECK IF WE ARENT OUT OF BOUNDS OF THE BOARD
			if (originCoords.y + y + y <= 7 && originCoords.y + y + y >= 0 && originCoords.x + x + x <= 7 && originCoords.x + x + x >= 0) 
			{
				//CHECK IF AROUND THE IS A PIECE
				if (board[originCoords.y + y][originCoords.x + x]->GetPawn()) 
				{
					//CHECK IF ITS ENEMY PAWN
					if (board[originCoords.y + y][originCoords.x + x]->GetPawn()->getPawnColor() != playerTurn) 
					{
						// CHECK IF ITS DIAGONAL
						if (abs(x) == abs(y)) {
							//CHECK IF A TILE AFTER PIECE IS EMPTY - CAN JUMP THERE
							if (!board[originCoords.y + y + y][originCoords.x + x + x]->GetPawn()) 
							{
								//  CHECK PAWN CLASS - KING?
								if (typeid(*board[originCoords.y][originCoords.x]->GetPawn()) != typeid(King)) 
								{
									// CHECK IF ITS BLACK
									if (board[originCoords.y][originCoords.x]->GetPawn()->getPawnColor() == -1) 
									{
										// MAKE SURE HE DOESNT GO BACK
										if (originCoords.y + y >= originCoords.y) 
										{
											// FILL OUT THE VECTOR OF POSSIBLE JUMPS
											possibleJumps.push_back(sf::Vector2i(originCoords.x + x + x, originCoords.y + y + y));
										}
									}
									// CHECK IF ITS WHITE
									else if (board[originCoords.y][originCoords.x]->GetPawn()->getPawnColor() == 1) 
									{
										// MAKE SURE HE DOESNT GO BACK
										if (originCoords.y + y <= originCoords.y) 
										{
											std::cout << " I AM WHITE";
											// FILL OUT THE VECTOR OF POSSIBLE JUMPS
											possibleJumps.push_back(sf::Vector2i(originCoords.x + x + x, originCoords.y + y + y));				
										}
									}
								}
								// IF ITS A KING
								else if (typeid(*board[originCoords.y][originCoords.x]->GetPawn()) == typeid(King)) 
								{
									possibleJumps.push_back(sf::Vector2i(originCoords.x + x + x, originCoords.y + y + y));
								
								}
							}
						}
					}
				}
			}
		}
	}


	// IF THERE ARE POSSIBLE JUMPS
	if (!possibleJumps.empty())
		return true;

	//IF NOT
	return false;
}

void GameState::save() 
{
	std::string savingstring = "";

	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++) 
		{
			if (board[y][x]->GetPawn()) 
			{
				if (typeid(*board[y][x]->GetPawn()) == typeid(King)) 
				{
					board[y][x]->getTiletype() < 0 ? savingstring += "-2 " : savingstring += "2 "; 

				}
				else
					savingstring += std::to_string(board[y][x]->getTiletype()) + " ";
			}
			else
				savingstring += "0 ";
		}


	savingstring += std::to_string(playerTurn) + " ";
	std::ofstream file("saves/save.txt");
	file << savingstring;
	file.close();
}

void GameState::load() 
{
	std::ifstream file("saves/save.txt");
	int a, it{ 0 };


	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++) 
		{
			if (board[y][x]->GetPawn())
				board[y][x]->killTilePawn();
			board[y][x]->setTileActive(false);
		}

	origin = destination = inBetweenTile = nullptr;

	while (file >> a)
	{
		if (a && it < 64)
			board[it / 8][it % 8]->loadTilePiece(a);
		else
			playerTurn = a;
		it++;
	}

	file.close();
}

