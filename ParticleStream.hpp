#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include "LAB2/mutable_array_sequence.hpp"

struct StreamParticle {
    sf::Vector2f position;
    float speedFactor;
};

class ParticleStream {
private:
    MutableArraySequence<StreamParticle> particles;
    int size;
    float streamY;
    float screenWidth;
    float baseVelocity;
    float averageCharge;

    static StreamParticle* generateInitialData(int count, float yLevel, float width) {
        if (count <= 0) return nullptr;
        StreamParticle* data = new StreamParticle[count];
        for (int i = 0; i < count; ++i) {
            data[i].position = sf::Vector2f(
                static_cast<float>(rand() % static_cast<int>(width)),
                yLevel + (rand() % 16 - 8)
            );
            data[i].speedFactor = 0.8f + (rand() % 40) / 100.0f;
        }
        return data;
    }

public:
    ParticleStream()
        : size(0), streamY(0.0f), screenWidth(1200.0f), baseVelocity(180.0f), averageCharge(1.0f) {
    }

    ParticleStream(int count, float yLevel, float width)
        : size(count), streamY(yLevel), screenWidth(width), baseVelocity(180.0f), averageCharge(1.0f),
        particles(generateInitialData(count, yLevel, width), count)
    {
    }

    void update(float dt) {
        if (size == 0) return;
        for (int i = 0; i < size; ++i) {
            StreamParticle& p = const_cast<StreamParticle&>(particles.get(i));

            p.position.x += baseVelocity * p.speedFactor * dt;

            if (p.position.x > screenWidth) {
                p.position.x = 0.0f;
                p.position.y = streamY + (rand() % 16 - 8);
            }
        }
    }

    float calculateMagneticFieldAt(const sf::Vector2f& targetPos) const {
        if (size == 0) return 0.0f;

        float totalB = 0.0f;
        float k = 0.005f;

        for (int i = 0; i < size; ++i) {
            const StreamParticle& p = particles.get(i);

            float dx = targetPos.x - p.position.x;
            float dy = targetPos.y - p.position.y;
            float r2 = dx * dx + dy * dy + 400.0f;
            float r = std::sqrt(r2);

            float B_particle = (averageCharge * (baseVelocity * p.speedFactor) * (-dy)) / (r2 * r);
            totalB += B_particle;
        }

        return totalB * k;
    }

    float& getVelocityRef() { return baseVelocity; }
    float& getChargeRef() { return averageCharge; }
    int get_size() const { return size; }

    void draw(sf::RenderWindow& window) const {
        if (size == 0) return;
        for (int i = 0; i < size; ++i) {
            const StreamParticle& p = particles.get(i);

            sf::CircleShape dot(3.0f);
            dot.setOrigin(3.0f, 3.0f);
            dot.setPosition(p.position);

            if (averageCharge > 0.0f) {
                dot.setFillColor(sf::Color(0, 255, 150, 220));
            }
            else if (averageCharge < 0.0f) {
                dot.setFillColor(sf::Color(255, 150, 0, 220));
            }
            else {
                dot.setFillColor(sf::Color(150, 150, 150, 150));
            }
            window.draw(dot);
        }
    }
};