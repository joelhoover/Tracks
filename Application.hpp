#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <Thor/Input/ActionMap.hpp>
#include <Thor/Input/EventSystem.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>

class Application
{
public:
    Application();
    void run();

private:
    enum class Action{
        Close,
        Jump,
        Rocket,
        ToggleFPS,
        ToggleArrow
    };

    using ActionMap = thor::ActionMap<Action>;

    sfg::SFGUI m_sfgui;
    sf::RenderWindow m_window;
    sfg::Desktop m_desktop;
    sf::Image m_icon;
    ActionMap m_actions;
    ActionMap::CallbackSystem m_system;
};
