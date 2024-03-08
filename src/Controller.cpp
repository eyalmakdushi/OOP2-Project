#include "Controller.h"
#include <sstream>

void Controller::startGame()
{
	startInitializiation();

	int levelResult = 0; //the game hasn't been played yet

	while (!m_file.eof())
	{
		if (levelResult == GAME_OVER) //check if levelResult = GAME_OVER to finish the game
			return showLosingWindow();

		prepareLevel();
		levelResult = m_screen.runLevel();

		while (levelResult == FALL_IN_HOLE)
		{
			repeatLevel();
			levelResult = m_screen.runLevel();
		}
	}

	if (levelResult == GAME_OVER) //check if player lost at the last level, in order to not show winning window
		showLosingWindow();

	else
		showWinningWindow();
}

void Controller::startInitializiation()
{
	m_screen.openGameWindow();
	m_screen.initializeFrog();
}

void Controller::prepareLevel()
{
	m_screen.prepareForLevel();
	readTxtFile();
	addObjectsToGame();
	m_screen.createPathNodes();
	m_screen.createPath();
	m_screen.insertBall(500.f); //insert the first ball
}

void Controller::readTxtFile()
{
	m_levelLines.clear();

	auto line = std::string();

	if (!std::getline(m_file, m_levelLines))
		exit(EXIT_FAILURE);

	m_file.get(); //read \n from file after two numbers
}

void Controller::repeatLevel()
{
	m_screen.prepareForLevel();
	addObjectsToGame();
}

void Controller::addObjectsToGame()
{
	std::istringstream stream(m_levelLines);

	int frogX, frogY;
	stream >> frogX >> frogY;
	m_screen.addObjectsToGame('F', sf::Vector2i(frogX, frogY));

	int holeX, holeY;
	stream >> holeX >> holeY;
	m_screen.addObjectsToGame('H', sf::Vector2i(holeX, holeY));

	int roadX, roadY;
	while (stream >> roadX)
	{
		stream >> roadY;
		m_screen.addObjectsToGame('R', sf::Vector2i(roadX, roadY));
	}
}

void Controller::showLosingWindow()
{
	m_screen.closeGameWindow();
	Resources::instance().stopGameMusic();
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Zuma-Lose");
	sf::Sprite losingSprite = sf::Sprite(Resources::instance().getTexture("lost.jpg"));
	Resources::instance().playSound("lose.ogg");

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		window.draw(losingSprite);
		window.display();
		waitForEvents(window);
	}
}

void Controller::showWinningWindow()
{
	m_screen.closeGameWindow();
	Resources::instance().stopGameMusic();
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(833, 747), "Zuma-Win");
	sf::Sprite winningSprite = sf::Sprite(Resources::instance().getTexture("win.jpg"));
	Resources::instance().playSound("winning.ogg");

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		window.draw(winningSprite);
		window.display();
		waitForEvents(window);
	}
}

void Controller::waitForEvents(sf::RenderWindow& window) const
{
	if (auto event = sf::Event{}; window.waitEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;

			default:
				break;
			}
		}
	}
}