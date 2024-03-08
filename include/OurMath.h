#pragma once
#include <SFML/Graphics.hpp>

class OurMath
{
public:
	static float euclideanDistance(const sf::Vector2f& coord1, const sf::Vector2f& coord2);
	static sf::Vector2f directionalVector(const sf::Vector2f& coord1, const sf::Vector2f& coord2);
	static float calculateAngle(const sf::Vector2f& directionVec);
};