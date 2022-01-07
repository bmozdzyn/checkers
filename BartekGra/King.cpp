#include "King.h"

void King::setPawnColor(unsigned heightY) 
{
	whatColor = 0; // none

	if (heightY < 3) {
		pawnSprite.setTextureRect(sf::IntRect(0 * 32, 0, 32, 32));
		whatColor = -1; // black
	}
	if (heightY > 4) {
		pawnSprite.setTextureRect(sf::IntRect(1 * 32, 0, 32, 32));
		whatColor = 1; //white
	}
}

King::King(const std::string texturePath, const sf::RectangleShape& tileBox, unsigned heightY)
{
	setPawnGraphics(texturePath);
	setPawnPosition(tileBox);
	setPawnColor(heightY);
	std::cout << "Creating king\n";

}
