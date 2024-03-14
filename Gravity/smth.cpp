#include "smth.h"

sf::Vector2f VecToSFVec(Vector2f vec)
{
	return sf::Vector2f{ vec.x,vec.y };
}

Vector2f SFVrcToVec(sf::Vector2f vec)
{
	return Vector2f(vec.x,vec.y);
}

void DrawVector(sf::RenderWindow& window, const Vector2f& origin, const Vector2f& vector, const sf::Color& color)
{
    sf::Vertex velocityLine[2] = {
           {
               VecToSFVec(origin),
               color,
           },
           {
               VecToSFVec(origin + vector),
               color,
           },
    };

    window.draw(velocityLine, 2, sf::Lines);
}
