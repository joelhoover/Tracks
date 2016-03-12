#include "Application.hpp"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>

Application::Application():
    m_window(sf::VideoMode(800, 600), "Application")
{
    m_window.setFramerateLimit(60);

    m_actions[Action::Close] = thor::Action(sf::Event::Closed);
    m_system.connect(Action::Close, std::bind(&sf::Window::close, &m_window));
}



class Tracks : public sf::Drawable
{
public:
    Tracks()
    {
        m_points.emplace_back(100.f, 100.f);
        m_points.emplace_back(200.f, 200.f);
        m_points.emplace_back(300.f, 300.f);
        m_points.emplace_back(400.f, 200.f);
        m_points.emplace_back(450.f, 120.f);
        m_points.emplace_back(480.f, 200.f);
        m_points.emplace_back(520.f, 350.f);

        m_vertexArray.setPrimitiveType(sf::LinesStrip);
    }

    sf::Vector2f getDirection(float x) const{
        //TODO: Use Niebler range
        for (std::size_t i=0; i<m_points.size()-1; ++i)
            if (x>=m_points[i].x && x<= m_points[i+1].x)
                return thor::unitVector(m_points[i+1] - m_points[i]);
        return {1.f, 0.f};
    }

    bool collidesWith(const sf::FloatRect& rect) const{
        for (std::size_t i=0; i<m_points.size()-1; ++i){
            sf::FloatRect segment(m_points[i].x, m_points[i].y,
                                  m_points[i+1].x-m_points[i].x,
                    m_points[i+1].y-m_points[i].y);

            if (segment.intersects(rect))
                return true;
        }
        return false;
    }


private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override{
        m_vertexArray.clear();
        for (const auto& point : m_points)
            m_vertexArray.append(sf::Vertex(point, sf::Color::Cyan));
        target.draw(m_vertexArray);
    }

    mutable sf::VertexArray m_vertexArray;

    std::vector<sf::Vector2f> m_points;
};

class Wagon : public sf::Drawable
{
public:

    enum class State{
        OnTrack,
        OnAir
    };

    Wagon(const Tracks& track):
        m_track(track),
        m_state(State::OnTrack)
    {
        float width = 50.f;
        float height = 10.f;
        m_shape.setSize({width, height});
        m_shape.setOrigin(width/2.f, height);
        m_shape.setPosition(100.f, 100.f);
        m_shape.setOutlineThickness(1.f);
        m_shape.setOutlineColor(sf::Color::Green);
        m_shape.setFillColor(sf::Color::Transparent);
    }

    void jump(){
        if (m_state == State::OnAir)
            return;

        m_state = State::OnAir;
        m_velocity += sf::Vector2f{0.f, -1.f};
    }

    void update(){
        if (m_velocity.y > 0.f && m_track.collidesWith(m_shape.getGlobalBounds())){
            m_state = State::OnTrack;
        }



        if (m_state == State::OnTrack){
            auto direction = m_track.getDirection(m_shape.getPosition().x);
            static const float speed = 1.f;
            m_velocity = speed * direction;
            m_shape.setRotation(thor::polarAngle(direction));
        }else{
            static const sf::Vector2f gravity(0.f, 0.01f);
            m_velocity += gravity;
        }

        m_shape.move(m_velocity);
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override{
        target.draw(m_shape);
    }

    sf::RectangleShape m_shape;
    const Tracks& m_track;
    sf::Vector2f m_velocity;
    State m_state;
};

void Application::run()
{
    sf::Clock clock;

    Tracks tracks;
    Wagon wagon(tracks);

    m_actions[Action::Jump] = thor::Action(sf::Keyboard::Space, thor::Action::ActionType::PressOnce);
    m_system.connect0(Action::Jump, [&wagon]{
        wagon.jump();
    });




    while (m_window.isOpen()){

        m_actions.clearEvents();

        sf::Event event;
        while (m_window.pollEvent(event)){
            m_actions.pushEvent(event);
            m_desktop.HandleEvent(event);
        }

        m_actions.invokeCallbacks(m_system, &m_window);
        wagon.update();
        m_desktop.Update(clock.restart().asSeconds());

        m_window.clear();
        m_window.draw(wagon);
        m_window.draw(tracks);
        m_sfgui.Display(m_window);
        m_window.display();
    }
}
