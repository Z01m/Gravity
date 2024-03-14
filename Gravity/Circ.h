#pragma once
#include "Entity.h"
#include"Vector2f.h"
#include"constants.h"

class Circ : public Entity
{
public:
    static constexpr float MaxEntityRadius = 1.0e5f;
    static constexpr float DefaultDensity = 3.0e6f;
    static constexpr float DefaultRadius = 1.0f;

    Circ();
    Circ(
        Vector2f position,
        Vector2f velocity,
        float radius,
        float density = DefaultDensity,
        sf::Color color = sf::Color::White);
    ~Circ() = default;

    float GetRadius() const;
    sf::Color GetColor() const;

    Vector2f CalculateGForce(
        float mass, Vector2f position) const;

    Vector2f CalculateGForce(Circ* entity) const;

    void Draw(sf::RenderWindow& window) const override;
    void Update() override;

private:
    sf::CircleShape m_shape;

    void UpdateShapePosition();
};