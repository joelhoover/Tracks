#include "Score.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

static std::string toTwoDigits(int s){
    std::string toReturn;
    if (s<10)
        toReturn += "0";
    toReturn += std::to_string(s);
    return toReturn;
}

static std::string timeToString(float milliseconds){
    int minutes = milliseconds/60000;
    int seconds = (static_cast<int>(std::ceil(milliseconds))/1000)%60;
    int centiseconds = (static_cast<int>(std::ceil(milliseconds))/10)%100;

    std::string totalTime = toTwoDigits(minutes) + ":" +
            toTwoDigits(seconds) + ":" +
            toTwoDigits(centiseconds);

    return totalTime;
}

Score::Score():
    m_milliseconds(0.f),
    m_bestTime(std::nanf(""))
{
    m_font.loadFromFile("KBZipaDeeDooDah.ttf");
    m_text.setFont(m_font);

    std::ifstream bestTime("BestTime");
    if (bestTime.good() && bestTime >> m_bestTime)
        m_bestTimeString = "Best: " + timeToString(m_bestTime);
}

Score::~Score()
{
    try {
        if (!std::isnan(m_bestTime)){
            std::ofstream bestTime("BestTime");
            bestTime << m_bestTime;
        }
    }catch(const std::exception& e){
        std::cerr << "Failed to record highscore: " << e.what() << '\n';
    }
}

void Score::update(const sf::Time &time)
{
    m_milliseconds += time.asMilliseconds();
    auto toDisplay = "Current: " + timeToString(m_milliseconds) + "\n" + m_bestTimeString;
    m_text.setString(toDisplay);
}

void Score::newSegment()
{
    if (std::isnan(m_bestTime) || m_milliseconds < m_bestTime){
        m_bestTime = m_milliseconds;
        m_bestTimeString = "Best: " + timeToString(m_bestTime);
    }

    m_milliseconds = 0.f;
}

void Score::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text);
}
