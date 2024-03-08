#pragma once
#include "AnimationData.h"

class Animation
{
public:
    Animation(const AnimationData& animationData, sf::Sprite& sprite, const std::string& textureFilename);

    // Add more time to the elapsed time; if enough time passed, it
    // updates the sprite to show the next frame in the animation
    void update(sf::Time delta);

private:
    // Update the sprite to take the correct part of the texture,
    // based on current dir and index
    void initSprite();

    const AnimationData& m_animationData;
    sf::Time m_elapsed = {};
    int m_index = 0;
    sf::Sprite& m_sprite;
    std::string m_animationFilename;
};
