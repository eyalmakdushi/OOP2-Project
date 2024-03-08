#include <cmath>
#include "OurMath.h"
#include <numbers> //for pi

float OurMath::euclideanDistance(const sf::Vector2f& coord1, const sf::Vector2f& coord2)
{
    return std::sqrtf(std::powf(coord1.x - coord2.x, 2.f) + std::powf(coord1.y - coord2.y, 2.f));
}

sf::Vector2f OurMath::directionalVector(const sf::Vector2f& coord1, const sf::Vector2f& coord2)
{
    return sf::Vector2f(float((coord2.x - coord1.x) / euclideanDistance(coord1, coord2)),
                        float(coord2.y - coord1.y) / euclideanDistance(coord1, coord2));
}

float OurMath::calculateAngle(const sf::Vector2f& directionVec)
{
    return float(-std::atan2(directionVec.x, directionVec.y)) * float(180) / float(std::numbers::pi);
}
