#include "BaseObject.h"

BaseObject::BaseObject(const sf::Texture& texture, sf::Vector2f position)
	: m_sprite(sf::Sprite(texture))
{
	m_sprite.setPosition(position);
}

sf::Sprite BaseObject::getSprite() const
{
	return m_sprite;
}

void BaseObject::draw(sf::RenderWindow& window) const
{
	window.draw(m_sprite);
}

sf::FloatRect BaseObject::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}