#pragma once

#include "Vector2f.h"
#include <vector>

    //! A 2D bounding box class.
    //! The box is defined by a minimum point and a maximum point.
    class BoundingBox2D
    {
    public:
        static BoundingBox2D CreateInvalid();
        static BoundingBox2D CreateFromDimensions(
            const Vector2f& minPoint, const Vector2f& dimensions);
        static BoundingBox2D CreateFromPoints(
            const std::vector<Vector2f>& points);

        BoundingBox2D();
        BoundingBox2D(const Vector2f& minPoint, const Vector2f& dimensions);
        BoundingBox2D(const BoundingBox2D& other) = default;
        BoundingBox2D(BoundingBox2D&& other) = default;
        ~BoundingBox2D() = default;

        //! The Bounding box is valid if the minimum point is less than the
        //! maximum point.
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] bool IsPointWithinBounds(const Vector2f& point) const;

        [[nodiscard]] Vector2f GetDimensions() const;
        [[nodiscard]] Vector2f GetMinPoint() const;
        [[nodiscard]] Vector2f GetMaxPoint() const;

        BoundingBox2D& operator=(const BoundingBox2D& other) = default;

    private:
        Vector2f m_minPoint, m_maxPoint, m_dimensions;
    };