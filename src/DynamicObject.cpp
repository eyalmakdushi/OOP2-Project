#include "DynamicObject.h"
#include "Resources.h"


DynamicObject::DynamicObject(const int animationNum, const std::string textureFilename, sf::Vector2f position)
	: BaseObject(Resources::instance().getTexture(textureFilename), position),
	  m_animation(Resources::instance().animationData(animationNum), m_sprite, textureFilename)
{
	m_sprite.setOrigin(sf::Vector2f(float(m_sprite.getTextureRect().width / 2u),
									float(m_sprite.getTextureRect().height / 2u)));
}

bool DynamicObject::checkCollision(DynamicObject& firedBall) const
{
	return firedBall.getGlobalBounds().contains(m_sprite.getPosition());
}