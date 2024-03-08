#include "Play.h"
#include "Controller.h"

Play::Play(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window, 
		   const float xCoord, const float yCoord)
	: Button (normalTexture, hoveredTexture, window, xCoord, yCoord)
{}

void Play::reactToClick()
{
	Resources::instance().stopMenuMusic();
	closeWindow();

	Resources::instance().playGameMusic();
	Controller().startGame();

	Resources::instance().playMenuMusic();
	showMenu();
}