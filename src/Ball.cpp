#include "Ball.h"
#include "Resources.h"
#include "OurMath.h"

Ball::Ball(const int randNum, const sf::Vector2f& position, const sf::Vector2f& directionVector, const float speed)
    : DynamicObject(randNum, "Ball.png", position), m_direction(directionVector), m_color(randNum), m_speed(speed)
{
    float angle = OurMath::calculateAngle(directionVector);
    m_sprite.setRotation(angle);

    auto size = sf::Vector2f(float(m_sprite.getTextureRect().width), float(m_sprite.getTextureRect().height) / 2.f);
    m_frontRect.setSize(size);
    m_backRect.setSize(size);

    auto newPos = m_sprite.getPosition();
    newPos.x += directionVector.x * m_sprite.getTextureRect().width * .75f;
    newPos.y += directionVector.y * m_sprite.getTextureRect().height * .75f;

    m_frontRect.setPosition(newPos);
    m_frontRect.setFillColor(sf::Color(255, 0, 0, 70));
    m_frontRect.setOrigin(m_frontRect.getSize() / 2.f);
    
    newPos.x -= directionVector.x * m_sprite.getTextureRect().width * .75f;
    newPos.y -= directionVector.y * m_sprite.getTextureRect().height * .75f;

    m_backRect.setPosition(newPos);
    m_backRect.setFillColor(sf::Color(255, 0, 255, 125));
    m_backRect.setOrigin(m_backRect.getSize() / 2.f);
}

void Ball::setDirection(const sf::Vector2f& directionVector)
{
    m_direction = directionVector;
}

void Ball::update(sf::Time delta)
{
    m_animation.update(delta);
}

void Ball::move(sf::Time delta)
{
    m_sprite.move(m_direction * delta.asSeconds() * m_speed);

    auto newPos = m_sprite.getPosition();
    newPos.x += m_direction.x * m_sprite.getTextureRect().width * .5f;
    newPos.y += m_direction.y * m_sprite.getTextureRect().height * .5f;
    m_frontRect.setPosition(newPos);

    newPos.x -= m_direction.x * m_sprite.getTextureRect().width * .75f;
    newPos.y -= m_direction.y * m_sprite.getTextureRect().height * .75f;
    m_backRect.setPosition(newPos);
}

void Ball::rotate(const sf::Vector2f& directionVector)
{
    float angle = OurMath::calculateAngle(directionVector);

    m_sprite.setRotation(angle);
    m_frontRect.setRotation(angle);
    m_backRect.setRotation(angle);

    m_direction = directionVector;
}

bool Ball::checkCollisionInFront(const Ball& firedBall) const
{
    return m_frontRect.getGlobalBounds().contains(firedBall.getPosition());
}