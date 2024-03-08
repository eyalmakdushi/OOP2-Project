#pragma once
#include "Button.h"

class Exit : public Button
{
public:
    Exit(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window, 
         const float xCoord, const float yCoord);

    void reactToClick() override;
};