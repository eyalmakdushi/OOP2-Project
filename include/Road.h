#pragma once
#include "BaseObject.h"

class Road : public BaseObject
{
public:
	Road(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& directionVector);
};