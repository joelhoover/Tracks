#include "Application.hpp"
#include <SFML/Graphics.hpp>
#include <SFGUI/Widgets.hpp>
#include "FrameCounter.hpp"
#include "Tracks.hpp"
#include "Wagon.hpp"
#include "Window.hpp"
#include "Score.hpp"

Application::Application():
    m_window(sf::VideoMode(screenWidth, screenHeight), "Tracks")
{
    m_window.setFramerateLimit(60);

    m_actions[Action::Close] = thor::Action(sf::Event::Closed);
    m_system.connect(Action::Close, std::bind(&sf::Window::close, &m_window));
}

void Application::run()
{
    sf::Clock clock;

    Score score;
    Tracks tracks;
    Wagon wagon(tracks);
    auto view = m_window.getDefaultView();

    m_actions[Action::Jump] = thor::Action(sf::Keyboard::Space, thor::Action::ActionType::PressOnce);
    m_system.connect0(Action::Jump, [&wagon]{
        wagon.jump();
    });

    m_actions[Action::Rocket] = thor::Action(sf::Keyboard::F, thor::Action::ActionType::PressOnce);
    m_system.connect0(Action::Rocket, [&wagon]{
        wagon.rocket();
    });

    auto frameCounter = FrameCounter::Create();
    m_desktop.Add(frameCounter);
    frameCounter->SetPosition({screenWidth-50.f, 0.f});
    frameCounter->Show(false);

    m_actions[Action::ToggleFPS] = thor::Action(sf::Keyboard::Tilde, thor::Action::ActionType::PressOnce);
    m_system.connect0(Action::ToggleFPS, [&frameCounter]{
        frameCounter->Show(!frameCounter->IsLocallyVisible());
    });

    m_actions[Action::ToggleArrow] = thor::Action(sf::Keyboard::V, thor::Action::ActionType::PressOnce);
    m_system.connect0(Action::ToggleArrow, [&wagon]{
        wagon.toggleArrow();
    });

    auto progressBar = sfg::ProgressBar::Create();
    auto progressBarWidth = screenWidth/6.f;
    progressBar->SetPosition({screenWidth/2.f - progressBarWidth/2.f, 0.f});
    progressBar->SetRequisition({progressBarWidth, 4*screenHeight/100.f});
    m_desktop.Add(progressBar);

    float left(-screenWidth), right(screenWidth);
    float previousX = wagon.getPosition().x;

    float segmentDistance = 0.f;
    static const float segmentLength = 10000.f;


    while (m_window.isOpen()){

        m_actions.clearEvents();

        sf::Event event;
        while (m_window.pollEvent(event)){
            m_actions.pushEvent(event);
            m_desktop.HandleEvent(event);
        }

        auto frameTime = clock.restart();

        m_actions.invokeCallbacks(m_system, &m_window);

        auto movedBy = wagon.getPosition().x - previousX;

        previousX = wagon.getPosition().x;
        left += movedBy;
        right += movedBy;
        tracks.update(left, right);

        segmentDistance += movedBy;
        if (segmentDistance > segmentLength){
            score.newSegment();
            segmentDistance -= segmentLength;
            progressBar->SetFraction(0.f);
        }
        progressBar->SetFraction(segmentDistance/segmentLength);

        wagon.update(frameTime);
        score.update(frameTime);
        view.setCenter(wagon.getPosition());
        m_desktop.Update(frameTime.asSeconds());

        m_window.clear();
        m_window.setView(view);
        m_window.draw(wagon);
        m_window.draw(tracks);
        m_window.setView(m_window.getDefaultView());
        m_window.draw(score);
        m_sfgui.Display(m_window);
        m_window.display();
    }
}
