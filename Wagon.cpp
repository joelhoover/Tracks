#include "Wagon.hpp"
#include "Tracks.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Math/Distributions.hpp>
#include <Thor/Animations/FadeAnimation.hpp>
#include <Thor/Shapes/Arrow.hpp>
#include <boost/math/special_functions/sign.hpp>

#include "ResourcePath.hpp"

Wagon::Wagon(const Tracks &track):
    m_track(track),
    m_state(State::OnTrack),
    m_rocketOn(false),
    m_showArrow(false)
{
    float width = 50.f;
    float height = 10.f;

    m_shape.setSize({width, height});
    m_shape.setOrigin(width/2.f, height);
    m_shape.setPosition(m_track.getStart());
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor(sf::Color::Green);
    m_shape.setFillColor(sf::Color::Transparent);

    if (!m_explosion.loadFromFile(resourcePath() + "explosion00.png"))
        throw std::runtime_error("Cannot load texture");



    m_emitter.setParticleLifetime(sf::seconds(1.f));
    float particleScale = 1/30.f;
    m_emitter.setParticleScale(thor::Distributions::deflect({particleScale, particleScale}, 10.f));
    m_emitter.setParticleVelocity(thor::Distributions::deflect(m_velocity, 10.f));
    m_emitter.setEmissionRate(0.f);

    m_particleSystem.setTexture(m_explosion);
    m_particleSystem.addAffector(thor::TorqueAffector(100.f));
    thor::FadeAnimation fader(0.1f, 0.4f);
    m_particleSystem.addAffector(thor::AnimationAffector(fader));
    m_particleSystem.addEmitter(thor::refEmitter(m_emitter));
}

void Wagon::jump(){
    if (m_state == State::OnAir)
        return;

    m_state = State::OnAir;
    auto angle = thor::toRadian(m_shape.getRotation()-90.f);
    auto toAdd = 5.f*sf::Vector2f(std::cos(angle), std::sin(angle));
    m_velocity += toAdd;
}

void Wagon::rocket()
{
    m_rocketOn = true;
    m_rocketTime = 0.f;

    m_emitter.setEmissionRate(30.f);
}

void Wagon::toggleArrow()
{
    m_showArrow = !m_showArrow;
}

const sf::Vector2f &Wagon::getPosition() const{
    return m_shape.getPosition();
}

void Wagon::update(const sf::Time& time){

    //particle system
    m_particleSystem.update(time);

    m_emitter.setParticlePosition(m_shape.getPosition());


    auto currentX = m_shape.getPosition().x;
    auto currentY = m_shape.getPosition().y;
    static const float lowerBuffer(100.f);//, upperBuffer(500.f);

    //wrap around
    if (currentY - lowerBuffer > m_track.bottom()){
        m_shape.setPosition(currentX, m_track.top()-lowerBuffer);
        m_velocity.x += 1.f;
    }
    //    if (currentY + upperBuffer < m_track.top())
    //        m_shape.setPosition(currentX, m_track.bottom()-upperBuffer);

    static float speed = 5.f;

    //rocket
    if (m_rocketOn){
        m_rocketTime += time.asSeconds();
        if (m_rocketTime > 1.f){
            m_rocketOn = false;
            m_emitter.setEmissionRate(0.f);
            speed = 5.f;
        }
        else{
            speed = 10.f;
        }
    }


    auto currentChunk = m_track.chunkAt(currentX);

    if (currentChunk == m_track.end()){
        m_state = State::OnAir;
        applyDragGravity();
    }else{
        if (m_velocity.y > 0.f && m_track.collidesWith(m_shape)){
            m_state = State::OnTrack;
            m_shape.setPosition(m_track.getPointAt(currentChunk, currentX));
        }

        if (m_state == State::OnTrack){
            auto direction = m_track.getDirection(currentChunk, currentX);
            m_velocity = speed * direction;
            m_shape.setRotation(thor::polarAngle(direction));
        }else{
            applyDragGravity();
        }
    }

    m_shape.move(m_velocity);
}

void Wagon::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(m_shape);
    target.draw(m_particleSystem);

    if (m_showArrow){
        auto arrow = thor::Arrow(m_shape.getPosition(), 10.f*(m_velocity),sf::Color(255,21,146));
        target.draw(arrow);
    }
}

void Wagon::applyDragGravity()
{
    static const float gravity = 0.07f;
    auto drag = -boost::math::sign(m_velocity.x)*0.01f;
    m_velocity.x += drag;
    m_velocity.y += gravity;
}
