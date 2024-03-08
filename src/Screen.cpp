#include "Screen.h"
#include "OurMath.h"
#include "Resources.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>

const float BALL_EDGE_SIZE = 41.f;
const sf::Vector2f BALL_VEC_SIZE = sf::Vector2f(41.f, 41.f);

Screen::Screen()
{}

void Screen::displayAll(const AnimationData& data)
{
    const auto size = [](sf::IntRect rect)
    {
        return sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
    };

    auto currentPosition = sf::Vector2f();

    int col = 0;

    for (const auto& ball : data.m_list)
    {
        auto sprite = sf::Sprite(Resources::instance().getTexture("Ball.png"), ball);
        sprite.setPosition(currentPosition);
        m_window.draw(sprite);

        currentPosition.x += size(ball).x * 2.f;
        ++col;

        if (col == 10)
        {
            col = 0;
            currentPosition.x = 0;
            currentPosition.y += size(ball).y * 2.f;
        }
    }
}

void Screen::openGameWindow()
{
    m_window.setFramerateLimit(60);
    //m_window.create(sf::VideoMode::getDesktopMode(), "Zuma-Game", sf::Style::Close);
    m_window.create(sf::VideoMode(800, 900), "Zuma-Game", sf::Style::Close);
    //m_window.setMouseCursorVisible(false); // Hide cursor

    sf::View fixed = m_window.getView(); // Create a fixed view

    m_cursor.setTexture(Resources::instance().getTexture("crosshair copy.png"));

    m_cursor.setOrigin(sf::Vector2f(float(m_cursor.getTextureRect().width / 2u),
        float(m_cursor.getTextureRect().height / 2u)));

    auto modes = sf::VideoMode::getFullscreenModes();
    //m_window.create(sf::VideoMode::getDesktopMode(), "Zuma-Game", sf::Style::Close);

    /*const sf::Vector2u originalSize { 800u, 900u };
    sf::View worldview(m_window.getDefaultView());
    worldview.setCenter(sf::Vector2f)*/
    m_background.setTexture(Resources::instance().getTexture("sand.jpg"));
}

void Screen::initializeFrog()
{
    m_frogPtr = std::make_unique<Frog>(sf::Vector2f(), *this, m_window);
}

void Screen::prepareForLevel()
{
    m_path.clear(); //delete contents of previous vector
    m_road.clear(); //delete contents of previous vector
    m_gapIndeces.clear(); //delete contents of previous vector
    m_firedBallsPtrVec.clear(); //delete contents of previous vector

    m_currentLevelScore = 0;
    m_insertIndex = -1;
    m_firstNotRemoved = 0;
    m_collsionBall = 0;

    m_leftMousePressed = false;
    m_insertBallFront = false;
    m_insertBallBack = false;
    m_stopMakingNewBalls = false;
    m_firstBallsInserted = false;
    m_lost = false;
    m_wonLevel = false;

    ++m_levelNumber;

    if (m_levelNumber >= 1 && m_levelNumber <= 3)
        m_randBalls = 3;
    
    else if (m_levelNumber >= 4 && m_levelNumber <= 6)
        m_randBalls = 4;
    
    else if (m_levelNumber >= 7 && m_levelNumber <= 9)
        m_randBalls = 5;
    
    else if (m_levelNumber == 10)
        m_randBalls = 6;

    m_frogPtr->resetBalls();
}

void Screen::addObjectsToGame(const char object, const sf::Vector2i& pos)
{
    switch (object)
    {
    case ('F'):
        m_frogPtr->setPosition(sf::Vector2f(pos));
        return;

    case('H'):
        m_hole.setPosition(sf::Vector2f(pos));
        return;

    case('R'):
        m_path.push_back(sf::Vector2f(pos));
        return;
    }
}

int Screen::runLevel()
{
    m_frogPtr->createNextMouthBall(m_randBalls);

    m_clock.restart(); // reset the time from program start

    while (m_window.isOpen())
    {
        manageWindow();

        if (m_wonLevel && m_ballsInPath.size() == 0)
            return wonLevel();

        if(m_frogPtr->getLives() == 0)
            return gameOver();

        if (m_lost && m_ballsInPath.size() == 0)
            return FallInHole();
    }

    return 42; //never reached, to avoid Warning C4715 not all control paths return a value
}

