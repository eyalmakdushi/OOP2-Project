#pragma once
#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <string>
#include "AnimationData.h"
#include <vector>

const int WINDOW_HEIGHT = 1000;
const int WINDOW_WIDTH = 800;
const int NUM_OF_SOUNDS = 12;

class Resources
{
public:
	enum Balls
	{
		BlueBall,
		YellowBall,
		GreenBall,
		RedBall,
		PurpleBall,
		GreyBall,
		Max
	};

	static Resources& instance();
	Resources(const Resources&) = delete;
	void operator = (const Resources&) = delete;

	void playGameMusic();
	void stopGameMusic();
	void playMenuMusic();
	void stopMenuMusic();
	void playSound(const std::string& fileName);

	const sf::Texture& getTexture(const std::string& fileName) const;
	const AnimationData& animationData(const int animationNum) const { return m_animationData[animationNum]; }
	const sf::Font& getFont() const;

private:
	Resources();

	std::vector<AnimationData> m_animationData;

	std::vector <std::string> m_texturesFilenames = { "crosshair copy.png", "sand.jpg", "frog sprite sheet copy.png", "help hovered.png",
													"help normal.png", "hole.png", "menu background1.jpg", "play hovered.png", "play normal.png", 
													"quit hovered.png", "quit normal.png", "road.png","Ball.png","information.jpg",
													"lost.jpg", "win.jpg"};

	std::vector <sf::Texture> m_textures;

	sf::SoundBuffer m_buffers[NUM_OF_SOUNDS];
	std::vector<std::string >m_soundsFilenames = { "ballclick1.ogg", "ballclick2.ogg", "ballsdestroyed1.ogg", "button1.ogg", 
													"chant4.ogg", "death_skull_chomp.ogg", "fireball1.ogg", "warning1.ogg",
													"lose.ogg", "winning.ogg", "hooray.ogg","game_sound.ogg"};
	sf::Sound m_sounds[NUM_OF_SOUNDS];

	sf::Music m_gameMusic;
	sf::Music m_menuMusic;

	sf::Font m_font;
};