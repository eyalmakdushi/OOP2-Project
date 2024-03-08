//---- include section ----
#pragma once
#include "Ball.h"
class Screen;

//---- class section ----
class Frog : public DynamicObject
{
public:
	Frog(sf::Vector2f position, Screen& screen, sf::RenderWindow& window);

	// --------------------- set funcs: ---------------------
	void setPosition(const sf::Vector2f position);
	void draw();

	// --------------------- get funcs: ---------------------
	const sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
	int getLives() const { return m_lives; }
	int getScore() const { return m_score; }
	int getMouthBallColor() const { return m_mouthBallPtr->getColor(); }

	// --------------------- update funcs: ---------------------
	//void prepareForNextLevel();
	void move(sf::Time delta) override;
	void moveMouth();
	void moveNextBallPosition();
	void rotateFrog();
	void updateMousePos();
	void update(sf::Time delta) override;
	void changeLivesLeft(const int newLife) { m_lives += newLife; }
	void changeScore(const int newScore) { m_score += newScore; }
	void resetBalls() { m_mouthBallPtr.release(); m_nextBallPtr.release(); }
	void createNextMouthBall(const int randBalls);
	std::unique_ptr <Ball> shootBall(const sf::Time& delta);
	void swapBalls();

private:
	Screen* m_screenPtr = nullptr;
	sf::RenderWindow& m_window;
	sf::Vector2f m_mouseCurrPos;
	sf::Vector2f m_mouthPos;
	sf::Vector2f m_nextBallPos;
	unsigned int m_score = 0;
	unsigned int m_lives = 3;

	std::unique_ptr <Ball> m_mouthBallPtr;
	std::unique_ptr <Ball> m_nextBallPtr;
};