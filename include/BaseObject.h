#pragma once
#include <SFML/Graphics.hpp>

class BaseObject
{
public:
	BaseObject(const BaseObject&) = delete;
	void operator = (const BaseObject&) = delete;
	BaseObject(BaseObject&&) = default;
	
	void draw(sf::RenderWindow& window) const;

	sf::FloatRect getGlobalBounds() const;
	sf::Sprite getSprite() const;
	const sf::Vector2f& getPosition() const { return m_sprite.getPosition(); }

	void setPosition(const sf::Vector2f position) { m_sprite.setPosition(position); }

protected:
	BaseObject(const sf::Texture& texture, sf::Vector2f position);
	sf::Sprite m_sprite;
};