void Screen::createPathNodes()
{
    for (int i = 0; i < m_path.size() - 1; ++i)
    {
        int j = i + 1;
        auto directionVector = OurMath::directionalVector(m_path[i], m_path[j]);
        m_road.emplace_back(Resources::instance().getTexture("road.png"), m_path[i], directionVector);
    }

    auto directionVector = OurMath::directionalVector(m_path[m_path.size() - 1], m_hole.getPosition());
    m_road.emplace_back(Resources::instance().getTexture("road.png"), m_path[m_path.size() - 1], directionVector);
}

void Screen::createPath()
{
    auto roadTexture = Resources::instance().getTexture("road.png");

    for (int i = 0; i < m_path.size() - 1; ++i)
    {
        int j = i + 1;
        float distance = OurMath::euclideanDistance(m_path[i], m_path[j]);
        auto directionVector = OurMath::directionalVector(m_path[i], m_path[j]);
        auto numOfRoadsToInsert = int(distance / roadTexture.getSize().x);

        for (int node = 0; node < numOfRoadsToInsert; ++node)
        {
            sf::Vector2f nodePos;
            nodePos.x = float(m_path[i].x + 40 * (node + 1) * directionVector.x);
            nodePos.y = float(m_path[i].y + 40 * (node + 1) * directionVector.y);
            m_road.emplace_back(Resources::instance().getTexture("road.png"), nodePos, directionVector);
        }
    }
}

void Screen::manageWindow()
{
    int sidp = 0;

    if (sidp == 1)
    {
        m_window.clear();
        displayAll(Resources::instance().animationData(1));
        m_window.display();
        pollEvent();
    }

    else
    {
        m_window.clear(sf::Color::Black);
        draw();
        m_window.display();
        pollEvent();
        update();
    }
}

void Screen::draw()
{
    m_window.draw(m_background);
    m_frogPtr->draw();

    for (const auto& road : m_road)
        m_window.draw(road.getSprite());

    m_hole.draw(m_window);
    
    std::string number;

    for (auto& ball : m_firedBallsPtrVec)
        ball->draw(m_window);

    for (int i = 0; i < m_ballsInPath.size(); ++i)
    {
        m_ballsInPath[i]->draw(m_window);

        //drawIndeces(number, i);
    }

    drawText();

    drawCursor();
}

void Screen::drawIndeces(std::string& number, int i)
{
    number = std::to_string(i);
    auto result = sf::Text(number, m_font, 30u);
    result.setFillColor(sf::Color::White);
    result.setOutlineColor(sf::Color::Black);
    result.setOutlineThickness(5);
    result.setPosition(m_ballsInPath[i]->getPosition());
    result.setOrigin(result.getLocalBounds().width / 2, result.getLocalBounds().height / 2);
    m_window.draw(result);
}

void Screen::drawCursor()
{
    m_cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));

    switch (m_frogPtr->getMouthBallColor())
    {
    case 0:     m_cursor.setColor(sf::Color(32, 118, 255));     break;
    case 1:     m_cursor.setColor(sf::Color(241, 233, 4));      break;
    case 2:     m_cursor.setColor(sf::Color(87, 211, 25));      break;
    case 3:     m_cursor.setColor(sf::Color::Red);              break;
    case 4:     m_cursor.setColor(sf::Color(128, 45, 189));     break;
    case 5:     m_cursor.setColor(sf::Color::White);            break;
    }

    m_window.draw(m_cursor);
}

void Screen::drawText()
{
    sf::View view = m_window.getView();
    sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

    auto string = "Current level: " + std::to_string(m_levelNumber) +
        "\t Lives left: " + std::to_string(m_frogPtr->getLives()) +
        "\t Score: " + std::to_string(m_frogPtr->getScore());

    auto result = sf::Text(string, m_font, 30u);
    result.setFillColor(sf::Color::Black);
    result.setOutlineColor(sf::Color::White);
    result.setOutlineThickness(3);
    result.setPosition(float(viewRect.width) / 2, float(viewRect.height - 50));
    result.setOrigin(result.getLocalBounds().width / 2, result.getLocalBounds().height / 2);
    m_window.draw(result);
}

