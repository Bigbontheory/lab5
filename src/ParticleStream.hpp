#pragma once

#include <cstdlib>
#include <cmath>
#include "../LAB2/mutable_array_sequence.hpp"
#include "PhysicsScale.hpp"

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
            sf::Vector2f screenPos(
                static_cast<float>(rand() % static_cast<int>(width)),
                yLevel + (rand() % 16 - 8)
            );
            data[i].position = PhysicsScale::toPhysics(screenPos);
            data[i].speedFactor = 0.8f + (rand() % 40) / 100.0f;
        }
        return data;
    }

public:
    ParticleStream()
        : size(0),
         streamY(0.0f),
         screenWidth(PhysicsScale::toPhysics(1200.0f)),
         baseVelocity(180.0f),
         averageCharge(1.0f)
    {
    }

    ParticleStream(int count, float yLevel, float width) :
         particles(generateInitialData(count, yLevel, width), count),
         size(count),
         streamY(PhysicsScale::toPhysics(yLevel)),
         screenWidth(PhysicsScale::toPhysics(width)),
         baseVelocity(180.0f),
         averageCharge(1.0f)
    {
    }

    void update(float dt) {
        if (size == 0) return;
        float physVel = PhysicsScale::toPhysics(baseVelocity);

        for (int i = 0; i < size; ++i) {
            StreamParticle p = particles.get(i);

            p.position.x += physVel * p.speedFactor * dt;

            if (p.position.x > screenWidth) {
                p.position.x = 0.0f;
                float screenY = PhysicsScale::toPixels(streamY) + (rand() % 16 - 8);
                p.position.y = PhysicsScale::toPhysics(screenY);
            }
            particles.set(i, p);
        }
    }

    float calculateMagneticFieldAt(const sf::Vector2f& targetPos) const {
        if (size == 0) return 0.0f;

        float totalB = 0.0f;
        float k = 1000.0f;
        float physVel = PhysicsScale::toPhysics(baseVelocity);
        float physCharge = PhysicsScale::chargeToCoulombs(averageCharge);

        for (int i = 0; i < size; ++i) {
            const StreamParticle& p = particles.get(i);

            float dx = targetPos.x - p.position.x;
            float dy = targetPos.y - p.position.y;
            float r2 = dx * dx + dy * dy + 0.04f;
            float r = std::sqrt(r2);

            float B_particle = (physCharge * (physVel * p.speedFactor) * (-dy)) / (r2 * r);
            totalB += B_particle;
        }

        return totalB * k;
    }

    float& getVelocityRef() { return baseVelocity; }
    float& getChargeRef() { return averageCharge; }
    int get_size() const { return size; }
    float getVelocity() const { return baseVelocity; }
    float getCharge() const { return averageCharge; }

    const StreamParticle& getParticle(int index) const { return particles.get(index); }
};