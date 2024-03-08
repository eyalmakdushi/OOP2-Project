#pragma once
//---- include section ----
#include <fstream>
#include <SFML\Audio.hpp>
#include <string>

#include "Screen.h"

using std::ifstream;

//---- const section ----
const int LOSE = -1;

//---- class section ----
class Controller
{
public:
	Controller() = default;
	Controller(const Controller&) = delete;
	void operator = (const Controller&) = delete;

	void startGame();
	void startInitializiation();

private:
	void prepareLevel();
	void repeatLevel();
	void readTxtFile();

	void addObjectsToGame();

	//window funcs:
	void showLosingWindow();
	void showWinningWindow();
	void waitForEvents(sf::RenderWindow& window) const;

	Screen m_screen;

	std::ifstream m_file = std::ifstream("Path.txt");
	std::string m_levelLines;
};