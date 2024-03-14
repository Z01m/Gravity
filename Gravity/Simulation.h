#pragma once
#pragma once

#include "Circ.h"
#include "Node.h"
#include "QuadTree.h"
#include "BoundingBox2D.h"
#include <vector>
#include"iostream"
#include"SFML/Graphics.hpp"

struct SimulationPreset
{
    static constexpr std::string_view DefaultName = "Default";

    std::string m_name{ DefaultName.data() };
    bool m_isBuiltIn{ true };
    std::vector<Circ> m_entities;
};

class Simulation
{
public:
    explicit Simulation(const BoundingBox2D& mapBounds);

    void Update();
    void Draw(sf::RenderWindow& window) const;
    void LoadPreset(const SimulationPreset& simulationPreset);
   // void OnImGuiUpdate();

private:
    std::string m_currentPresetName;

    BoundingBox2D m_mapBounds;
    QuadTree m_quadTree;
    std::vector<Circ> m_entities;

    void ClearTrackers();

    std::vector<Circ*> GetEntityPointers();
};

