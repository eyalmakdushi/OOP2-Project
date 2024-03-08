//---- include section ----
#pragma once
#include <memory>
#include "Frog.h"
#include "Road.h"
#include "Ball.h"
#include "Hole.h"
#include "Resources.h"

const int GAME_OVER = 3;
const int FALL_IN_HOLE = 1;
const int WON_LEVEL = 2;

//---- class section ----
class Screen
{
public:
	Screen();
	Screen(const Screen&) = delete;
	void operator = (const Screen&) = delete;

	//----------------- initialization funcs: -----------------
	void openGameWindow();
	void initializeFrog();
	void prepareForLevel();
	void addObjectsToGame(const char object, const sf::Vector2i& pos);
	void createPath();
	void createPathNodes();
	void insertBall(const float speed);

	//----------------- level managing funcs: -----------------
	int runLevel();
	int FallInHole();
	int gameOver();
	int wonLevel();
	void closeGameWindow(); //to close the window of the game after finishing it (losing or winning)

private:
	void displayAll(const AnimationData& data);

	void manageWindow();
	void update();
	void checkBallCollisions();

	void collisionFromBehind(std::vector<std::pair<int, bool>>& firedBallsToRemove, int& firedBall, std::vector<int>& collisionBalls, int& collisionBall);

	void collisionInFront(std::vector<std::pair<int, bool>>& firedBallsToRemove, int& firedBall, std::vector<int>& collisionBalls, int& collisionBall);

	void insertFiredBallToPath(std::vector < std::pair<int, bool>>& firedBallsToRemove, std::vector<int>& collisionBalls);
	std::vector<std::unique_ptr<Ball>>::iterator insertInFront(int ballIndex, std::vector<int>& collisionBalls, const int firedBallIndex);
	std::vector<std::unique_ptr<Ball>>::iterator insertFromBehined(int ballIndex, std::vector<int>& collisionBalls, const int firedBallIndex);

	void countBallsSameColor(const int color, std::vector<std::unique_ptr<Ball>>::iterator& insertIt);
	void countBallsSameColorTowardsEnd(std::vector<int>& indeces, const int color, std::vector<std::unique_ptr<Ball>>::iterator& insertIt);
	void countBallsSameColorTowardsBegin(std::vector<int>& indeces, const int color, std::vector<std::unique_ptr<Ball>>::iterator& insertIt);

	bool concatenation(const int firstNotRemoved);
	bool ballsStopped(const int firstNotRemoved);
	void insertion();

	void removeSameColorBalls(std::vector<int>& indeces, std::vector<std::unique_ptr<Ball>>::iterator& insertIt);
	void collisionWithOtherBalls(int collisionBall, int firedBall);
	void collisionAction(const int firedBall, const int collisionBall);
	void leftButtonPressed(const sf::Time& delta);
	void updatePathBalls(const sf::Time& delta);

	void checkIfLost();

	void checkIfToTurn(const int ball, const int limit, const int nextNode);
	
	void addNewBall();
	void insertNewBall(const float speed);
	void updateFiredBalls(const sf::Time& delta);
	void checkIfBallOutOfWindow(std::unique_ptr<Ball>& ball);
	void draw();
	void drawIndeces(std::string& number, int i);
	void drawCursor();
	void drawText();
	void pollEvent();



	//----------------- data members: -----------------
	std::vector <sf::Vector2f> m_path;
	
	sf::Sprite m_background;
	sf::Sprite m_cursor;

	std::vector <Road> m_road;
	std::unique_ptr <Frog> m_frogPtr;
	Hole m_hole{ Resources::instance().getTexture("hole.png"), sf::Vector2f() };
	std::vector<std::unique_ptr <Ball>> m_ballsInPath;
	std::vector<std::unique_ptr <Ball>> m_firedBallsPtrVec;
	std::vector<std::pair<int, bool>> m_gapIndeces;

	////window related members: 
	sf::RenderWindow m_window;
	bool m_leftMousePressed = false;
	bool m_insertBallFront = false;
	bool m_insertBallBack = false;
	bool m_stopMakingNewBalls = false;
	bool m_firstBallsInserted = false;
	bool m_lost = false;
	bool m_wonLevel = false;

	//time related members: 
	sf::Clock m_clock;
	sf::Time m_elapsed = {};

	int m_firstNotRemoved = 0;
	int m_collsionBall = 0;

	int m_insertIndex = -1;
	int m_levelNumber = 0;
	int m_randBalls = 0;
	int m_currentLevelScore = 0;

	sf::Font m_font = Resources::instance().getFont();
};