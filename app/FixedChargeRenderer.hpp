
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../src/FixedCharge.hpp"

class FixedChargeR {
public:
    static void draw(sf::RenderTarget& target, const FixedCharge& fc) {
        float charge = fc.getCharge();
        sf::Vector2f position = fc.getPosition();

        if (std::abs(charge) > 0.01f) {
            float glowRadius = 20.0f;
            sf::CircleShape glow(glowRadius);
            glow.setOrigin(glowRadius, glowRadius);
            glow.setPosition(position);

            if (charge > 0.0f) {
                glow.setFillColor(sf::Color(255, 60, 60, 45));
            }
            else {
                glow.setFillColor(sf::Color(60, 60, 255, 45));
            }
            target.draw(glow);
        }
        float radius = 6.0f;
        sf::CircleShape shape(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);

        if (charge > 0.0f) {
            shape.setFillColor(sf::Color(255, 50, 50));
            shape.setOutlineColor(sf::Color(255, 150, 150));
            shape.setOutlineThickness(2.0f);
        }
        else if (charge < 0.0f) {
            shape.setFillColor(sf::Color(50, 50, 255));
            shape.setOutlineColor(sf::Color(150, 150, 255));
            shape.setOutlineThickness(2.0f);
        }
        else {
            shape.setFillColor(sf::Color(120, 120, 120));
            shape.setOutlineThickness(0.0f);
        }

        target.draw(shape);
    }
};