void Screen::update()
{
    if (m_stopMakingNewBalls == false && m_wonLevel == false)
        addNewBall();

    const auto delta = m_clock.restart();
    updateFiredBalls(delta);

    updatePathBalls(delta);
    leftButtonPressed(delta);
    checkBallCollisions();
}

void Screen::addNewBall()
{
    if (!m_firstBallsInserted)
    {
        if (m_ballsInPath.size() == 2) //to deal with 0 ball being ahead of 1 ball, place 0 right after 1
        {
            sf::Vector2f offset = m_ballsInPath[0]->getDirection() * BALL_EDGE_SIZE;
            m_ballsInPath[0]->setPosition(m_ballsInPath[1]->getPosition() + offset);
        }

        if (m_ballsInPath.size() < 20) //while the 20 first balls haven't been inserted, keep inserting fast balls
            insertNewBall(500.f);

        else //the 20 first balls have been inserted, slow all the balls
        {
            m_firstBallsInserted = true;
            for (auto& ball : m_ballsInPath)
                ball->setSpeed(35.f);
        }
    }

    else //all the balls have been cleared, but the level isn't won/lost yet
    {
        if (m_ballsInPath.empty()) //insert the first ball
            insertBall(35.f);

        else 
            insertNewBall(35.f);
    }
}

void Screen::insertBall(const float speed)
{
    m_ballsInPath.emplace_back(std::make_unique<Ball>(rand() % m_randBalls, m_path[0], OurMath::directionalVector(m_path[0], m_path[1]), speed));
}

void Screen::insertNewBall(const float speed)
{
    if (OurMath::euclideanDistance(m_path[0], m_ballsInPath.back()->getPosition()) >= BALL_EDGE_SIZE)
        insertBall(speed);
}

void Screen::checkBallCollisions()
{
    std::vector<std::pair<int, bool>> firedBallsToRemove;
    std::vector<int> collisionBalls;

    for (int firedBall = 0; firedBall < m_firedBallsPtrVec.size(); ++firedBall)
    {
        for (int ball = 0; ball < m_ballsInPath.size(); ++ball)
        {
            if (firedBall < m_firedBallsPtrVec.size())
            {
                if (m_ballsInPath[ball]->checkCollisionInFront(*m_firedBallsPtrVec[firedBall]))
                {
                    collisionInFront(firedBallsToRemove, firedBall, collisionBalls, ball);
                    ++firedBall; //to avoid checking same firedBall with another ball, both in front and otherwise
                }

                else if (m_ballsInPath[ball]->checkCollision(*m_firedBallsPtrVec[firedBall]))
                {
                    collisionFromBehind(firedBallsToRemove, firedBall, collisionBalls, ball);
                    ++firedBall; //to avoid checking same firedBall with another ball, both in front and otherwise
                }
            }
        }
    }

    if (!firedBallsToRemove.empty())
        insertFiredBallToPath(firedBallsToRemove, collisionBalls);

    std::erase_if(m_firedBallsPtrVec, [](auto& ballPointer) {return ballPointer == nullptr; });
}

void Screen::collisionInFront(std::vector<std::pair<int, bool>>& firedBallsToRemove, int& firedBall, std::vector<int>& collisionBalls, int& collisionBall)
{
    firedBallsToRemove.push_back(std::pair(firedBall, true));
    collisionBalls.push_back(collisionBall);

    if (collisionBall == 0) //collision in front with first ball
    {
        collisionAction(firedBall, 0);
        sf::Vector2f offset = m_firedBallsPtrVec[firedBall]->getDirection() * BALL_EDGE_SIZE;
        m_firedBallsPtrVec[firedBall]->setPosition(m_ballsInPath[collisionBall]->getPosition() + offset);
    }

    else //collision in front with other ball
    {
        collisionAction(firedBall, collisionBall);
        m_insertBallFront = true;
    }
}

