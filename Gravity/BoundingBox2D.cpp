#include "BoundingBox2D.h"


    BoundingBox2D BoundingBox2D::CreateInvalid()
    {
        return { Vector2f{ 1.0f }, Vector2f{ -1.0f } };
    }

    BoundingBox2D BoundingBox2D::CreateFromDimensions(
        const Vector2f& minPoint, const Vector2f& dimensions)
    {
        return { minPoint, dimensions };
    }

    BoundingBox2D BoundingBox2D::CreateFromPoints(
        const std::vector<Vector2f>& points)
    {
        if (points.empty())
        {
            return BoundingBox2D::CreateInvalid();
        }

        Vector2f minPoint, maxPoint;
        minPoint = maxPoint = points[0];
        for (auto& point : points)
        {
            minPoint.x = std::min(minPoint.x, point.x);
            minPoint.y = std::min(minPoint.y, point.y);
            maxPoint.x = std::max(maxPoint.x, point.x);
            maxPoint.y = std::max(maxPoint.y, point.y);
        }

        return BoundingBox2D{ minPoint, maxPoint - minPoint };
    }

    BoundingBox2D::BoundingBox2D()
    {
        (*this) = BoundingBox2D::CreateInvalid();
    }

    BoundingBox2D::BoundingBox2D(
        const Vector2f& minPoint, const Vector2f& dimensions)
        : m_minPoint{ minPoint }
        , m_maxPoint{ minPoint + dimensions }
        , m_dimensions{ dimensions }
    {
    }

    bool BoundingBox2D::IsValid() const
    {
        return m_minPoint.x < m_maxPoint.x &&
            m_minPoint.y < m_maxPoint.y;
    }

    bool BoundingBox2D::IsPointWithinBounds(const Vector2f& point) const
    {
        return point.x >= m_minPoint.x && point.y >= m_minPoint.y &&
            point.x < m_maxPoint.x && point.y < m_maxPoint.y;
    }

    Vector2f BoundingBox2D::GetDimensions() const
    {
        return m_dimensions;
    }

    Vector2f BoundingBox2D::GetMinPoint() const
    {
        return m_minPoint;
    }

    Vector2f BoundingBox2D::GetMaxPoint() const
    {
        return m_maxPoint;
    }