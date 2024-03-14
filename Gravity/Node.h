#pragma once
#include"BoundingBox2D.h"
#include"Circ.h"
#include <array>
#include <cassert>
#include <iostream>
#include <stack>


enum Quadrant : int
{
    NONE = -1,
    NORTH_WEST = 0,
    NORTH_EAST = 1,
    SOUTH_WEST = 2,
    SOUTH_EAST = 3,
};

class Node
{
public:
    static constexpr size_t SUBQUADRANT_COUNT = 4;
    static const float PHI;

    explicit Node(const BoundingBox2D& boundingBox);
    Node(Node* parent, Quadrant parentSubquadrant);
    ~Node();

    void Update();
    void UpdateMass();
    void UpdateCenterOfMass();
    void Push(Circ* entity);

    void MoveUp(Circ* entity, Quadrant subquadrant, bool removeChild);

    void Draw(sf::RenderWindow& window) const;

    //! Recursively calculates forces for all
    //! bodies.
    void ApplyGForceToEntity(Circ* entity, bool useBarnesHut);

private:
    BoundingBox2D m_boundingBox;
    float m_mass{ 0.0f };
    Vector2f m_centerOfMass{ Vector2f::CreateZero() };

    Circ* m_assignedEntity{ nullptr };

    Node* m_parent;
    Quadrant m_subquadrant;
    std::array<Node*, SUBQUADRANT_COUNT> m_children{ { nullptr } };

    std::array<sf::Vertex, 8> m_lines;

    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] bool IsRoot() const;

    [[nodiscard]] bool HasNoChildren() const;
    
    [[nodiscard]] bool HasOnlyOneSubquadrant() const;
    
    bool IsEntityInside(Circ* entity) const;
 
    [[nodiscard]] bool IsAssignedEntityInside() const;

    [[nodiscard]] bool IsOnlyChild() const;

    void InitializeLines();
    void EnsureSubquadrantExists(Quadrant subquadrant);
    bool IsFarEnough(Circ* entity) const;

    [[nodiscard]] BoundingBox2D GetSubquadrantBoundingBox(Quadrant subquadrant) const;

    [[nodiscard]] Quadrant SelectSubquadrant(Vector2f position) const;
};

