#include "Tracks.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/min_element.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include "Window.hpp"
#include <Thor/Math/Random.hpp>

Tracks::Tracks(){
    m_chunks.push_back(TrackChunk(0.f, screenWidth, screenHeight/2.f));
    m_right = m_chunks.back().right();
}

void Tracks::addNewChunk()
{
    auto newChunkRight = thor::random(m_right.x+screenWidth/2.f, m_right.x + screenWidth);
    m_chunks.push_back(TrackChunk(m_right.x,
                                  newChunkRight,
                                  m_right.y));
    m_right = m_chunks.back().right();
}

void Tracks::update(float left, float right){
    boost::remove_erase_if(m_chunks, [left](const auto& chunk){
        return chunk.right().x < left;
    });

    if (m_chunks.empty())
        addNewChunk();

    while (right > m_right.x){
        auto haveSpace = thor::random(0,1);
        if (haveSpace)
            m_right.x += distancePossible();
        else
            addNewChunk();
    }
}

void Tracks::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    for (const auto& chunk : m_chunks)
        target.draw(chunk);
}

Tracks::ChunkIterator Tracks::chunkAt(float x) const {
    return boost::find_if(m_chunks, [x](const auto& chunk){
        return chunk.left()<=x && x<=chunk.right().x;
    });
}

Tracks::ChunkIterator Tracks::end() const
{
    return m_chunks.cend();
}

float Tracks::top() const
{
    return boost::min_element(m_chunks, [](const auto& lhs, const auto& rhs){
        return lhs.top() < rhs.top();
    })->top();
}

float Tracks::bottom() const
{
    return boost::max_element(m_chunks, [](const auto& lhs, const auto& rhs){
        return lhs.bottom() < rhs.bottom();
    })->bottom();
}

float Tracks::distancePossible() const
{
    //TODO: calculate this analytically
    return thor::random(50, 500);
}

sf::Vector2f Tracks::getDirection(ChunkIterator chunk, float x) const{
    return chunk->getDirection(x);
}

sf::Vector2f Tracks::getPointAt(ChunkIterator chunk, float x) const{
    return chunk->getPointAt(x);
}

bool Tracks::collidesWith(const sf::Shape &shape) const{
    return boost::algorithm::any_of(m_chunks, [&shape](const auto& chunk){
        return chunk.collidesWith(shape);
    });
}

sf::Vector2f Tracks::getStart() const{
    return m_chunks.front().getStart();
}
