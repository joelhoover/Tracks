#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Thor/Time.hpp>

class Score : public sf::Drawable
{
public:
    Score();

    ~Score();

    void update(const sf::Time& time);
    void newSegment();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Font m_font;
    sf::Text m_text;
    float m_milliseconds;
    float m_bestTime;
    std::string m_bestTimeString;
};
