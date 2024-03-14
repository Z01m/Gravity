#include "Node.h"
#include "QuadTree.h"
#include"smth.h"
#include<ranges>
const float Node::PHI = 0.5f;

Node::Node(const BoundingBox2D& boundingBox)
    : m_parent{ nullptr }
    , m_subquadrant{ NONE }
    , m_boundingBox{ boundingBox }
{
    InitializeLines();
}

Node::Node(Node* parent, Quadrant parentSubquadrant)
    : m_parent{ parent }
    , m_subquadrant{ parentSubquadrant }
    , m_boundingBox{ parent->GetSubquadrantBoundingBox(parentSubquadrant) }
{
    InitializeLines();
}

Node::~Node()
{
    for (Node* child : m_children)
    {
        delete child;
    }
}

void Node::Update()
{
    if (!IsEmpty())
    {
        const bool IsInside = IsAssignedEntityInside();
        Circ* tempEntity = m_assignedEntity;

        if (!IsInside || IsOnlyChild())
        {
            m_assignedEntity = nullptr;
            if (!IsRoot())
            {
                m_parent->MoveUp(tempEntity, m_subquadrant, true);
            }
        }
        return;
    }

    for (Node* child : m_children)
    {
        if (child)
        {
            child->Update();
        }
    }
}

void Node::UpdateMass()
{
    if (!IsEmpty())
    {
        m_mass = m_assignedEntity->GetMass();
        return;
    }

    m_mass = 0.0f;
    for (Node* child : m_children)
    {
        if (child)
        {
            child->UpdateMass();
            m_mass += child->m_mass;
        }
    }
}

void Node::UpdateCenterOfMass()
{
    if (!IsEmpty())
    {
        m_centerOfMass = m_assignedEntity->GetPosition();
        return;
    }

    m_centerOfMass = Vector2f::CreateZero();
    for (Node* child : m_children)
    {
        if (child)
        {
            child->UpdateCenterOfMass();
            m_centerOfMass += child->m_centerOfMass * child->m_mass;
        }
    }

    m_centerOfMass /= m_mass;
}

void Node::Push(Circ* entity)
{
    assert(entity != nullptr);

    if (IsEmpty() && HasNoChildren())
    {
        m_assignedEntity = entity;
        return;
    }

    if (!IsEmpty())
    {
        Circ* assignedEntity = m_assignedEntity;
        if (IsAssignedEntityInside())
        {
            m_assignedEntity = nullptr;
            const Quadrant tempQuadrant =
                SelectSubquadrant(assignedEntity->GetPosition());
            EnsureSubquadrantExists(tempQuadrant);
            m_children[tempQuadrant]->Push(assignedEntity);
        }
        else
        {
            m_assignedEntity = entity;
            m_parent->MoveUp(assignedEntity, m_subquadrant, false);
            return;
        }
    }

    const Quadrant quadrant = SelectSubquadrant(entity->GetPosition());
    EnsureSubquadrantExists(quadrant);
    m_children[quadrant]->Push(entity);
}

void Node::MoveUp(Circ* entity, Quadrant subquadrant, bool removeChild)
{
    if (removeChild)
    {
        QuadTree::StackPush(m_children[subquadrant]);
        m_children[subquadrant] = nullptr;
    }

    if (!IsEntityInside(entity))
    {
        if (IsRoot())
        {
            // Let the simulation detect this and remove it.
            return;
        }

        m_parent->MoveUp(entity, m_subquadrant, HasNoChildren());
        return;
    }

    if (!IsOnlyChild() || IsRoot() || !HasNoChildren())
    {
        Push(entity);
        return;
    }

    m_parent->MoveUp(entity, m_subquadrant, HasNoChildren());
}

void Node::Draw(sf::RenderWindow& window) const
{
    window.draw(m_lines.data(), m_lines.size(), sf::Lines);

    for (Node* child : m_children)
    {
        if (child)
        {
            child->Draw(window);
        }
    }
}

void Node::ApplyGForceToEntity(Circ* entity, bool useBarnesHut)
{
    if (!IsEmpty() && entity != m_assignedEntity)
    {
        entity->ApplyForce(entity->CalculateGForce(m_assignedEntity));
        return;
    }

    if (useBarnesHut && IsFarEnough(entity))
    {
        entity->ApplyForce(entity->CalculateGForce(m_mass, m_centerOfMass));
        return;
    }

    for (Node* child : m_children)
    {
        if (child)
        {
            child->ApplyGForceToEntity(entity, useBarnesHut);
        }
    }
}

