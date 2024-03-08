#pragma once
#include "Button.h"

class Play : public Button
{
public:
	Play(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window, 
		 const float xCoord, const float yCoord);

	void reactToClick() override;
};