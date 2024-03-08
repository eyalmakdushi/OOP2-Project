#include "Frog.h"
#include "Screen.h"
#include <memory>
#include "OurMath.h"
#include "Resources.h"

Frog::Frog(sf::Vector2f position, Screen& screen, sf::RenderWindow& window)
	: DynamicObject(6, "frog sprite sheet copy.png", position), m_screenPtr(&screen), m_window(window)
{}

// --------------------- set funcs: ---------------------
void Frog::setPosition(const sf::Vector2f position)
{
	m_sprite.setPosition(position);
	m_sprite.setRotation(0.f);
	m_mouthPos = m_sprite.getPosition();
	m_mouthPos.y += 30;

	m_nextBallPos = m_sprite.getPosition();
	m_nextBallPos.y -= 26;
}

void Frog::draw()
{
	m_window.draw(m_sprite);
	m_mouthBallPtr->draw(m_window);
	m_nextBallPtr->draw(m_window);
}

void Frog::move(sf::Time)
{
	updateMousePos();
	rotateFrog();
	moveMouth();
	moveNextBallPosition();
}

void Frog::moveMouth()
{
	auto mouseFrogDistance = OurMath::euclideanDistance(m_mouseCurrPos, m_sprite.getPosition());

	auto mouthFrogDistance = OurMath::euclideanDistance(m_mouthPos, m_sprite.getPosition());

	auto ratio = mouthFrogDistance / mouseFrogDistance;

	m_mouthPos.x = float((1 - ratio) * m_sprite.getPosition().x + ratio * m_mouseCurrPos.x);
	m_mouthPos.y = float((1 - ratio) * m_sprite.getPosition().y + ratio * m_mouseCurrPos.y);

	m_mouthBallPtr->setPosition(m_mouthPos);
	m_mouthBallPtr->rotate(OurMath::directionalVector(m_sprite.getPosition(), m_mouthPos));
}

void Frog::moveNextBallPosition()
{
	auto mouseFrogDistance = OurMath::euclideanDistance(m_mouseCurrPos, m_sprite.getPosition());

	auto nextFrogDistance = -OurMath::euclideanDistance(m_nextBallPos, m_sprite.getPosition());

	auto ratio = nextFrogDistance / mouseFrogDistance;

	m_nextBallPos.x = float((1 - ratio) * m_sprite.getPosition().x + ratio * m_mouseCurrPos.x);
	m_nextBallPos.y = float((1 - ratio) * m_sprite.getPosition().y + ratio * m_mouseCurrPos.y);

	m_nextBallPtr->setPosition(m_nextBallPos);
	m_nextBallPtr->rotate(-OurMath::directionalVector(m_sprite.getPosition(), m_nextBallPos));
}

void Frog::rotateFrog()
{
	float mouseAngleInDegrees = OurMath::calculateAngle(OurMath::directionalVector(m_sprite.getPosition(), m_mouseCurrPos));
	m_sprite.setRotation(mouseAngleInDegrees);
}

void Frog::updateMousePos()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
	m_mouseCurrPos = m_window.mapPixelToCoords(pixelPos);
}

void Frog::update(sf::Time delta)
{
	m_animation.update(delta);
}

void Frog::createNextMouthBall(const int randBalls)
{
	if (m_mouthBallPtr == nullptr && m_nextBallPtr == nullptr) //create mouth ball for the first time
	{
		m_mouthBallPtr = std::make_unique<Ball>(rand() % randBalls, m_mouthPos, sf::Vector2f(0, 1), 600.f);
		m_mouthBallPtr->setScale(.75f);
		m_mouthBallPtr->rotate(OurMath::directionalVector(m_sprite.getPosition(), m_mouthPos));
	}


	else //when the mouth ball is shot, move the next to the mouth and create new next
	{
		m_mouthBallPtr = std::move(m_nextBallPtr);
		m_mouthBallPtr->setScale(.75f);
		m_mouthBallPtr->rotate(OurMath::directionalVector(m_sprite.getPosition(), m_mouthPos));
		m_mouthBallPtr->setPosition(m_mouthPos);
	}

	//create the next ball any way
	m_nextBallPtr = std::make_unique<Ball>(rand() % randBalls, m_nextBallPos, sf::Vector2f(0, 1), 600.f);
	m_nextBallPtr->setScale(.5f);
	m_nextBallPtr->rotate(OurMath::directionalVector(m_sprite.getPosition(), m_mouthPos));
}

std::unique_ptr <Ball> Frog::shootBall(const sf::Time& /*delta*/)
{
	Resources::instance().playSound("fireball1.ogg");
	//get mouse position
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
	auto mouseCurrPos = m_window.mapPixelToCoords(pixelPos);

	//calculate direction vector
	auto direction = OurMath::directionalVector(m_mouthPos, mouseCurrPos);

	m_mouthBallPtr->rotate(direction);
	m_mouthBallPtr->setScale(1);

	//update(delta);

	return std::move(m_mouthBallPtr);
}

void Frog::swapBalls()
{
	m_mouthBallPtr.swap(m_nextBallPtr);
	m_mouthBallPtr->setScale(.75f);
	m_nextBallPtr->setScale(.5f);

	auto tempPos = m_nextBallPtr->getPosition();
	m_nextBallPtr->setPosition(m_mouthBallPtr->getPosition());
	m_mouthBallPtr->setPosition(tempPos);
}