inline bool Node::IsEmpty() const
{
    return !m_assignedEntity;
}

bool Node::IsRoot() const
{
    return !m_parent;
}

inline bool Node::HasNoChildren() const
{
    return std::ranges::none_of(
        m_children.begin(),
        m_children.end(),
        [](Node* node)
        {
            return node;
        });
}

bool Node::HasOnlyOneSubquadrant() const
{
    int subquadrantCount = 0;
    for (const Node* child : m_children)
    {
        if (child && ++subquadrantCount > 1)
        {
            return false;
        }
    }

    return subquadrantCount == 1;
}

bool Node::IsEntityInside(Circ* entity) const
{
    return m_boundingBox.IsPointWithinBounds(entity->GetPosition());
}

bool Node::IsAssignedEntityInside() const
{
    assert(!IsEmpty());
    return IsEntityInside(m_assignedEntity);
}

bool Node::IsOnlyChild() const
{
    if (!m_parent)
    {
        return true;
    }

    return m_parent->HasOnlyOneSubquadrant();
}

void Node::InitializeLines()
{
    static const sf::Color color = { 32, 94, 37, 255 };

    const sf::Vector2f UpperLeftCorner =
        VecToSFVec(m_boundingBox.GetMinPoint());

    const sf::Vector2f UpperRightCorner = VecToSFVec(
        m_boundingBox.GetMinPoint() +
        Vector2f{ m_boundingBox.GetDimensions().x, 0.0f });

    const sf::Vector2f LowerLeftCorner = VecToSFVec(
        m_boundingBox.GetMinPoint() +
        Vector2f{ 0.0f, m_boundingBox.GetDimensions().y });

    const sf::Vector2f LowerRightCorner =
        VecToSFVec(m_boundingBox.GetMaxPoint());

    m_lines = {
        sf::Vertex{ UpperLeftCorner, color },
        sf::Vertex{ UpperRightCorner, color },
        sf::Vertex{ UpperRightCorner, color },
        sf::Vertex{ LowerRightCorner, color },
        sf::Vertex{ LowerRightCorner, color },
        sf::Vertex{ LowerLeftCorner, color },
        sf::Vertex{ LowerLeftCorner, color },
        sf::Vertex{ UpperLeftCorner, color },
    };
}

void Node::EnsureSubquadrantExists(Quadrant subquadrant)
{
    if (m_children[subquadrant])
    {
        return;
    }

    m_children[subquadrant] = new Node(this, subquadrant);
}

bool Node::IsFarEnough(Circ* entity) const
{
    return std::max(
        m_boundingBox.GetDimensions().x,
        m_boundingBox.GetDimensions().y) <
        Node::PHI * (m_centerOfMass - entity->GetPosition()).GetLength();
}

  BoundingBox2D Node::GetSubquadrantBoundingBox(
    Quadrant subquadrant) const
{
        Vector2f subquadrantMinPoint = m_boundingBox.GetMinPoint();
        Vector2f subquadrantDimensions =
        m_boundingBox.GetDimensions() / 2.0f;

    switch (subquadrant)
    {
    case Quadrant::NORTH_EAST:
        subquadrantMinPoint.x += subquadrantDimensions.x;
        break;
    case Quadrant::SOUTH_WEST:
        subquadrantMinPoint.y += subquadrantDimensions.y;
        break;
    case Quadrant::SOUTH_EAST:
        subquadrantMinPoint += subquadrantDimensions;
        break;
    default:
        break;
    }

    return { subquadrantMinPoint, subquadrantDimensions };
}

Quadrant Node::SelectSubquadrant(Vector2f position) const
{
    assert(m_boundingBox.GetDimensions() != Vector2f::CreateZero());

    const Vector2f middlePoint =
        m_boundingBox.GetMinPoint() + m_boundingBox.GetDimensions() / 2.0f;

    bool x_less, y_less;
    x_less = position.x < middlePoint.x;
    y_less = position.y < middlePoint.y;

    if (x_less)
    {
        if (y_less)
        {
            return Quadrant::NORTH_WEST;
        }
        else
        {
            return Quadrant::SOUTH_WEST;
        }
    }
    else if (y_less)
    {
        return Quadrant::NORTH_EAST;
    }

    return Quadrant::SOUTH_EAST;
}