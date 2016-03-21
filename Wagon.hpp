#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/Emitters.hpp>
#include <SFML/Graphics/Texture.hpp>

class Tracks;

class Wagon : public sf::Drawable
{
public:
    enum class State{
        OnTrack,
        OnAir
    };

    Wagon(const Tracks& track);
    void jump();
    void rocket();
    void toggleArrow();
    const sf::Vector2f& getPosition() const;
    void update(const sf::Time &time);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void applyDragGravity();

    sf::RectangleShape m_shape;
    const Tracks& m_track;
    sf::Vector2f m_velocity;
    State m_state;
    sf::Texture m_explosion;

    thor::ParticleSystem m_particleSystem;
    thor::UniversalEmitter m_emitter;

    float m_rocketTime;
    bool m_rocketOn;

    bool m_showArrow;
};
