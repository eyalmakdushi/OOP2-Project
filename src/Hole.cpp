#include "Hole.h"

Hole::Hole(const sf::Texture& texture, sf::Vector2f position)
	:BaseObject(texture,position)
{
	m_sprite.setOrigin(sf::Vector2f(float(m_sprite.getTextureRect().width / 2u), float(m_sprite.getTextureRect().height / 2u)));
}