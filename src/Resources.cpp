#include "Resources.h"
#include <algorithm> //for find
#include <iostream> //for cerr

const int NUM_OF_COLS = 10;
const int NUM_OF_ROWS = 6;

namespace
{
    AnimationData ballData(const int ballIndex)
    {
        const auto ballSize = sf::Vector2i(41, 41);
        const auto initSpace = sf::Vector2i(0, ballIndex * 248);

        auto ball = AnimationData{};
        auto currentStart = initSpace;

        int col = 0;
        int row = 0;

        auto nextStart = [&]()
        {
            currentStart.x += ballSize.x;

            if (currentStart.x >= 410)
            {
                currentStart.x = 0;
                currentStart.y += ballSize.y;
            }

            return currentStart;
        };

        while (row != NUM_OF_ROWS)
        {
            if (row == 0 && col == 0)
            {
                ball.m_list.emplace_back(currentStart, ballSize);
                ++col;
            }

            else
            {
                if (col == NUM_OF_COLS - 1)
                {
                    ball.m_list.emplace_back(nextStart(), ballSize);
                    col = 0;
                    ++row;
                }

                else
                {
                    ball.m_list.emplace_back(nextStart(), ballSize);
                    ++col;
                }
            }
        }

        return ball;
    }
}

namespace
{
    AnimationData frogData()
    {
        auto frog = AnimationData{};
        
        const auto frogSize = sf::Vector2i(106, 107);
        const auto frogStart = sf::Vector2i(0, 0);
        frog.m_list.emplace_back(frogStart, frogSize);

        const auto middleEyesSize = sf::Vector2i(57, 18);
        const auto middleEyesStart = sf::Vector2i(108, 0);
        frog.m_list.emplace_back(middleEyesStart, middleEyesSize);

        const auto closedEyesSize = sf::Vector2i(57, 18);
        const auto closedEyesStart = sf::Vector2i(108, 19);
        frog.m_list.emplace_back(middleEyesStart, middleEyesSize);

        const auto tongueSize = sf::Vector2i(28, 53);
        const auto tongueStart = sf::Vector2i(110, 41);
        frog.m_list.emplace_back(tongueStart, tongueSize);

        return frog;
    }
}

Resources::Resources()
{
    for (auto i = 0; i < m_texturesFilenames.size(); ++i)
    {
        m_textures.push_back(sf::Texture());

        if (!m_textures[i].loadFromFile(m_texturesFilenames[i]))
            exit(EXIT_FAILURE);
    }

    for (auto i = 0; i < m_soundsFilenames.size(); ++i)
    {
        if (!m_buffers[i].loadFromFile(m_soundsFilenames[i]))
            exit(EXIT_FAILURE);

        m_sounds[i].setBuffer(m_buffers[i]);
    }

    if (!m_font.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
        exit(EXIT_FAILURE);

    if (!m_gameMusic.openFromFile("game_sound.ogg"))
        exit(EXIT_FAILURE);
    m_gameMusic.setLoop(true);

    if (!m_menuMusic.openFromFile("menu_sound.ogg"))
        exit(EXIT_FAILURE);
    m_menuMusic.setLoop(true);

    for (int i = 0; i < Max; ++i)
        m_animationData.emplace_back(ballData(i));

    m_animationData.emplace_back(frogData());
}

Resources& Resources::instance()
{
	static auto resources = Resources();
	return resources;
}

const sf::Texture& Resources::getTexture(const std::string& fileName) const
{
    auto it = std::find(m_texturesFilenames.begin(), m_texturesFilenames.end(), fileName);

    if (it == m_texturesFilenames.end())
    {
        std::cerr << "the texture " << fileName << " couldn't be found\n";
        exit(EXIT_FAILURE);
    }

    else
    {
        int textureIndex = int(std::distance(m_texturesFilenames.begin(), it));
        return m_textures[textureIndex];
    }
}

const sf::Font& Resources::getFont() const
{
	return m_font;
}

void Resources::playGameMusic()
{
	m_gameMusic.play();
}

void Resources::stopGameMusic()
{
	m_gameMusic.stop();
}

void Resources::playMenuMusic()
{
	m_menuMusic.play();
}

void Resources::stopMenuMusic()
{
	m_menuMusic.stop();
}

void Resources::playSound(const std::string& fileName)
{
    auto it = std::find(m_soundsFilenames.begin(), m_soundsFilenames.end(), fileName);

    if (it == m_soundsFilenames.end())
    {
        std::cerr << "the sound " << fileName << " couldn't be found\n";
        exit(EXIT_FAILURE);
    }

    else
    {
        int soundIndex = int(std::distance(m_soundsFilenames.begin(), it));
        m_sounds[soundIndex].play();
    }
}