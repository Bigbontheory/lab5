#pragma once
#include <SFML/Graphics.hpp>
#include "../src/ParticleStream.hpp" 

class ParticleStreamRenderer {
public:
    static void draw(sf::RenderWindow& window, const ParticleStream& stream) {
        int size = stream.get_size();
        if (size == 0) return;

        float charge = stream.getCharge();

        for (int i = 0; i < size; ++i) {
            const StreamParticle& p = stream.getParticle(i);

            sf::CircleShape dot(3.0f);
            dot.setOrigin(3.0f, 3.0f);
            dot.setPosition(p.position);

            if (charge > 0.0f) {
                dot.setFillColor(sf::Color(0, 255, 150, 220));
            }
            else if (charge < 0.0f) {
                dot.setFillColor(sf::Color(255, 150, 0, 220));
            }
            else {
                dot.setFillColor(sf::Color(150, 150, 150, 150));
            }
            window.draw(dot);
        }
    }
};