void Screen::collisionFromBehind(std::vector<std::pair<int, bool>>& firedBallsToRemove, int& firedBall, std::vector<int>& collisionBalls, int& collisionBall)
{
    firedBallsToRemove.push_back(std::pair(firedBall, false));
    collisionBalls.push_back(collisionBall);

    if (collisionBall == m_ballsInPath.size() - 1) //collision from behind with last ball
    {
        collisionAction(firedBall, int(m_ballsInPath.size() - 1));
        sf::Vector2f offset = m_firedBallsPtrVec[firedBall]->getDirection() * BALL_EDGE_SIZE;
        m_firedBallsPtrVec[firedBall]->setPosition(m_ballsInPath[collisionBall]->getPosition() - offset);
    }

    else //collision from behind with other ball
    {
        collisionAction(firedBall, collisionBall);
        m_insertBallBack = true;
    }
}

void Screen::insertFiredBallToPath(std::vector<std::pair<int, bool>>& firedBallsToRemove, std::vector<int>& collisionBalls)
{
    auto insertIt = m_ballsInPath.begin(); //initialize with something, so auto will know what to deduce

    for (int ballIndex = 0; ballIndex < firedBallsToRemove.size(); ++ballIndex)
    {
        auto firedBallIndex = firedBallsToRemove[ballIndex].first;
        int color = m_firedBallsPtrVec[firedBallIndex]->getColor();

        if (firedBallsToRemove[ballIndex].second == true)
            insertIt = insertInFront(ballIndex, collisionBalls, firedBallIndex);

        else
            insertIt = insertFromBehined(ballIndex, collisionBalls, firedBallIndex);

        m_insertIndex = int(std::distance(m_ballsInPath.begin(), insertIt));

        if (insertIt != m_ballsInPath.begin() && insertIt != m_ballsInPath.end() - 1)
        {
            for (auto beforeIt = insertIt - 1; ; --beforeIt)
            {
                (*beforeIt)->setSpeed(500.f);

                if (beforeIt == m_ballsInPath.begin())
                    break;
            }
        }

        countBallsSameColor(color, insertIt);
    }
}

std::vector<std::unique_ptr<Ball>>::iterator Screen::insertInFront(int ballIndex, std::vector<int>& collisionBalls, const int firedBallIndex)
{
    auto insertIt = m_ballsInPath.begin() + collisionBalls[ballIndex];
    insertIt = m_ballsInPath.insert(insertIt, std::move(m_firedBallsPtrVec[firedBallIndex]));
    return insertIt;
}

std::vector<std::unique_ptr<Ball>>::iterator Screen::insertFromBehined(int ballIndex, std::vector<int>& collisionBalls, const int firedBallIndex)
{
    auto insertIt = m_ballsInPath.begin() + collisionBalls[ballIndex] + 1;
    insertIt = m_ballsInPath.insert(insertIt, std::move(m_firedBallsPtrVec[firedBallIndex]));
    return insertIt;
}

void Screen::countBallsSameColor(const int color, std::vector<std::unique_ptr<Ball>>::iterator& insertIt)
{
    std::vector<int> indeces; //indeces of balls with the same color in the current section

    if (insertIt == m_ballsInPath.begin())
        countBallsSameColorTowardsEnd(indeces, color, insertIt);

    else if (insertIt == m_ballsInPath.end())
        countBallsSameColorTowardsBegin(indeces, color, insertIt);

    else
    {
        countBallsSameColorTowardsEnd(indeces, color, insertIt);
        countBallsSameColorTowardsBegin(indeces, color, insertIt);
    }

    if (indeces.size() >= 2) //can remove same color balls from the vector
    {
        removeSameColorBalls(indeces, insertIt);
        
        m_currentLevelScore += int(indeces.size() * 10);
        m_frogPtr->changeScore(int(indeces.size() * 10));

        if (m_frogPtr->getScore() >= 50 && m_wonLevel == false)
        {
            m_wonLevel = true;
            Resources::instance().playSound("chant4.ogg");
        }
            
    }

    else
    {
        for (auto& gap : m_gapIndeces)
        {
            if (gap.first > m_insertIndex)
                ++gap.first;

            if (gap.first + 1 == m_insertIndex)
                ++gap.first;
        }
    }
}

