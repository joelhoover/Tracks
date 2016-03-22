#include "TrackChunk.hpp"
#include "Point.hpp"
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/min_element.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <Thor/Vectors.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Thor/Math/Random.hpp>
#include "Window.hpp"

BOOST_GEOMETRY_REGISTER_LINESTRING(Points)

namespace ChunkFunctions {

static const float scaleFactor = 100.f;

float sin(float x){
    return 100.f*std::sin(x/scaleFactor);
}


struct poly {
    poly(){
        m_roots.emplace_back(-1.f/scaleFactor);
        m_roots.emplace_back(2.f/scaleFactor);
        m_roots.emplace_back(5.f/scaleFactor);
    }

    float operator()(float x){
        x /= scaleFactor;

        float toReturn = 1.f;
        for (auto root : m_roots)
            toReturn *= (x-root);
        return toReturn;
    }

    float firstRoot() const {
        return m_roots.front();
    }

private:
    std::vector<float> m_roots;
};

}

namespace bg = boost::geometry;

TrackChunk::TrackChunk(float left, float right, float initialHeight):
    m_initialHeight(initialHeight)
{
    static const std::array<std::function<float(float)>, 2> functions{{
            ChunkFunctions::sin, ChunkFunctions::poly()
                                                                      }};

    auto i = thor::random(0u, functions.size()-1);
    auto f = functions[i];

    for (float x = 0.f; x<right-left; ++x){
        auto poly = f.target<ChunkFunctions::poly>();
        float xx;
        if (poly)
            xx = x + poly->firstRoot();
        else
            xx = x;
        m_points.emplace_back(x+left, f(xx)+initialHeight);
    }

    m_vertexArray.setPrimitiveType(sf::LinesStrip);
}

sf::Vector2f TrackChunk::getDirection(float x) const{    
    auto p1 = pointBefore(x);
    assert(p1 != m_points.cend());
    return thor::unitVector(*std::next(p1) - *p1);
}

sf::Vector2f TrackChunk::getPointAt(float x) const{
    auto p1 = pointBefore(x);
    assert(p1 != m_points.cend());
    return (*std::next(p1) + *p1)/2.f;
}

bool TrackChunk::collidesWith(const sf::Shape &shape) const{
    bg::model::ring<sf::Vector2f> ring;
    for (std::size_t i=0; i<shape.getPointCount(); ++i)
        bg::append(ring, shape.getTransform().transformPoint(shape.getPoint(i)));
    return bg::intersects(ring, m_points);
}

sf::Vector2f TrackChunk::getStart() const{
    return m_points.front();
}

float TrackChunk::left() const{
    return m_points.front().x;
}

const sf::Vector2f &TrackChunk::right() const{
    return m_points.back();
}

float TrackChunk::top() const
{
    return boost::min_element(m_points, [](const auto& lhs, const auto& rhs){
       return lhs.y < rhs.y;
    })->y;
}

float TrackChunk::bottom() const
{
    return boost::max_element(m_points, [](const auto& lhs, const auto& rhs){
       return lhs.y < rhs.y;
    })->y;
}

void TrackChunk::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    m_vertexArray.clear();
    for (const auto& point : m_points)
        m_vertexArray.append(sf::Vertex(point, sf::Color::Cyan));
    target.draw(m_vertexArray);
}

PointIterator TrackChunk::pointBefore(float x) const
{
    return boost::adjacent_find(m_points, [x](const auto& p1, const auto& p2){
       return p1.x<=x && x<=p2.x;
    });
}
