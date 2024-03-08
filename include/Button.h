#pragma once
#include <SFML/Graphics.hpp>

const int BUTTON_SOUND = 2;

class Button
{
public:
    virtual ~Button() = default;
    Button(const Button&) = delete;
    void operator = (const Button&) = delete;

    void draw(sf::RenderWindow& window);
    void showMenu();
    bool checkLocation (const sf::Vector2f& location) const;
    void setHoveredSprite (const sf::Vector2f& location) ;
    virtual void reactToClick () = 0;

protected:
    Button(const sf::Texture& normalTexture, const sf::Texture& hoveredTexture, sf::RenderWindow& window,
           const float xCoord, const float yCoord);
    void closeWindow();

    //Help funcs
    void showHowToPlay(const sf::Sprite howToPlay);
    bool waitForEvent();

private:
    sf::RenderWindow& m_window;
    sf::Sprite m_normalSprite;
    sf::Sprite m_hoveredSprite;
    sf::Sprite m_drawingSprite;
};