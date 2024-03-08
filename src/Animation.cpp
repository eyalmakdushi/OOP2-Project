#include "Animation.h"
#include "Resources.h"


const auto AnimationTime = sf::seconds(0.03f);

Animation::Animation(const AnimationData& animationData, sf::Sprite& sprite, const std::string& textureFilename)
    : m_animationData(animationData), m_sprite(sprite), m_animationFilename(textureFilename)
{
    m_sprite.setTexture(Resources::instance().getTexture(textureFilename));
    initSprite();
}

void Animation::update(sf::Time delta)
{
    m_elapsed += delta;
    if (m_elapsed >= AnimationTime)
    {
        m_elapsed -= AnimationTime;
        ++m_index;
        m_index %= m_animationData.m_list.size();
        initSprite();
    }
}

void Animation::initSprite()
{
    m_sprite.setTextureRect(m_animationData.m_list[m_index]);
}
