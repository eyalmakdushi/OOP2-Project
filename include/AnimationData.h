#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct AnimationData
{
    using ListType = std::vector<sf::IntRect>;
    ListType m_list;
};
