#include "Road.h"
#include "OurMath.h"

Road::Road(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& directionVector)
	:BaseObject(texture,position)
{
	m_sprite.setOrigin(sf::Vector2f(m_sprite.getTexture()->getSize() / 2u));
	float angle = OurMath::calculateAngle(directionVector);
	m_sprite.setRotation(angle);
}