void Screen::countBallsSameColorTowardsEnd(std::vector<int>& indeces, const int color, std::vector<std::unique_ptr<Ball>>::iterator& insertIt)
{
    for (auto afterIt = insertIt + 1; afterIt != m_ballsInPath.end(); ++afterIt)
    {
        if ((*afterIt)->getColor() == color)
            indeces.push_back(int(std::distance(m_ballsInPath.begin(), afterIt)));

        else
            break;
    }
}

void Screen::countBallsSameColorTowardsBegin(std::vector<int>& indeces, const int color, std::vector<std::unique_ptr<Ball>>::iterator& insertIt)
{
    for (auto beforeIt = insertIt - 1; ; --beforeIt)
    {
        if ((*beforeIt)->getColor() == color)
            indeces.push_back(int(std::distance(m_ballsInPath.begin(), beforeIt)));

        else
            break;

        if (beforeIt == m_ballsInPath.begin())
            break;
    }
}

void Screen::removeSameColorBalls(std::vector<int>& indeces, std::vector<std::unique_ptr<Ball>>::iterator& insertIt)
{
    indeces.push_back(int(std::distance(m_ballsInPath.begin(), insertIt)));
    std::sort(indeces.begin(), indeces.end(), std::greater<int>());
    
    for (int index = 0; index < indeces.size(); ++index)
        m_ballsInPath.erase(m_ballsInPath.begin() + indeces[index]);

    if (m_ballsInPath.size() <= 1)
    {
        m_gapIndeces.clear();
        return;
    }

    if (indeces.back() != 0)
        m_firstNotRemoved = indeces.back() - 1;

    std::pair<int, bool> tempPair{m_firstNotRemoved, false};
    auto it = std::find(m_gapIndeces.begin(), m_gapIndeces.end(), tempPair);

    if (it != m_gapIndeces.end())
        m_gapIndeces.erase(it);

    if (m_insertIndex >= m_ballsInPath.size())
    {
        for (int i = m_firstNotRemoved; i > -1; --i)
        {
            if (m_ballsInPath[i]->getStopped() == false)
                m_ballsInPath[i]->setSpeed(35.f);

            else
                m_ballsInPath[i]->setSpeed(0.f);
        }
        m_insertIndex = -1;
        return;
    }

    int afterRemoved = m_firstNotRemoved + 1;

    if (afterRemoved >= m_ballsInPath.size())
        return;

    if (indeces.back() != 0)
    {
        for (auto& gap : m_gapIndeces)
        {
            if (gap.first > m_firstNotRemoved)
                gap.first -= int(indeces.size() - 1);

            if (std::find(m_gapIndeces.begin(), m_gapIndeces.end(), std::pair(m_firstNotRemoved, false)) == m_gapIndeces.end())
                m_gapIndeces.push_back(std::make_pair(m_firstNotRemoved, false));

            std::sort(m_gapIndeces.begin(), m_gapIndeces.end());
        }

        if (m_gapIndeces.empty())
            m_gapIndeces.push_back(std::make_pair(m_firstNotRemoved, false));

        for (int i = m_firstNotRemoved; i > -1; --i)
        {
            m_ballsInPath[i]->setSpeed(0.f);
            m_ballsInPath[i]->setStopped();
        }
    }

    else //stop first ball, indeces.back() = 0
    {
        for (auto& gap : m_gapIndeces)
        {
            if (gap.first > indeces.back())
                if (gap.first - (int(indeces.size()) - 1) >= 0)
                    gap.first -= (int(indeces.size()) - 1);

            if (std::find(m_gapIndeces.begin(), m_gapIndeces.end(), std::pair(m_firstNotRemoved,false)) == m_gapIndeces.end())
                m_gapIndeces.push_back(std::make_pair(m_firstNotRemoved, false));
            std::sort(m_gapIndeces.begin(), m_gapIndeces.end());
        }

        if (m_gapIndeces.empty())
            m_gapIndeces.push_back(std::make_pair(m_firstNotRemoved, false));

        for (int i = m_firstNotRemoved; i > -1; --i)
        {
            m_ballsInPath[i]->setSpeed(0.f);
            m_ballsInPath[i]->setStopped();
        }
    }

    m_insertIndex = -1;

    Resources::instance().playSound("ballsdestroyed1.ogg");
}

