#include "Menu.h"
#include "Resources.h"
#include "Play.h"
#include "Help.h"
#include "Exit.h"

const float BUTTON_HEIGHT = 125;
const float DISTANCE_BETWEEN_BUTTONS = 22;
const float BUTTON_X_COORD = 620;
const float PLAY_Y_COORD = 95;
const float HELP_Y_COORD = PLAY_Y_COORD + DISTANCE_BETWEEN_BUTTONS + BUTTON_HEIGHT;
const float EXIT_Y_COORD = HELP_Y_COORD + DISTANCE_BETWEEN_BUTTONS + BUTTON_HEIGHT;

Menu::Menu()
{
	m_menuBackground = sf::Sprite(Resources::instance().getTexture("menu background1.jpg"));

	m_buttons.push_back(std::make_unique <Play>(Resources::instance().getTexture("play normal.png"),
												Resources::instance().getTexture("play hovered.png"), m_window, BUTTON_X_COORD, PLAY_Y_COORD));

	m_buttons.push_back(std::make_unique <Help>(Resources::instance().getTexture("help normal.png"),
												Resources::instance().getTexture("help hovered.png"), m_window, BUTTON_X_COORD,
												HELP_Y_COORD, Resources::instance().getTexture("information.jpg")));

	m_buttons.push_back(std::make_unique <Exit>(Resources::instance().getTexture("quit normal.png"),
												Resources::instance().getTexture("quit hovered.png"), m_window, BUTTON_X_COORD, EXIT_Y_COORD));
}

void Menu::displayMenu()
{
	Resources::instance().playMenuMusic();
	m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Zuma-Menu");

	while (m_window.isOpen())
	{
		m_window.clear(sf::Color::White);
		draw();
		m_window.display();
		waitForEvents();
	}
}

void Menu::draw()
{
	m_window.draw(m_menuBackground);

	for (int i = 0; i < m_buttons.size(); ++i)
		m_buttons[i]->draw(m_window);
}

void Menu::waitForEvents()
{
	if (auto event = sf::Event{}; m_window.waitEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;

		case sf::Event::MouseButtonReleased:
			switch (event.mouseButton.button)
			{

			case sf::Mouse::Button::Left:
				leftButton(event);
				break;
			}

			break;

		case sf::Event::MouseMoved:
			mouseMoved();
			break;

		case sf::Event::KeyPressed:
			keyPressed(event);
			break;

		}
	}
}

void Menu::leftButton(sf::Event& event)
{
	auto location = m_window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

	for (int i = 0; i < NUMBER_OF_BUTTONS; ++i)
		if (m_buttons[i]->checkLocation(location))
		{
			Resources::instance().playSound("button1.ogg");
			m_buttons[i]->reactToClick();
		}
}

void Menu::mouseMoved()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
	sf::Vector2f location = m_window.mapPixelToCoords(pixelPos);

	for (int i = 0; i < NUMBER_OF_BUTTONS; ++i)
		m_buttons[i]->setHoveredSprite(location);
}

void Menu::keyPressed(sf::Event& event)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Enter:
		Resources::instance().playSound("button1.ogg");
		m_buttons[0]->reactToClick(); //play
		break;

	case sf::Keyboard::Escape:
		Resources::instance().playSound("button1.ogg");
		m_window.close();
		break;
	}
}
