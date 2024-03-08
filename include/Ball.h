#pragma once
#include "DynamicObject.h"

class Ball : public DynamicObject
{
public:
    Ball(const int randNum, const sf::Vector2f& position, const sf::Vector2f& directionVector, const float speed = 35.f);
    
    void move(sf::Time delta) override;
    void update(sf::Time delta) override;
    void rotate(const sf::Vector2f& directionVector);
    bool checkCollisionInFront(const Ball& firedBall) const;

    void setDirection(const sf::Vector2f& directionVector);
    void setPosition(const sf::Vector2f position) { m_sprite.setPosition(position); }
    void setSpeed(const float newSpeed) { m_speed = newSpeed; }
    void setScale(const float scale) { m_sprite.setScale(sf::Vector2f(scale, scale)); }
    void setStopped() { m_stopped = true; }
    void setNotStopped() { m_stopped = false; }
    void setNextNode(const int nextNode) { m_nextNode = nextNode; }

    const bool getStopped() const { return m_stopped; }
    const int getColor() const { return m_color; }
    const sf::Vector2f& getOrigin() const { return m_sprite.getOrigin(); }
    const float getRotation() const { return m_sprite.getRotation(); }
    const float getSpeed() const { return m_speed; }
    const sf::Vector2f& getDirection() const { return m_direction; }
    const sf::Vector2f& getBackRectPosition() const { return m_backRect.getPosition(); }
    int getNextNode() const { return m_nextNode; }

private:
    sf::Vector2f m_direction;
    sf::RectangleShape m_frontRect;
    sf::RectangleShape m_backRect;
    bool m_stopped = false;
    float m_speed = 35.f;
    int m_color;
    int m_nextNode = 1;
};