#pragma once
#include "Button.h"

class Help : public Button
{
public:
    Help(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window, 
         const float xCoord, const float yCoord, const sf::Texture& howToPlayTexture);

    void reactToClick() override;

private:
    sf::Sprite m_howToPlay;
};