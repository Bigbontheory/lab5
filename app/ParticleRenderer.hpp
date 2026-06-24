#pragma once
#include <SFML/Graphics.hpp>
#include "../src/Particle.hpp"

class ParticleR {
public:
    ParticleR() = delete;

    static void draw(sf::RenderTarget& target, const Particle& particle) {
        float charge = particle.getCharge();
        sf::Vector2f position = particle.getPosition();

        float radius = 6.0f;
        sf::CircleShape shape(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);

        if (charge > 0.0f) {
            shape.setFillColor(sf::Color::Red);
        }
        else if (charge < 0.0f) {
            shape.setFillColor(sf::Color::Blue);
        }
        else {
            shape.setFillColor(sf::Color::White);
        }

        target.draw(shape);
    }
};