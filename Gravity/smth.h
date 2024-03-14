#pragma once,
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include"Vector2f.h"
	sf::Vector2f VecToSFVec(Vector2f vec);
	Vector2f SFVrcToVec(sf::Vector2f);
    void DrawVector(
        sf::RenderWindow& window,
        const Vector2f& origin,
        const Vector2f& vector,
        const sf::Color& color = sf::Color::Green);

