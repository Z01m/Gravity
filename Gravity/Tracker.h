#pragma once

#include <SFML/Graphics.hpp>
#include "Vector2f.h"
#include <iostream>
#include"smth.h"

class Tracker
{
public:
    Tracker() = default;
    ~Tracker() = default;

    void AddVertex(Vector2f position);
    void Clear();
    void Draw(sf::RenderWindow& window) const;

    static void ToggleLimit();
    static void ToggleDraw();

private:
    static constexpr size_t MaxLength = 256UL;
    static bool ShouldLimit, ShouldDraw;
    std::vector<sf::Vertex> m_vertices;
};