void Screen::collisionWithOtherBalls(int collisionBall, int firedBall)
{
    m_collsionBall = collisionBall;
    int afterCollision = collisionBall + 1;

    collisionAction(firedBall, afterCollision);
}

void Screen::collisionAction(const int firedBall, const int collisionBall)
{
    m_firedBallsPtrVec[firedBall]->rotate(m_ballsInPath[collisionBall]->getDirection());
    m_firedBallsPtrVec[firedBall]->setSpeed(35.f);
    
    if (m_ballsInPath[collisionBall]->getStopped())
    {
        m_firedBallsPtrVec[firedBall]->setStopped();
        m_firedBallsPtrVec[firedBall]->setSpeed(0.f);
    }

    m_firedBallsPtrVec[firedBall]->setNextNode(m_ballsInPath[collisionBall]->getNextNode());
}

void Screen::leftButtonPressed(const sf::Time& delta)
{
    if (m_leftMousePressed)
    {
        m_firedBallsPtrVec.push_back(m_frogPtr->shootBall(delta));
        m_frogPtr->createNextMouthBall(m_randBalls);
        m_leftMousePressed = false;
    }
}

void Screen::updatePathBalls(const sf::Time& delta)
{
    if (!m_gapIndeces.empty())
    {
        for (int index = 0; index < m_gapIndeces.size(); ++index)
            if (m_gapIndeces[index].second == false) //balls stopped
                if (ballsStopped(m_gapIndeces[index].first))
                    m_gapIndeces[index].first = -1;

        std::erase_if(m_gapIndeces, [](auto& pair) {return pair.first == -1; });
    }

    for (int ball = 0; ball < int(m_ballsInPath.size()); ++ball)
    {
        if (m_ballsInPath[ball]->getSpeed() != 0)
        {
            m_ballsInPath[ball]->update(delta);
            m_ballsInPath[ball]->move(delta);
        }

        checkIfToTurn(ball, int(m_path.size() - 1), 1);
    }

    insertion();

    if (!m_ballsInPath.empty())
        checkIfLost();
}

void Screen::checkIfLost()
{
    if (OurMath::euclideanDistance(m_ballsInPath[0]->getPosition(), m_hole.getPosition()) <= 20)
    {
        if (!m_stopMakingNewBalls)
        {
            m_stopMakingNewBalls = true;
            m_frogPtr->changeLivesLeft(-1);
            m_lost = true;

            for (int ballIndex = 0; ballIndex < m_ballsInPath.size(); ++ballIndex)
                m_ballsInPath[ballIndex]->setSpeed(500.f);
        }

        m_ballsInPath.erase(m_ballsInPath.begin()); //remove the first ball if it arrives at the hole
    }
}

void Screen::checkIfToTurn(const int ball, const int limit, const int nextNode)
{
    auto a = m_path[m_ballsInPath[ball]->getNextNode()];

    if (OurMath::euclideanDistance(m_ballsInPath[ball]->getBackRectPosition(), 
                                   m_path[m_ballsInPath[ball]->getNextNode()]) <= 15)
    {
        if (m_ballsInPath[ball]->getNextNode() != limit)
        {
            int newNode = m_ballsInPath[ball]->getNextNode() + nextNode;
            auto newDirection = OurMath::directionalVector(m_path[m_ballsInPath[ball]->getNextNode()], m_path[newNode]);

            m_ballsInPath[ball]->rotate(newDirection);
            m_ballsInPath[ball]->setNextNode(newNode);

            if(m_insertIndex!=-1)
            {
                if (ball == m_insertIndex - 1)
                {
                    m_ballsInPath[m_insertIndex]->rotate(newDirection);
                    m_ballsInPath[m_insertIndex]->setNextNode(newNode);
                }
            }
        }
    }
}

