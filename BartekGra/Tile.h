#pragma once

#include "King.h"

enum class TileStates { TILE_IDLE = 0, TILE_HOVER, TILE_ACTIVE };


class Tile {
	//==============================              VARIABLES             ============================

	sf::Vector2i tileCoordinates;
	sf::RectangleShape tile, tileMask;

	int whatColor; // 0 empty, 1 white, -1 black

	TileStates tileState; // active inactive 

	bool tileStaysActive;

	Pawn* pawn;


	void setTilePosition(int x, int y);
	void setTileGraphics(int x, int y);
	void setTilePawn(int x, int y);

	void updateTilePiece();
	void updateTileState(const sf::Vector2f& mousePos);
	void updateTileColoring();


	int cellSize = 100;

public:
	Tile(int x, int y);
	~Tile();

	//==============================              SETTERS             ============================

	void setTileActive(bool active) { tileStaysActive = active; };
	void killTilePawn();
	void setPawn(Pawn* pawn);

	//==============================              GETTERS             ============================

	const bool isButtonPressed() const;
	const bool getIsActive() const;
	const sf::Vector2i getCoords();
	Pawn* GetPawn() { return this->pawn; };
	sf::RectangleShape getRectangle() { return tile; }

	//==============================              UPDATES             ============================

	void updateTile(const sf::Vector2f& mousePos);

	//==============================              RENDERS             ============================

	void renderTile(sf::RenderTarget& target);

	//==============================            LOAD/SAVE            ============================
	void loadTilePiece(int type);
	const int getTiletype() const;
};

