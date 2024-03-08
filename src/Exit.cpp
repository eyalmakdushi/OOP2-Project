#include "Exit.h"

Exit::Exit(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window,
			const float xCoord, const float yCoord) 
	: Button(normalTexture, hoveredTexture, window, xCoord, yCoord)
{}

void Exit::reactToClick()
{
	closeWindow();
}