bool Screen::ballsStopped(const int firstNotRemoved)
{
    int afterMoved = firstNotRemoved + 1;

    if (afterMoved < m_ballsInPath.size())
    {
        auto a = OurMath::euclideanDistance(m_ballsInPath[firstNotRemoved]->getPosition(), m_ballsInPath[afterMoved]->getPosition());

        if (a <= BALL_EDGE_SIZE)
        {
            for (int i = firstNotRemoved; i > -1; --i)
            {
                if (i != 0)
                {
                    int j = i - 1;

                    auto d = OurMath::euclideanDistance(m_ballsInPath[i]->getPosition(), m_ballsInPath[j]->getPosition());

                    if (d < BALL_EDGE_SIZE * 1.5)
                    {
                        m_ballsInPath[i]->setSpeed(35.f);
                        m_ballsInPath[i]->setNotStopped();
                    }

                    else
                    {
                        m_ballsInPath[i]->setSpeed(35.f);
                        m_ballsInPath[i]->setNotStopped();
                        break;
                    }
                }

                else
                {
                    m_ballsInPath[i]->setSpeed(35.f);
                    m_ballsInPath[i]->setNotStopped();
                }
            }

            return true;
        }

        else
            return false;
    }

    else
        return false;
}

void Screen::insertion()
{
    if (m_insertIndex < m_ballsInPath.size() - 1 && m_insertIndex > 0)
    {
        auto beforeIndex = m_insertIndex - 1;
        auto afterIndex = m_insertIndex + 1;

        if (OurMath::euclideanDistance(m_ballsInPath[beforeIndex]->getPosition(), m_ballsInPath[afterIndex]->getPosition()) >= BALL_EDGE_SIZE * 2)
        {
            auto offset = m_ballsInPath[m_insertIndex]->getDirection() * BALL_EDGE_SIZE;

            if (m_insertBallFront)
            {
                m_ballsInPath[m_insertIndex]->setPosition(m_ballsInPath[afterIndex]->getPosition() + offset);
                m_insertBallFront = false;
            }

            else if (m_insertBallBack)
            {
                m_ballsInPath[m_insertIndex]->setPosition(m_ballsInPath[beforeIndex]->getPosition() - offset);
                m_insertBallBack = false;
            }

            for (auto& ball : m_ballsInPath)
            {
                if (ball->getStopped() == false)
                    ball->setSpeed(35.f);

                else
                    ball->setSpeed(0.f);
            }

            m_insertIndex = -1;
        }
    }
}

void Screen::updateFiredBalls(const sf::Time& delta)
{
    for (auto& ball : m_firedBallsPtrVec)
    {
        ball->move(delta);
        checkIfBallOutOfWindow(ball);
    }

    std::erase_if(m_firedBallsPtrVec, [](auto& ball) {return ball == nullptr; });
}

void Screen::checkIfBallOutOfWindow(std::unique_ptr<Ball>& ball)
{
    sf::View view = m_window.getView();
    sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

    sf::Vector2f ballPosition = ball->getPosition();

    if (!viewRect.contains(ballPosition))
        ball.release();
}

void Screen::pollEvent()
{
    for (auto event = sf::Event{}; m_window.pollEvent(event);)
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_window.close();
            //Resources::instance().stopGameMusic();
            exit(EXIT_SUCCESS);
            break;

        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {

            case sf::Mouse::Button::Left:
                if (m_firstBallsInserted && !m_stopMakingNewBalls)
                    m_leftMousePressed = true;
                break;

            case sf::Mouse::Button::Right:
                m_frogPtr -> swapBalls();
                break;
            }

            break;

        case sf::Event::MouseMoved:
            m_frogPtr->move(sf::Time());
            break;
        }
    }
}

int Screen::wonLevel()
{
    Resources::instance().playSound("hooray.ogg");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return WON_LEVEL;
}

int Screen::gameOver()
{
    m_window.close();
    Resources::instance().stopGameMusic();
    return GAME_OVER;
}

int Screen::FallInHole()
{
    Resources::instance().playSound("death_skull_chomp.ogg");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_frogPtr->changeScore(-m_currentLevelScore);
    return FALL_IN_HOLE;
}

void Screen::closeGameWindow()
{
    m_window.close();
}