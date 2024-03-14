#pragma once

#include "Camera.h"
#include "Simulation.h"

class Application
{
public:
    static Application& GetApplication();

    void Start();

    Application(const Application& other) = delete;
    Application(Application&& other) = delete;

private:
    enum class State
    {
        Simulating,
        EditingPreset
    };
    bool m_isSimulationPaused{ false };

    sf::Clock m_deltaClock;
    sf::RenderWindow m_window;

    Simulation m_simulation;

    State m_currentState{ State::Simulating };

    Application();
    ~Application() = default;

    bool IsRunning() const;

    void HandleEvents();
    void HandleMouseScroll(float delta);
    void HandleKeyInputs();

    void Update();
    void OnSFMLUpdate();
    void Render();
};