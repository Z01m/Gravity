#include "Simulation.h"
#include "smth.h"

Simulation::Simulation(const BoundingBox2D& mapBounds)
    : m_mapBounds{ mapBounds }
    , m_quadTree{ m_mapBounds }
{
}


void Simulation::Update()
{
    for (auto& entity : m_entities)
    {
        entity.Update();
        if (!m_mapBounds.IsPointWithinBounds(entity.GetPosition()))
        {
            Vector2f newPosition =
                entity.GetPosition() - m_mapBounds.GetMinPoint();
            newPosition = newPosition.GetMod(m_mapBounds.GetDimensions());
            newPosition += m_mapBounds.GetMinPoint();
            entity.SetPosition(newPosition);
        }
    }

    m_quadTree.Update();

    for (auto& entity : m_entities)
    {
        entity.ClearAcceleration();
        m_quadTree.ApplyGForcesToEntity(&entity);
    }
}

void Simulation::LoadPreset(const SimulationPreset& simulationPreset)
{
    m_currentPresetName = simulationPreset.m_name;
    m_entities = simulationPreset.m_entities;
    m_quadTree.Clear();
    m_quadTree.Build(GetEntityPointers());
}

void Simulation::Draw(sf::RenderWindow& window) const
{
    for (const auto& entity : m_entities)
    {
        entity.Draw(window);
    }

    m_quadTree.Draw(window);
}


void Simulation::ClearTrackers()
{
    for (auto& entity : m_entities)
    {
        entity.ClearTracker();
    }
}

std::vector<Circ*> Simulation::GetEntityPointers()
{
    std::vector<Circ*> entityPointers;
    entityPointers.reserve(m_entities.size());

    for (auto& entity : m_entities)
    {
        entityPointers.push_back(&entity);
    }

    return entityPointers;
}