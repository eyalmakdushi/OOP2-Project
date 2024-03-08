#include "Help.h"

Help::Help(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window,
			const float xCoord, const float yCoord, const sf::Texture& howToPlayTexture)
	: Button(normalTexture, hoveredTexture, window, xCoord, yCoord), m_howToPlay(sf::Sprite(howToPlayTexture))
{}

void Help::reactToClick()
{
	showHowToPlay(m_howToPlay);
}