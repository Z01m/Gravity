#include "Circ.h"
#include <cmath>
#include"smth.h"
#include <cmath>

Circ::Circ()
    : Circ(
         Vector2f::CreateZero(),
         Vector2f::CreateZero(),
        DefaultRadius)
{
}

Circ::Circ(Vector2f position,Vector2f velocity,float radius,float density,sf::Color color) : Entity(position,velocity,(4.0f / 3.0f)*  PI * radius* radius* radius*density)
{
    radius = std::min(MaxEntityRadius, radius);
    radius = std::max(0.0f, radius);

    m_shape.setPosition(VecToSFVec(
        position -  Vector2f{ radius }));
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_font = m_font;
}

float Circ::GetRadius() const
{
    return m_shape.getRadius();
}

sf::Color Circ::GetColor() const
{
    return m_shape.getFillColor();
}

void Circ::Draw(sf::RenderWindow& window) const
{
    m_tracker.Draw(window);

    window.draw(m_shape);

    if (!Entity::ShouldDrawVectors)
    {
        return;
    }

    DrawVector(window, m_position, m_acceleration, sf::Color::Red);
    DrawVector(window, m_position, m_velocity);
}

void Circ::Update()
{
    Entity::Update();
    this->UpdateShapePosition();
}

void Circ::UpdateShapePosition()
{
    const  Vector2f shapePosition = m_position -  Vector2f{ m_shape.getRadius() };
    m_shape.setPosition(VecToSFVec(shapePosition));
}

 Vector2f Circ::CalculateGForce(float mass,  Vector2f position) const
{
    const  Vector2f displacementVector = position - m_position;
    const  Vector2f normalVector = displacementVector.GetNormalized();

    const float distanceSquared = displacementVector.GetLengthSquared();

    if (distanceSquared < 1.0f)
    {
        return  Vector2f::CreateZero();
    }

    float force =  BigG * mass * m_mass /(  distanceSquared);

    return normalVector * force;
}

 Vector2f Circ::CalculateGForce(Circ* entity) const
{
    return CalculateGForce(entity->GetMass(), entity->GetPosition());
}