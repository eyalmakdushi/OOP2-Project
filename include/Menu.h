#pragma once
#include <vector>
#include <memory>
#include "Button.h"

//---- const section ----
const int NUMBER_OF_BUTTONS = 3;

class Menu
{
public:
	Menu();
	Menu(const Menu&) = delete;
	void operator = (const Menu&) = delete;
	void displayMenu();

private:
	void draw();
	void waitForEvents();
	void keyPressed(sf::Event& event);
	void mouseMoved();
	void leftButton(sf::Event& event);

	sf::RenderWindow m_window;
	sf::Sprite m_menuBackground;
	std::vector <std::unique_ptr <Button>> m_buttons;
};