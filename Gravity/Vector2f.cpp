#include "Vector2f.h"
#include <cmath>


    Vector2f::Vector2f()
    {
        (*this) = Vector2f::CreateZero();
    }

    Vector2f::Vector2f(float x, float y)
        : x{ x }
        , y{ y }
    {
    }

    float Q_rsqrt(float number)
    {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *(long*)&y;                       // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1);               // what the fuck? 
        y = *(float*)&i;
        y = y * (threehalfs - (x2 * y * y));   // 1st iteration
        //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

        return y;
    }
    float FMod(float x, float y)
    {
        float mod = std::fmod((x < 0.0f) ? x + y : x, y);
        return mod;
    }
    Vector2f Vector2f::CreateZero()
    {
        return Vector2f{ 0.0f };
    }

    Vector2f::Vector2f(float value)
        : x{ value }
        , y{ value }
    {
    }

    float Vector2f::GetLengthSquared() const
    {
        return x * x + y * y;
    }

    float Vector2f::GetLength() const
    {
        return std::sqrt(GetLengthSquared());
    }

    Vector2f Vector2f::GetNormalized() const
    {
        return (*this) * Q_rsqrt(GetLengthSquared());
    }

    Vector2f Vector2f::GetMod(const Vector2f& other) const
    {
        return { FMod(x, other.x), FMod(y, other.y) };
    }

    float Vector2f::Dot(const Vector2f& vector) const
    {
        return x * vector.x + y * vector.y;
    }

    bool Vector2f::operator==(const Vector2f& other) const
    {
        return x == other.x && y == other.y;
    }

    bool Vector2f::operator!=(const Vector2f& other) const
    {
        return !((*this) == other);
    }

    Vector2f Vector2f::operator*(float value) const
    {
        return Vector2f(*this) *= value;
    }

    Vector2f Vector2f::operator/(float value) const
    {
        Vector2f vector = (*this);
        vector /= value;
        return vector;
    }

    Vector2f& Vector2f::operator*=(float value)
    {
        x *= value;
        y *= value;

        return (*this);
    }

    Vector2f& Vector2f::operator/=(float value)
    {
        x /= value;
        y /= value;

        return (*this);
    }

    Vector2f Vector2f::operator+(const Vector2f& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2f Vector2f::operator-(const Vector2f& other) const
    {
        return { x - other.x, y - other.y };
    }

    Vector2f Vector2f::operator*(const Vector2f& other) const
    {
        return { x * other.x, y * other.y };
    }

    Vector2f Vector2f::operator/(const Vector2f& other) const
    {
        return { x / other.x, y / other.y };
    }

    Vector2f& Vector2f::operator+=(const Vector2f& other)
    {
        return (*this) = (*this) + other;
    }

    Vector2f& Vector2f::operator-=(const Vector2f& other)
    {
        return (*this) = (*this) - other;
    }

    Vector2f& Vector2f::operator*=(const Vector2f& other)
    {
        return (*this) = (*this) * other;
    }

    Vector2f& Vector2f::operator/=(const Vector2f& other)
    {
        return (*this) = (*this) / other;
    }

    Vector2f operator-(const Vector2f& vector)
    {
        return { -vector.x, -vector.y };
    }

    Vector2f operator*(float lhs, const Vector2f& rhs)
    {
        return rhs * lhs;
    }