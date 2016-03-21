#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <deque>

using Points = std::vector<sf::Vector2f>;
using PointIterator = Points::const_iterator;

namespace sf { class Shape; }

class TrackChunk : public sf::Drawable {
public:
    TrackChunk(float left, float right, float initialHeight);
    sf::Vector2f getDirection(float x) const;
    sf::Vector2f getPointAt(float x) const;
    bool collidesWith(const sf::Shape& shape) const;
    sf::Vector2f getStart() const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;
    float rightHeight() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    PointIterator pointBefore(float x) const;

    mutable sf::VertexArray m_vertexArray;
    Points m_points;    
    float m_initialHeight;
};
