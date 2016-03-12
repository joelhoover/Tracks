#include "Application.hpp"
#include <SFML/Graphics.hpp>

Application::Application():
    m_window(sf::VideoMode(800, 600), "Application")
{
    m_window.setFramerateLimit(60);

    m_actions[Action::Close] = thor::Action(sf::Event::Closed);
    m_system.connect(Action::Close, std::bind(&sf::Window::close, &m_window));
}

class Wagon : public sf::Drawable
{
public:
    Wagon()
    {
        m_shape.setSize({50.f, 10.f});
        m_shape.setOutlineThickness(1.f);
        m_shape.setOutlineColor(sf::Color::Green);
        m_shape.setFillColor(sf::Color::Transparent);
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override{
        target.draw(m_shape);
    }

    sf::RectangleShape m_shape;
};

class Tracks : public sf::Drawable
{
public:
    Tracks()
    {
        m_vertexArray.setPrimitiveType(sf::LinesStrip);
        m_vertexArray.append(sf::Vertex({100,100}, sf::Color::Cyan));
        m_vertexArray.append(sf::Vertex({200,200}, sf::Color::Cyan));
        m_vertexArray.append(sf::Vertex({300,300}, sf::Color::Cyan));
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override{
        target.draw(m_vertexArray);
    }

    sf::VertexArray m_vertexArray;
};

void Application::run()
{
    sf::Clock clock;

    Wagon wagon;
    Tracks tracks;

    while (m_window.isOpen()){

        m_actions.clearEvents();

        sf::Event event;
        while (m_window.pollEvent(event)){
            m_actions.pushEvent(event);
            m_desktop.HandleEvent(event);
        }

        m_actions.invokeCallbacks(m_system, &m_window);
        m_desktop.Update(clock.restart().asSeconds());

        m_window.clear();
        m_window.draw(wagon);
        m_window.draw(tracks);
        m_sfgui.Display(m_window);
        m_window.display();
    }
}
