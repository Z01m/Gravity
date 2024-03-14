#include "Application.h"
#include<SFML/GpuPreference.hpp>

static constexpr float DefaultMapSize = 1e8f;

static SimulationPreset GenerateDefaultPreset()
{
    SimulationPreset simulationPreset;

    simulationPreset.m_entities.reserve(DefaultEntityCount);
    simulationPreset.m_entities.emplace_back(
        Vector2f::CreateZero(),
        Vector2f{ float(rand() % 600), float(rand() % 600) },
        DefaultBigRadius);

    for (size_t i = 1LU; i < DefaultEntityCount * 2; ++i)
    {
        const Vector2f Position =
            Vector2f{ float(rand()),float(rand()) } *float(rand()) * 1.0e2f;

        const Vector2f Velocity = Vector2f{float(rand()%6), float(rand() % 6)};

        const float Radius = 1.0e5f / float((rand() % 10) + 1);
        float den = 3.0e6f / float((rand() % 10) + 1);
        simulationPreset.m_entities.emplace_back(Position, Velocity, Radius, den,sf::Color{uint8_t(rand()%255),uint8_t(rand() % 255) ,uint8_t(rand() % 255) });
    }

    return simulationPreset;
}
static SimulationPreset GenerateFiveBodyPreset()
{
    static constexpr float distanceFromCenter = 2.0e6f;

    SimulationPreset simulationPreset;
    simulationPreset.m_name = "FiveBody";

    Circ entity = { Vector2f::CreateZero(),
                          Vector2f::CreateZero(),
                          DefaultBigRadius };

    const float speed = std::sqrt(
        5.0f * BigG * entity.GetMass() /
        distanceFromCenter / 2.0f);

    simulationPreset.m_entities.emplace_back(
        Vector2f{ distanceFromCenter, 0.0f },
        Vector2f{ 0.0f, -speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Vector2f{ -distanceFromCenter, 0.0f },
        Vector2f{ 0.0f, speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Vector2f{ 0.0f, -distanceFromCenter },
        Vector2f{ -speed, 0.0f },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Vector2f{ 0.0f, distanceFromCenter },
        Vector2f{ speed, 0.0f },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Vector2f{ 0.0f, 0.0f },
        Vector2f{ 0.0f, 0.0f },
        DefaultBigRadius);

    return simulationPreset;
}
static SimulationPreset ManyBodyPreset()
{
    SimulationPreset simulationPreset;
    simulationPreset.m_name = "ManyBody";

    simulationPreset.m_entities.reserve(DefaultEntityCount);
    simulationPreset.m_entities.emplace_back(
        Vector2f::CreateZero(),
        Vector2f::CreateZero(),
        DefaultBigRadius);

    for (size_t i = 1LU; i < 100 ; ++i)
    {
        simulationPreset.m_entities.emplace_back(
            Vector2f{ float(rand()), float(rand()) },
            Vector2f{ 0.0f, 0.0f },
            DefaultBigRadius);
    }
    return simulationPreset;
}


Application& Application::GetApplication()
{
    static Application application;
    return application;
}

Application::Application()
    : m_window{ sf::VideoMode(1920, 1080), "Simulation", sf::Style::Fullscreen }
    , m_simulation{ BoundingBox2D{
          Vector2f{ -DefaultMapSize / 2.0f },
          Vector2f{ DefaultMapSize },
      } }
      , m_currentState{ State::Simulating }
{
    m_window.setFramerateLimit(60);
    Camera& camera = Camera::Get();
    camera.SetView(m_window.getView());
    camera.Zoom(1.0e-3f);
}

bool Application::IsRunning() const
{
    return m_window.isOpen();
}

void Application::HandleMouseScroll(float delta)
{
    Camera& camera = Camera::Get();
    if (delta < 0.0f)
    {
        camera.Zoom(0.8f);
        return;
    }

    camera.Zoom(1.2f);
}

void Application::HandleKeyInputs()
{
    static constexpr float Force = 1e-3f;
    Vector2f input = Vector2f::CreateZero();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        input += Vector2f{ 0.0f, -1.0f };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        input += Vector2f{ 0.0f, 1.0f };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        input += Vector2f{ -1.0f, 0.0f };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        input += Vector2f{ 1.0f, 0.0f };
    }

    Camera& camera = Camera::Get();
    float force = Force * camera.GetView().getSize().x;
    camera.ApplyForce(input.GetNormalized() * force);
}

void Application::HandleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::MouseWheelScrolled:
                HandleMouseScroll(event.mouseWheelScroll.delta);
            break;
        default:
            break;
        }
    }
        HandleKeyInputs();
}

void Application::Start()
{
    m_simulation.LoadPreset(GenerateDefaultPreset()); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    while (IsRunning())
    {
        Update();
        Render();
    }
}

void Application::Update()
{
    HandleEvents();

    if (m_currentState == State::Simulating && !m_isSimulationPaused)
    {
        m_simulation.Update();
    }

    Camera& camera = Camera::Get();
    camera.Update();
    m_window.setView(camera.GetView());
}


void Application::Render()
{
    m_window.clear();

    if (m_currentState == State::Simulating)
    {
        m_simulation.Draw(m_window);
    }
    m_window.display();
}