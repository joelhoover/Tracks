#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "TrackChunk.hpp"

class Tracks : public sf::Drawable
{
public:
    using Chunks = std::vector<TrackChunk>;
    using ChunkIterator = Chunks::const_iterator;

    Tracks();
    sf::Vector2f getDirection(ChunkIterator chunk, float x) const;
    sf::Vector2f getPointAt(ChunkIterator chunk, float x) const;
    void update(float left, float right);
    bool collidesWith(const sf::Shape& shape) const;
    sf::Vector2f getStart() const;
    ChunkIterator chunkAt(float x) const;
    ChunkIterator end() const;
    float top() const;
    float bottom() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    float distancePossible() const;
    void addNewChunk();

    Chunks m_chunks;
    sf::Vector2f m_right;
};
