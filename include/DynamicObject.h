#pragma once
#include "BaseObject.h"
#include "Animation.h"

class DynamicObject : public BaseObject
{
public:
	virtual ~DynamicObject() = default;
	virtual void move(sf::Time delta) = 0;
	virtual void update(sf::Time delta) = 0;
	virtual bool checkCollision(DynamicObject& firedBall) const;

protected:
	DynamicObject(const int animationNum, const std::string textureFilename, sf::Vector2f position);
	Animation m_animation;
};
